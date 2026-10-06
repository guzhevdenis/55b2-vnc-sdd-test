#include <nds3/nds.h>

#include <iostream>

#include <vector>

#include "./include/DanDataGenerator.h"

#include <cmath>
 // This class declares our device in the contructor and supplies the functionalities to support it
#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */
// Keep NELM in DeviceNDSDANCP.substitutions in sync with this value.
#define SIZE 4096

//general type of mutex for working with threads 
static std::map < std::string, DanDataGenerator * > m_DeviceMap;

//general type of mutex for working with threads 
static std::mutex m_lockDeviceMap;

// Constructor. Here we setup the device structure
DanDataGenerator::DanDataGenerator(nds::Factory & factory,
  const std::string & deviceName,
    const nds::namedParameters_t & parameters) {

  nds::Port rootNode(deviceName);
  std::lock_guard < std::mutex > lock(m_lockDeviceMap);

  if (m_DeviceMap.find(deviceName) != m_DeviceMap.end()) {
    throw std::logic_error("Device with the same name already allocated. This should not happen");
  }
  m_DeviceMap[deviceName] = this;

  m_bStop_DataAcquisition = true;
  m_acquisitionThreadRunning = false;

  /*Creation of a port-node to communicate with control system*/

  nds::autoEnable_t autoEnable = nds::autoEnable_t::none;

  //Initialization parameters. It is located in userPreDriverConf.cmd file
  nds::namedParameters_t::const_iterator findParam = parameters.find("INIT");

  findParam = parameters.find("INIT");
  if (findParam != parameters.end() && findParam -> second == "YES") {
    /*Data acqusition handler*/
    nds::DataAcquisitionArgs_t handlerDAQ = nds::DataAcquisitionArgs_t(
      std::bind( & DanDataGenerator::switchOn_DataAcquisition, this),
      std::bind( & DanDataGenerator::switchOff_DataAcquisition, this),
      std::bind( & DanDataGenerator::start_DataAcquisition, this),
      std::bind( & DanDataGenerator::stop_DataAcquisition, this),
      std::bind( & DanDataGenerator::recover_DataAcquisition, this),
      std::bind( & DanDataGenerator::allow_DataAcquisition_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
      std::bind( & DanDataGenerator::PV_DataAcquisition_Gain_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_Offset_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_Bandwidth_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_Resolution_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_Impedance_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_Coupling_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_SignalRefType_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_Ground_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_DMAEnable_Writer, this, std::placeholders::_1, std::placeholders::_2),
      std::bind( & DanDataGenerator::PV_DataAcquisition_SamplingRate_Writer, this, std::placeholders::_1, std::placeholders::_2),
      autoEnable
    );

    //Optional methods
    handlerDAQ.PV_Gain_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_Gain_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Offset_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_Offset_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Bandwidth_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_Bandwidth_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Resolution_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_Resolution_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Impedance_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_Impedance_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Coupling_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_Coupling_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_SignalRefType_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_SignalRefType_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Ground_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_Ground_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_DMAEnable_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_DMAEnable_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_SamplingRate_Initializer = std::bind( & DanDataGenerator::PV_DataAcquisition_SamplingRate_Initializer, this, std::placeholders::_1, std::placeholders::_2);
    //Init values
    handlerDAQ.m_Decimation_Init = 1;
    handlerDAQ.m_DecimationType_Init = 2;
    m_DataAcquisition = rootNode.addChild(nds::DataAcquisition < std::vector < std::int16_t >> (
      "DataAcquisitionNode",
      SIZE,
      handlerDAQ));
    {
      const timespec initTs = wallClockTimestamp();
      /*m_DataAcquisition.setDecimation(initTs, handlerDAQ.m_Decimation_Init);
      m_DataAcquisition.setDecimationType(initTs, handlerDAQ.m_DecimationType_Init);*/
    }
  } else {
    m_DataAcquisition = rootNode.addChild(nds::DataAcquisition < std::vector < std::int16_t >>
      ("DataAcquisitionNode",
        SIZE, std::bind( & DanDataGenerator::switchOn_DataAcquisition, this),
        std::bind( & DanDataGenerator::switchOff_DataAcquisition, this),
        std::bind( & DanDataGenerator::start_DataAcquisition, this),
        std::bind( & DanDataGenerator::stop_DataAcquisition, this),
        std::bind( & DanDataGenerator::recover_DataAcquisition, this),
        std::bind( & DanDataGenerator::allow_DataAcquisition_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind( & DanDataGenerator::PV_DataAcquisition_Gain_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_Offset_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_Bandwidth_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_Resolution_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_Impedance_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_Coupling_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_SignalRefType_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_Ground_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_DMAEnable_Writer, this, std::placeholders::_1, std::placeholders::_2),
        std::bind( & DanDataGenerator::PV_DataAcquisition_SamplingRate_Writer, this, std::placeholders::_1, std::placeholders::_2),
        autoEnable
      ));
  }

  rootNode.initialize(this, factory);
}

// If we want to load our device support dynamically then we have to compile it
// as a dynamic module and we have to provide the functions to allocate it,
// deallocate it and retrieve its name

NDS_DEFINE_DRIVER(DanDataGenerator, DanDataGenerator)