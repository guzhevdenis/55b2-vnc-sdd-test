/**
 * @file Ni5763DANCP.cpp
 *
 * @author Javier Moreno (GMV)
 * @date 10 Sep 2018
 */

#include <tcn.h>
#include <math.h>

#include "include/Ni5763DANCP.h"
#include <iostream>

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */
#define S2NS 1000000000 //Seconds to nanoSeconds

Ni5763DANCP::Ni5763DANCP (nds::Factory& factory,
                                const std::string& deviceName,
                                const nds::namedParameters_t& namedParameters) :
      m_deviceName(deviceName),
      m_samplesCounter(0),
      m_danBuffer(NULL),
      rootNode(deviceName),
      m_data_PV("buffer",
                  std::bind(&Ni5763DANCP::PV_buffer_writer, this, std::placeholders::_1, std::placeholders::_2)),
      m_MaxChannels_PV(nds::PVDelegateOut<std::int32_t>("MaxChannels",
                  std::bind(&Ni5763DANCP::setMaxChannels, this, std::placeholders::_1, std::placeholders::_2)))
{

  m_stateMachine = rootNode.addChild(nds::StateMachine(true,
                  std::bind(&Ni5763DANCP::switchOn_Device, this),
                  std::bind(&Ni5763DANCP::switchOff_Device, this),
                  std::bind(&Ni5763DANCP::start_Device, this),
                  std::bind(&Ni5763DANCP::stop_Device, this),
                  std::bind(&Ni5763DANCP::recover_Device, this),
                  std::bind(&Ni5763DANCP::allowChange_Device, this,
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
 
  nds::autoEnable_t autoEnable = nds::autoEnable_t::running;

  m_DANCP = rootNode.addChild(ndsdan::DANCP("DAN_Ni5763",
                  std::bind(&Ni5763DANCP::dancp_initData, this, std::placeholders::_1),
                  std::bind(&Ni5763DANCP::dancp_releaseData, this, std::placeholders::_1),
                  std::bind(&Ni5763DANCP::dancp_setHeader, this, std::placeholders::_1), autoEnable));

  m_MaxChannels_RBVPV = nds::PVVariableIn<std::int32_t>("MaxChannels_RBV");
  m_MaxChannels_RBVPV.setValue(static_cast<int32_t>(0));

  rootNode.addChild(m_data_PV);
  rootNode.addChild(m_MaxChannels_PV);
  rootNode.addChild(m_MaxChannels_RBVPV);

  rootNode.initialize(this, factory);

  rootNode.setLogLevel(nds::logLevel_t::debug);

  nds::namedParameters_t::const_iterator findParam =  namedParameters.find("INIT");
  if (findParam != namedParameters.end() && findParam->second=="YES") {
    initializePVs();
  }
  m_DAN_Running = false;
}

Ni5763DANCP::~Ni5763DANCP ()
{
}

#ifdef EPICS

  NDS_DEFINE_DRIVER(Ni5763DANCP, Ni5763DANCP)

#else

  void*
  Ni5763DANCP::allocateDevice (nds::Factory& factory,
                                  const std::string& deviceName,
                                  const nds::namedParameters_t& namedParameters)
  {
    return new Ni5763DANCP(factory, deviceName, namedParameters);
  }

  void
  Ni5763DANCP::deallocateDevice (void* deviceName)
  {
    delete static_cast<Ni5763DANCP*>(deviceName);
  }

#endif

void
Ni5763DANCP::setMaxChannels(const timespec& timestamp,
                                const std::int32_t& maxNumberOfChannels)
{
  nds::state_t currentState = m_stateMachine.getLocalState();
  if (currentState == nds::state_t::off) {
      m_MaxChannels_RBVPV.setValue(rootNode.getTimestamp(), maxNumberOfChannels);
      m_MaxChannels_RBVPV.push(rootNode.getTimestamp(), maxNumberOfChannels);
  } else {
      ndsErrorStream(m_MaxChannels_RBVPV) << "The maximum number of channels can only be modified in OFF state." << std::endl;
  }
}

void
Ni5763DANCP::switchOn_Device (void)
{
  ndsDebugStream(rootNode) << (m_deviceName + "-STM: Switching On" ) << std::endl;
}

void
Ni5763DANCP::switchOff_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Switching Off" ) << std::endl;
}

void
Ni5763DANCP::start_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Starting" ) << std::endl;
  m_DAN_Running = true;
}

void
Ni5763DANCP::stop_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Stopping" ) << std::endl;
  ndsDebugStream(m_stateMachine) << "DAN-DAQ successfully stopped." << std::endl;
  m_DAN_Running = false;
}

