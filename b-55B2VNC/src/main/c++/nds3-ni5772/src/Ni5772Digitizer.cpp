/**
 * @file Ni5772Digitizer.cpp
 *
 * @authors Denis Guzhev, Danila Avdushkin
 * @date 22 Oct 2024
 */

#include "./../include/Ni5772Digitizer.h"


#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

// the number of elements to read from host-FIFO
#define SIZE 800000

// general type of mutex for working with threads
static std::map<std::string, Ni5772Digitizer*> m_DeviceMap;
// general type of mutex for working with threads
static std::mutex m_lockDeviceMap;

// constructor with all parameters
Ni5772Digitizer::Ni5772Digitizer(nds::Factory& factory,
                                 const std::string& deviceName,
                                 const nds::namedParameters_t& parameters)
    : m_name(deviceName) {
  std::lock_guard<std::mutex> lock(m_lockDeviceMap);

  if (m_DeviceMap.find(deviceName) != m_DeviceMap.end()) {
    throw std::logic_error(
        "Device with the same name already allocated. This should not happen");
    m_DeviceMap[deviceName] = this;
  }

  /*Creation of a port-node to communicate with control system*/
  nds::Port rootNode(deviceName);

  // Initialization parameters. It is located in userPreDriverConf.cmd file
  nds::namedParameters_t::const_iterator findParam = parameters.find("INIT");

  // adding a state machine node
  nds::StateMachine m_Device_stateMachine = rootNode.addChild(nds::StateMachine(
      true, std::bind(&Ni5772Digitizer::switchOn_Device, this),
      std::bind(&Ni5772Digitizer::switchOff_Device, this),
      std::bind(&Ni5772Digitizer::start_Device, this),
      std::bind(&Ni5772Digitizer::stop_Device, this),
      std::bind(&Ni5772Digitizer::recover_Device, this),
      std::bind(&Ni5772Digitizer::allow_Device_Change, this,
                std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)));

  // The parameter used to follow the parent node transitions. None - the node
  // won't follow it's parent node transition.
  nds::autoEnable_t autoEnable = nds::autoEnable_t::none;

  // Transitions type of STM can also be defined in userPreDriverConf.com file.
  if (findParam != parameters.end()) {
    if (findParam->second == "OFF") {
      autoEnable = nds::autoEnable_t::off;
    } else if (findParam->second == "ON") {
      autoEnable = nds::autoEnable_t::on;
    } else if (findParam->second == "RUNNING") {
      autoEnable = nds::autoEnable_t::running;
    }
  }

  NiFpga_MergeStatus(&status,
                     NiFpga_Open("./../bitfile/NiFpga_exp_test_FIC.lvbitx",
                                 NiFpga_exp_test_FIC_Signature, "RIO0", 0,
                                 &session));  

  /*FPGA Boolean Indicators*/
  nds::PVBaseIn Clock100PLLUnlocked_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "Clock100PLLUnlocked",
          std::bind(&Ni5772Digitizer::Clock100PLLUnlocked_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn EEPROMPowerEnabled_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "EEPROMPowerEnabled",
          std::bind(&Ni5772Digitizer::EEPROMPowerEnabled_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn IOModuleIOEnabled_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "IOModuleIOEnabled",
          std::bind(&Ni5772Digitizer::IOModuleIOEnabled_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn IOModuleInitializationDone_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "IOModuleInitializationDone",
          std::bind(&Ni5772Digitizer::IOModuleInitializationDone_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn IOModulePowerEnabled_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "IOModulePowerEnabled",
          std::bind(&Ni5772Digitizer::IOModulePowerEnabled_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn IOModulePowerGood_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "IOModulePowerGood",
          std::bind(&Ni5772Digitizer::IOModulePowerGood_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn IOModulePresent_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "IOModulePresent",
          std::bind(&Ni5772Digitizer::IOModulePresent_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn IndicatorBoolKey_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "IndicatorBoolKey",
          std::bind(&Ni5772Digitizer::IndicatorBoolKey_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  /*FPGA I8 Indicators*/
  nds::PVBaseIn DeviceTemperature_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "DeviceTemperature",
          std::bind(&Ni5772Digitizer::DeviceTemperature_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));
  /*FPGA I16 Indicators*/
  nds::PVBaseIn ResultAfterSub_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "ResultAfterSub",
          std::bind(&Ni5772Digitizer::ResultAfterSub_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));
  /*FPGA I32 Indicators*/
  nds::PVBaseIn Counter1_m = rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
      "Counter1", std::bind(&Ni5772Digitizer::Counter1_Reader, this,
                            std::placeholders::_1, std::placeholders::_2)));
  nds::PVBaseIn Counter2_m = rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
      "Counter2", std::bind(&Ni5772Digitizer::Counter2_Reader, this,
                            std::placeholders::_1, std::placeholders::_2)));
  nds::PVBaseIn Counter3_m = rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
      "Counter3", std::bind(&Ni5772Digitizer::Counter3_Reader, this,
                            std::placeholders::_1, std::placeholders::_2)));
  nds::PVBaseIn CounterTemp_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "CounterTemp",
          std::bind(&Ni5772Digitizer::CounterTemp_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));
  /*FPGA U64 Indicators*/
  nds::PVBaseIn SinglePulsesCountRate_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "SinglePulsesCountRate",
          std::bind(&Ni5772Digitizer::SinglePulsesCountRate_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn SinglePulsesRegistered_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "SinglePulsesRegistered",
          std::bind(&Ni5772Digitizer::SinglePulsesRegistered_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseIn TotalCountRate_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "TotalCountRate",
          std::bind(&Ni5772Digitizer::TotalCountRate_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));
  nds::PVBaseIn TotalPulsesRegistered_m =
      rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
          "TotalPulsesRegistered",
          std::bind(&Ni5772Digitizer::TotalPulsesRegistered_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  /*FPGA Boolean Controls*/
  nds::PVBaseOut ThresholdValid_m =
      rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
          "ThresholdValid",
          std::bind(&Ni5772Digitizer::ThresholdValid_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseOut ControlKey_m =
      rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
          "ControlKey",
          std::bind(&Ni5772Digitizer::ControlKey_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));
  /*FPGA I16 Controls*/
  nds::PVBaseOut Alpha16BitFirst_m =
      rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
          "Alpha16BitFirst",
          std::bind(&Ni5772Digitizer::Alpha16BitFirst_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseOut Alpha16BitSecond_m =
      rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
          "Alpha16BitSecond",
          std::bind(&Ni5772Digitizer::Alpha16BitSecond_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseOut Threshold_m =
      rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
          "Threshold",
          std::bind(&Ni5772Digitizer::Threshold_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  /*FPGA U16 Controls*/
  nds::PVBaseOut FilterType_m =
      rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
          "FilterType",
          std::bind(&Ni5772Digitizer::FilterType_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  /*FPGA U32 Controls*/
  nds::PVBaseOut MaxTicksPerPulse_m =
      rootNode.addChild(nds::PVDelegateOut<std::int64_t>(
          "MaxTicksPerPulse",
          std::bind(&Ni5772Digitizer::MaxTicksPerPulse_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseOut MinTicksPerPulse_m =
      rootNode.addChild(nds::PVDelegateOut<std::int64_t>(
          "MinTicksPerPulse",
          std::bind(&Ni5772Digitizer::MinTicksPerPulse_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseOut MinTicksPerPulse2_m =
      rootNode.addChild(nds::PVDelegateOut<std::int64_t>(
          "MinTicksPerPulse2",
          std::bind(&Ni5772Digitizer::MinTicksPerPulse2_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  nds::PVBaseOut CalibrationCoefficient_m =
      rootNode.addChild(nds::PVDelegateOut<double>(
          "CalibrationCoefficient",
          std::bind(&Ni5772Digitizer::CalibrationCoefficient_Writer, this,
                    std::placeholders::_1, std::placeholders::_2)));

  // Debug and Not FPGA Variables
  nds::PVBaseIn Status_m = rootNode.addChild(nds::PVDelegateIn<std::int32_t>(
      "Status", std::bind(&Ni5772Digitizer::Status_Reader, this,
                          std::placeholders::_1, std::placeholders::_2)));
  nds::PVBaseOut Reseter_m = rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
      "Reseter", std::bind(&Ni5772Digitizer::Reseter_Writer, this,
                           std::placeholders::_1, std::placeholders::_2)));

  MaximumAmplitude_m =
      rootNode.addChild(nds::PVVariableIn<double>(
          "MaximumAmplitude"));


  //Test Neutron Profile
  nds::PVBaseIn NeutronProfile_Ox_m =
      rootNode.addChild(nds::PVDelegateIn<std::vector<std::int32_t>>(
          "NeutronProfile",
          std::bind(&Ni5772Digitizer::NeutronProfile_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));
  NeutronProfile_Ox_m.setMaxElements(100);

  status = NiFpga_ConfigureFifo2(
      session, NiFpga_exp_test_FIC_TargetToHostFifoI16_PulsePeaksFIFO,
      1023 * 100000, &PulsePeaksFIFO_depth);

  nds::PVBaseIn CountRateAverage_m =
      rootNode.addChild(nds::PVDelegateIn<double>(
          "CountRateAverage",
          std::bind(&Ni5772Digitizer::CountRateAverage_Reader, this,
                    std::placeholders::_1, std::placeholders::_2)));

  status = NiFpga_ConfigureFifo2(
      session, NiFpga_exp_test_FIC_TargetToHostFifoU64_Countratefluctuation,
      1023 * 100000, &CountRateFluctuationFIFO_depth);


  nds::PVBaseIn MSVAverage_m = rootNode.addChild(nds::PVDelegateIn<double>(
      "MSV", std::bind(&Ni5772Digitizer::MSV_Reader, this,
                       std::placeholders::_1, std::placeholders::_2)));
  status = NiFpga_ConfigureFifo2(session,
                                 NiFpga_exp_test_FIC_TargetToHostFifoU64_MSVADC,
                                 1023 * 100000, &MSV_depth);
  /*The size of the bin*/
  Histogram_Bin_Step_m = rootNode.addChild(nds::PVVariableOut<std::int32_t>("Step")); 
  /*Data Acqusition Node*/

  findParam = parameters.find("INIT");
  if (findParam != parameters.end() && findParam->second == "YES") {
    /*Data acqusition handler*/
    nds::DataAcquisitionArgs_t handlerDAQ = nds::DataAcquisitionArgs_t(
        std::bind(&Ni5772Digitizer::switchOn_DataAcquisition, this),
        std::bind(&Ni5772Digitizer::switchOff_DataAcquisition, this),
        std::bind(&Ni5772Digitizer::start_DataAcquisition, this),
        std::bind(&Ni5772Digitizer::stop_DataAcquisition, this),
        std::bind(&Ni5772Digitizer::recover_DataAcquisition, this),
        std::bind(&Ni5772Digitizer::allow_DataAcquisition_Change, this,
                  std::placeholders::_1, std::placeholders::_2,
                  std::placeholders::_3),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Gain_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Offset_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Bandwidth_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Resolution_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Impedance_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Coupling_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_SignalRefType_Writer,
                  this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Ground_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_DMAEnable_Writer, this,
                  std::placeholders::_1, std::placeholders::_2),
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_SamplingRate_Writer,
                  this, std::placeholders::_1, std::placeholders::_2),
        autoEnable);

    // Optional methods
    handlerDAQ.PV_Gain_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Gain_Initializer, this,
                  std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Offset_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Offset_Initializer, this,
                  std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Bandwidth_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Bandwidth_Initializer,
                  this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Resolution_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Resolution_Initializer,
                  this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Impedance_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Impedance_Initializer,
                  this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Coupling_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Coupling_Initializer,
                  this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_SignalRefType_Initializer = std::bind(
        &Ni5772Digitizer::PV_DataAcquisition_SignalRefType_Initializer, this,
        std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_Ground_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_Ground_Initializer, this,
                  std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_DMAEnable_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_DMAEnable_Initializer,
                  this, std::placeholders::_1, std::placeholders::_2);
    handlerDAQ.PV_SamplingRate_Initializer =
        std::bind(&Ni5772Digitizer::PV_DataAcquisition_SamplingRate_Initializer,
                  this, std::placeholders::_1, std::placeholders::_2);
    // Init values
    handlerDAQ.m_Decimation_Init = 1;
    handlerDAQ.m_DecimationType_Init = 2;
    m_DataAcquisition =
        rootNode.addChild(nds::DataAcquisition<std::vector<std::int16_t>>(
            "DataAcquisitionNode", SIZE, handlerDAQ));
  } else {
    m_DataAcquisition =
        rootNode.addChild(nds::DataAcquisition<std::vector<std::int16_t>>(
            "DataAcquisitionNode", SIZE,
            std::bind(&Ni5772Digitizer::switchOn_DataAcquisition, this),
            std::bind(&Ni5772Digitizer::switchOff_DataAcquisition, this),
            std::bind(&Ni5772Digitizer::start_DataAcquisition, this),
            std::bind(&Ni5772Digitizer::stop_DataAcquisition, this),
            std::bind(&Ni5772Digitizer::recover_DataAcquisition, this),
            std::bind(&Ni5772Digitizer::allow_DataAcquisition_Change, this,
                      std::placeholders::_1, std::placeholders::_2,
                      std::placeholders::_3),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_Gain_Writer, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_Offset_Writer, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_Bandwidth_Writer,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_Resolution_Writer,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_Impedance_Writer,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_Coupling_Writer,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_SignalRefType_Writer,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_Ground_Writer, this,
                      std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_DMAEnable_Writer,
                      this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&Ni5772Digitizer::PV_DataAcquisition_SamplingRate_Writer,
                      this, std::placeholders::_1, std::placeholders::_2),
            autoEnable));
  }

  /*initialization of port*/
  rootNode.initialize(this, factory);
  
}

// Destructor. It closes the FPGA session, releases the fifo elements, releases
// the name of the device.
Ni5772Digitizer::~Ni5772Digitizer() {
  status = NiFpga_ReleaseFifoElements(
      session, NiFpga_exp_test_FIC_TargetToHostFifoI16_RawData,
      RawDataFIFO_depth);
  status = NiFpga_ReleaseFifoElements(
      session, NiFpga_exp_test_FIC_TargetToHostFifoI16_PulsePeaksFIFO,
      PulsePeaksFIFO_depth);
  status = NiFpga_ReleaseFifoElements(
      session, NiFpga_exp_test_FIC_TargetToHostFifoU64_Countratefluctuation,
      CountRateFluctuationFIFO_depth);
  status = NiFpga_ReleaseFifoElements(
      session, NiFpga_exp_test_FIC_TargetToHostFifoU64_MSVADC,
      MSVADCFIFO_depth);
  NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
  std::lock_guard<std::mutex> lock(m_lockDeviceMap);
  m_DeviceMap.erase(m_name);
}

/*find device function*/
Ni5772Digitizer* Ni5772Digitizer::getInstance(const std::string& deviceName) {
  std::lock_guard<std::mutex> lock(m_lockDeviceMap);
  std::map<std::string, Ni5772Digitizer*>::const_iterator findDevice =
      m_DeviceMap.find(deviceName);

  if (findDevice == m_DeviceMap.end()) {
    return 0;
  }
  return findDevice->second;
}

void* Ni5772Digitizer::allocateDevice(
    nds::Factory& factory, const std::string& DeviceName,
    const nds::namedParameters_t& parameters) {
  return new Ni5772Digitizer(factory, DeviceName, parameters);
}

/*Deallocation function*/
void Ni5772Digitizer::deallocateDevice(void* DeviceName) {
  // free the allocated memory
  m_DeviceMap.erase(((Ni5772Digitizer*)DeviceName)->m_name);
  delete (Ni5772Digitizer*)DeviceName;
}

/*Delegate function to read proccess variables*/
/*Functions for Reading variables from FPGA*/
// debug

void Ni5772Digitizer::Status_Reader(const timespec* timestamp,
                                    std::int32_t* pIn) {
  *pIn = int(status);
}
void Ni5772Digitizer::Reseter_Writer(const timespec& timestamp,
                                     const std::int32_t& pOut) {
  if (pOut == 1) {
    NiFpga_Reset(session);
    NiFpga_Run(session, 0);
  }
}

/*Bool*/
void Ni5772Digitizer::Clock100PLLUnlocked_Reader(const timespec* timestamp,
                                                 std::int32_t* pIn) {
  status = NiFpga_ReadBool(
      session, NiFpga_exp_test_FIC_IndicatorBool_Clock100PLLUnlocked,
      (NiFpga_Bool*)(pIn));
}
void Ni5772Digitizer::EEPROMPowerEnabled_Reader(const timespec* timestamp,
                                                std::int32_t* pIn) {
  status = NiFpga_ReadBool(session,
                           NiFpga_exp_test_FIC_IndicatorBool_EEPROMPowerEnabled,
                           (NiFpga_Bool*)(pIn));
}
void Ni5772Digitizer::IOModuleIOEnabled_Reader(const timespec* timestamp,
                                               std::int32_t* pIn) {
  status = NiFpga_ReadBool(session,
                           NiFpga_exp_test_FIC_IndicatorBool_IOModuleIOEnabled,
                           (NiFpga_Bool*)(pIn));
}
void Ni5772Digitizer::IOModuleInitializationDone_Reader(
    const timespec* timestamp, std::int32_t* pIn) {
  status = NiFpga_ReadBool(
      session, NiFpga_exp_test_FIC_IndicatorBool_IOModuleInitializationDone,
      (NiFpga_Bool*)(pIn));
}
void Ni5772Digitizer::IOModulePowerEnabled_Reader(const timespec* timestamp,
                                                  std::int32_t* pIn) {
  status = NiFpga_ReadBool(
      session, NiFpga_exp_test_FIC_IndicatorBool_IOModulePowerEnabled,
      (NiFpga_Bool*)(pIn));
}
void Ni5772Digitizer::IOModulePowerGood_Reader(const timespec* timestamp,
                                               std::int32_t* pIn) {
  status = NiFpga_ReadBool(session,
                           NiFpga_exp_test_FIC_IndicatorBool_IOModulePowerGood,
                           (NiFpga_Bool*)(pIn));
}
void Ni5772Digitizer::IOModulePresent_Reader(const timespec* timestamp,
                                             std::int32_t* pIn) {
  status = NiFpga_ReadBool(session,
                           NiFpga_exp_test_FIC_IndicatorBool_IOModulePresent,
                           (NiFpga_Bool*)(pIn));
}
void Ni5772Digitizer::IndicatorBoolKey_Reader(const timespec* timestamp,
                                              std::int32_t* pIn) {
  status = NiFpga_ReadBool(session, NiFpga_exp_test_FIC_IndicatorBool_Key,
                           (NiFpga_Bool*)(pIn));
}

/*I8*/
void Ni5772Digitizer::DeviceTemperature_Reader(const timespec* timestamp,
                                               std::int32_t* pIn) {
  std::int8_t temperature = 0;
  status = NiFpga_ReadI8(
      session, NiFpga_exp_test_FIC_IndicatorI8_DeviceTemperature, &temperature);
  *pIn = static_cast<std::int32_t>(temperature);
}

/*I16*/
void Ni5772Digitizer::ResultAfterSub_Reader(const timespec* timestamp,
                                            std::int32_t* pIn) {
  std::int16_t tmp = 0;
  status = NiFpga_ReadI16(
      session, NiFpga_exp_test_FIC_IndicatorI16_ResultAfterSub, &tmp);
  *pIn = tmp;
}

/*I32*/
void Ni5772Digitizer::Counter1_Reader(const timespec* timestamp,
                                      std::int32_t* pIn) {
  status =
      NiFpga_ReadI32(session, NiFpga_exp_test_FIC_IndicatorI32_Counter1, pIn);
}
void Ni5772Digitizer::Counter2_Reader(const timespec* timestamp,
                                      std::int32_t* pIn) {
  status =
      NiFpga_ReadI32(session, NiFpga_exp_test_FIC_IndicatorI32_Counter2, pIn);
}
void Ni5772Digitizer::Counter3_Reader(const timespec* timestamp,
                                      std::int32_t* pIn) {
  status =
      NiFpga_ReadI32(session, NiFpga_exp_test_FIC_IndicatorI32_Counter3, pIn);
}
void Ni5772Digitizer::CounterTemp_Reader(const timespec* timestamp,
                                         std::int32_t* pIn) {
  status = NiFpga_ReadI32(session, NiFpga_exp_test_FIC_IndicatorI32_CounterTemp,
                          pIn);
}

void Ni5772Digitizer::SinglePulsesCountRate_Reader(const timespec* timestamp,
                                                   std::int32_t* pIn) {
  status = NiFpga_ReadI32(
      session, NiFpga_exp_test_FIC_IndicatorI32_SinglePulsesCountRate, pIn);
}
void Ni5772Digitizer::SinglePulsesRegistered_Reader(const timespec* timestamp,
                                                    std::int32_t* pIn) {
  status = NiFpga_ReadI32(
      session, NiFpga_exp_test_FIC_IndicatorI32_SinglePulsesRegistered, pIn);
}
void Ni5772Digitizer::TotalCountRate_Reader(const timespec* timestamp,
                                            std::int32_t* pIn) {
  status = NiFpga_ReadI32(session,
                          NiFpga_exp_test_FIC_IndicatorI32_TotalCountRate, pIn);
}
void Ni5772Digitizer::TotalPulsesRegistered_Reader(const timespec* timestamp,
                                                   std::int32_t* pIn) {
  status = NiFpga_ReadI32(
      session, NiFpga_exp_test_FIC_IndicatorI32_TotalPulsesRegistered, pIn);
}

// Functions for control variables FPGA
// Bool
void Ni5772Digitizer::ThresholdValid_Writer(const timespec& timestamp,
                                            const std::int32_t& pOut) {
  status =
      NiFpga_WriteBool(session, NiFpga_exp_test_FIC_ControlBool_ThresholdValid,
                       (NiFpga_Bool)pOut);
}
void Ni5772Digitizer::ControlKey_Writer(const timespec& timestamp,
                                        const std::int32_t& pOut) {
  status = NiFpga_WriteBool(session, NiFpga_exp_test_FIC_ControlBool_ControlKey,
                            (NiFpga_Bool)pOut);
}
// I16
void Ni5772Digitizer::Alpha16BitFirst_Writer(const timespec& timestamp,
                                             const std::int32_t& pOut) {
  std::int16_t tmp;
  tmp = pOut;
  status = NiFpga_WriteI16(session,
                           NiFpga_exp_test_FIC_ControlI16_ALPHA16bitfirst, tmp);
}
void Ni5772Digitizer::Alpha16BitSecond_Writer(const timespec& timestamp,
                                              const std::int32_t& pOut) {
  std::int16_t tmp;
  tmp = pOut;
  status = NiFpga_WriteI16(
      session, NiFpga_exp_test_FIC_ControlI16_ALPHA16bitsecond, tmp);
}

void Ni5772Digitizer::Threshold_Writer(const timespec& timestamp,
                                       const std::int32_t& pOut) {
  status =
      NiFpga_WriteI16(session, NiFpga_exp_test_FIC_ControlI16_Threshold, pOut);
}

// U16
void Ni5772Digitizer::FilterType_Writer(const timespec& timestamp,
                                        const std::int32_t& pOut) {
  status =
      NiFpga_WriteU16(session, NiFpga_exp_test_FIC_ControlU16_Filtertype, pOut);
}
// U32
void Ni5772Digitizer::MaxTicksPerPulse_Writer(const timespec& timestamp,
                                              const std::int64_t& pOut) {
  status = NiFpga_WriteU32(
      session, NiFpga_exp_test_FIC_ControlU32_maxticksperpulse, pOut);
}
void Ni5772Digitizer::MinTicksPerPulse_Writer(const timespec& timestamp,
                                              const std::int64_t& pOut) {
  status = NiFpga_WriteU32(
      session, NiFpga_exp_test_FIC_ControlU32_minticksperpulse, pOut);
}
void Ni5772Digitizer::MinTicksPerPulse2_Writer(const timespec& timestamp,
                                               const std::int64_t& pOut) {
  status = NiFpga_WriteU32(
      session, NiFpga_exp_test_FIC_ControlU32_minticksperpulse2, pOut);
}

void Ni5772Digitizer::CalibrationCoefficient_Writer(const timespec& timestamp,
                                                    const double& pOut) {
  int64_t calibration_coefficient = convertDoubleToInt64(pOut);
  status = NiFpga_WriteI64(
      session, NiFpga_exp_test_FIC_ControlI64_CalibratedcoefficientINT64,
      calibration_coefficient);
}


// Proxy functions
void Ni5772Digitizer::NeutronProfile_Reader(const timespec* timestamp,
                                            std::vector<std::int32_t>* pIn) {

  // This is the proxy NeutronProfile Reader for HMI purposes
  pIn->clear();
  for (int i = 0; i < 100; ++i) {
    int x = i % 10;  // Column index
    int y = i / 10;  // Row index

    if ((x - 4) * (x - 4) + (y - 4) * (y - 4) < 9) {
      int random = std::rand() % 230 + 210;
      pIn->push_back(random);
    } else {
      int randomValue = std::rand() % 40 + 1;
      pIn->push_back(randomValue);
    }
  }
}

void Ni5772Digitizer::CountRateAverage_Reader(const timespec* timestamp,
                                              double* pIn) {
  *pIn = CountRate_Average;
}

void Ni5772Digitizer::MSV_Reader(const timespec* timestamp, double* pIn) {
  *pIn = MSV;
}

NDS_DEFINE_DRIVER(Ni5772Digitizer, Ni5772Digitizer)