void
Ni5763DANCP::recover_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Recovering" ) << std::endl;
}

bool
Ni5763DANCP::allowChange_Device(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState)
{
  if (currentState == nds::state_t::on && desiredState == nds::state_t::running && m_danBuffer == NULL) {
      ndsErrorStream(m_stateMachine) << (m_deviceName + "-STM: State change not allowed until switching on the DANCP' state machine" ) << std::endl;
      return false;
  } else {
      ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Allowing change" ) << std::endl;
      return true;
  }
}


void*
Ni5763DANCP::dancp_initData (ndsdan::dataDAN_t* dataDAN)
{
  std::int32_t bufferSize = m_DANCP.getBufferSize();
  if (bufferSize > 0) {
    m_danBuffer = (int32_t *) malloc(bufferSize);
    memset(m_danBuffer, 0, bufferSize);
    return m_danBuffer;
  } else {
      return NULL;
  }
}

void Ni5763DANCP::PV_buffer_writer(const timespec& timestamp, const std::vector<std::int32_t>& value){
  if(!m_DAN_Running) {
    return;
  }
  std::int32_t blockSize = m_DANCP.getBlockNumberOfSamples();
  std::int32_t nChannels = m_DANCP.getNumberOfChannels();
  std::int32_t sampleSize = m_DANCP.getSampleSize();
  std::int32_t blockSizeInBytes = blockSize*nChannels*sampleSize;
  std::int32_t vectorSize = value.size() * 4; //Received vector size in bytes
  timespec actualTime;
  std::uint64_t currentTimeNs;
  if(vectorSize==blockSizeInBytes){
          memcpy(m_danBuffer, value.data(), vectorSize);
          actualTime = m_DANCP.getTimestamp();
          currentTimeNs = actualTime.tv_sec*S2NS + actualTime.tv_nsec;
          m_DANCP.putDataBlock(currentTimeNs);
          usleep(50000);
  }else{
          ndsErrorStream(m_DANCP) << "DAN buffer size is different to received vector." << std::endl;
  }

}

void*
Ni5763DANCP::dancp_releaseData(void* dataStructure)
{
  free(dataStructure);
  return NULL;
}

char*
Ni5763DANCP::dancp_setHeader(const ndsdan::dataDAN_t& dataDAN)
{
  return NULL;
}

void
Ni5763DANCP::initializePVs(void)
{
  timespec timestamp = {0, 0};

  std::string initSourceName("Ni5763SourceName");
  std::string initICProgramName("nds-dan-ni5763");
  std::int32_t initSamplingRate(80000000);

  //the size of the int32_t in Bytes
  std::int32_t initSampleSize(4);

  std::int32_t initBlockNumberOfSamples(10240);


  std::uint64_t initTimeBetweenBlocks(10);
  std::uint64_t initMaxAcquisitionTime(80000000);
  std::int32_t initNumberOfBlocks(20480);
  std::int32_t initMaxChannels(1);
  std::int32_t initBufferSize(initBlockNumberOfSamples*initMaxChannels*initSampleSize);

  std::string initPathXML("../../../c++/DANDrivers/Ni5763DANCP/conf/Ni5763DanConfig.xml");

  m_DANCP.setSourceName(timestamp, initSourceName);
  m_DANCP.setICProgramName(timestamp, initICProgramName);
  m_DANCP.setSamplingRate(timestamp, initSamplingRate);
  m_DANCP.setBufferSize(timestamp, initBufferSize);
  m_DANCP.setSampleSize(timestamp, initSampleSize);
  m_DANCP.setBlockNumberOfSamples(timestamp, initBlockNumberOfSamples);

  m_DANCP.setTimeBetweenBlocks(timestamp, static_cast<double>(initTimeBetweenBlocks));
  m_DANCP.setMaxAcquisitionTime(timestamp, static_cast<double>(initMaxAcquisitionTime));
  m_DANCP.setNumberOfBlocks(timestamp, initNumberOfBlocks);

  m_DANCP.setPathXML(timestamp, initPathXML);

  setMaxChannels(timestamp, initMaxChannels);
}