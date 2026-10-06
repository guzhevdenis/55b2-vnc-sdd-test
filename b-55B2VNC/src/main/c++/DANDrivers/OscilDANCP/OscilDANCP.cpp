/**
 * @file OscilDANCP.cpp
 *
 * @author Javier Moreno (GMV)
 * @date 10 Sep 2018
 */

#include <tcn.h>
#include <math.h>

#include "include/OscilDANCP.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */
#define S2NS 1000000000 //Seconds to nanoSeconds

OscilDANCP::OscilDANCP (nds::Factory& factory,
                                const std::string& deviceName,
                                const nds::namedParameters_t& namedParameters) :
      m_deviceName(deviceName),
      m_samplesCounter(0),
      m_danBuffer_Ch0(NULL),
      m_danBuffer_Ch1(NULL),
      rootNode(deviceName),
      m_data_PV_Ch0("buffer_Ch0",
                  std::bind(&OscilDANCP::PV_buffer_writer_Ch0, this, std::placeholders::_1, std::placeholders::_2)),
      m_data_PV_Ch1("buffer_Ch1",
                  std::bind(&OscilDANCP::PV_buffer_writer_Ch1, this, std::placeholders::_1, std::placeholders::_2)),
      m_MaxChannels_PV_Ch0(nds::PVDelegateOut<std::int32_t>("MaxChannels_Ch0",
                  std::bind(&OscilDANCP::setMaxChannels_Ch0, this, std::placeholders::_1, std::placeholders::_2))),
      m_MaxChannels_PV_Ch1(nds::PVDelegateOut<std::int32_t>("MaxChannels_Ch1",
                  std::bind(&OscilDANCP::setMaxChannels_Ch1, this, std::placeholders::_1, std::placeholders::_2)))
{

  m_stateMachine = rootNode.addChild(nds::StateMachine(true,
                  std::bind(&OscilDANCP::switchOn_Device, this),
                  std::bind(&OscilDANCP::switchOff_Device, this),
                  std::bind(&OscilDANCP::start_Device, this),
                  std::bind(&OscilDANCP::stop_Device, this),
                  std::bind(&OscilDANCP::recover_Device, this),
                  std::bind(&OscilDANCP::allowChange_Device, this,
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
 
  nds::autoEnable_t autoEnable = nds::autoEnable_t::running;

  m_DANCP_Ch0 = rootNode.addChild(ndsdan::DANCP("DAN_Ch0",
                  std::bind(&OscilDANCP::dancp_initData_Ch0, this, std::placeholders::_1),
                  std::bind(&OscilDANCP::dancp_releaseData_Ch0, this, std::placeholders::_1),
                  std::bind(&OscilDANCP::dancp_setHeader_Ch0, this, std::placeholders::_1), autoEnable));

  m_DANCP_Ch1 = rootNode.addChild(ndsdan::DANCP("DAN_Ch1",
                  std::bind(&OscilDANCP::dancp_initData_Ch1, this, std::placeholders::_1),
                  std::bind(&OscilDANCP::dancp_releaseData_Ch1, this, std::placeholders::_1),
                  std::bind(&OscilDANCP::dancp_setHeader_Ch1, this, std::placeholders::_1), autoEnable));

  m_MaxChannels_RBVPV_Ch0 = nds::PVVariableIn<std::int32_t>("MaxChannels_RBV_Ch0");
  m_MaxChannels_RBVPV_Ch0.setValue(static_cast<int32_t>(0));

  m_MaxChannels_RBVPV_Ch1 = nds::PVVariableIn<std::int32_t>("MaxChannels_RBV_Ch1");
  m_MaxChannels_RBVPV_Ch1.setValue(static_cast<int32_t>(0));

  rootNode.addChild(m_data_PV_Ch0);
  rootNode.addChild(m_MaxChannels_PV_Ch0);
  rootNode.addChild(m_MaxChannels_RBVPV_Ch0);

  rootNode.addChild(m_data_PV_Ch1);
  rootNode.addChild(m_MaxChannels_PV_Ch1);
  rootNode.addChild(m_MaxChannels_RBVPV_Ch1);

  rootNode.initialize(this, factory);

  rootNode.setLogLevel(nds::logLevel_t::debug);

  nds::namedParameters_t::const_iterator findParam =  namedParameters.find("INIT");
  if (findParam != namedParameters.end() && findParam->second=="YES") {
    initializePVs();
  }
}



OscilDANCP::~OscilDANCP ()
{
}


#ifdef EPICS

  NDS_DEFINE_DRIVER(OscilDANCP, OscilDANCP)

#else


  void*
  OscilDANCP::allocateDevice (nds::Factory& factory,
                                  const std::string& deviceName,
                                  const nds::namedParameters_t& namedParameters)
  {
    return new OscilDANCP(factory, deviceName, namedParameters);
  }


  void
  OscilDANCP::deallocateDevice (void* deviceName)
  {
    delete static_cast<OscilDANCP*>(deviceName);
  }

#endif

void
OscilDANCP::setMaxChannels_Ch0(const timespec& timestamp,
                                const std::int32_t& maxNumberOfChannels)
{
  nds::state_t currentState = m_stateMachine.getLocalState();
  if (currentState == nds::state_t::off) {
      m_MaxChannels_RBVPV_Ch0.setValue(rootNode.getTimestamp(), maxNumberOfChannels);
      m_MaxChannels_RBVPV_Ch0.push(rootNode.getTimestamp(), maxNumberOfChannels);
  } else {
      ndsErrorStream(m_MaxChannels_RBVPV_Ch0) << "The maximum number of channels can only be modified in OFF state." << std::endl;
  }
}

void
OscilDANCP::setMaxChannels_Ch1(const timespec& timestamp,
                                const std::int32_t& maxNumberOfChannels)
{
  nds::state_t currentState = m_stateMachine.getLocalState();
  if (currentState == nds::state_t::off) {
      m_MaxChannels_RBVPV_Ch1.setValue(rootNode.getTimestamp(), maxNumberOfChannels);
      m_MaxChannels_RBVPV_Ch1.push(rootNode.getTimestamp(), maxNumberOfChannels);
  } else {
      ndsErrorStream(m_MaxChannels_RBVPV_Ch1) << "The maximum number of channels can only be modified in OFF state." << std::endl;
  }
}

void
OscilDANCP::switchOn_Device (void)
{
  ndsDebugStream(rootNode) << (m_deviceName + "-STM: Switching On" ) << std::endl;
}

void
OscilDANCP::switchOff_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Switching Off" ) << std::endl;
}

void
OscilDANCP::start_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Starting" ) << std::endl;
}

void
OscilDANCP::stop_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Stopping" ) << std::endl;
  ndsDebugStream(m_stateMachine) << "DAN-DAQ successfully stopped." << std::endl;
}

void
OscilDANCP::recover_Device (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Recovering" ) << std::endl;
}

bool
OscilDANCP::allowChange_Device(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState)
{
  if (currentState == nds::state_t::on && desiredState == nds::state_t::running && m_danBuffer_Ch0 == NULL && m_danBuffer_Ch1 == NULL) {
      ndsErrorStream(m_stateMachine) << (m_deviceName + "-STM: State change not allowed until switching on the DANCP' state machine" ) << std::endl;
      return false;
  } else {
      ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Allowing change" ) << std::endl;
      return true;
  }
}


void*
OscilDANCP::dancp_initData_Ch0 (ndsdan::dataDAN_t* dataDAN)
{
  std::int32_t bufferSize = m_DANCP_Ch0.getBufferSize();
  if (bufferSize > 0) {
    m_danBuffer_Ch0 = (int32_t *) malloc(bufferSize);
    memset(m_danBuffer_Ch0, 0, bufferSize);
    return m_danBuffer_Ch0;
  } else {
      return NULL;
  }
}

void*
OscilDANCP::dancp_initData_Ch1 (ndsdan::dataDAN_t* dataDAN)
{
  std::int32_t bufferSize = m_DANCP_Ch1.getBufferSize();
  if (bufferSize > 0) {
    m_danBuffer_Ch1 = (int32_t *) malloc(bufferSize);
    memset(m_danBuffer_Ch1, 0, bufferSize);
    return m_danBuffer_Ch1;
  } else {
      return NULL;
  }
}

void OscilDANCP::PV_buffer_writer_Ch0(const timespec& timestamp, const std::vector<std::int32_t>& value){

  std::int32_t blockSize = m_DANCP_Ch0.getBlockNumberOfSamples();
  std::int32_t nChannels = m_DANCP_Ch0.getNumberOfChannels();
  std::int32_t sampleSize = m_DANCP_Ch0.getSampleSize();
  std::int32_t blockSizeInBytes = blockSize*nChannels*sampleSize;
  std::int32_t vectorSize = value.size() * 4; //Received vector size in bytes
  timespec actualTime;
  std::uint64_t currentTimeNs;
  if(vectorSize==blockSizeInBytes){
          memcpy(m_danBuffer_Ch0, value.data(), vectorSize);
          actualTime = m_DANCP_Ch0.getTimestamp();
          currentTimeNs = actualTime.tv_sec*S2NS + actualTime.tv_nsec;
          m_DANCP_Ch0.putDataBlock(currentTimeNs);
          usleep(50000);
  }else{
          ndsErrorStream(m_DANCP_Ch0) << "DAN buffer size is different to received vector." << std::endl;
  }

}

void OscilDANCP::PV_buffer_writer_Ch1(const timespec& timestamp, const std::vector<std::int32_t>& value){

  std::int32_t blockSize = m_DANCP_Ch1.getBlockNumberOfSamples();
  std::int32_t nChannels = m_DANCP_Ch1.getNumberOfChannels();
  std::int32_t sampleSize = m_DANCP_Ch1.getSampleSize();
  std::int32_t blockSizeInBytes = blockSize*nChannels*sampleSize;
  std::int32_t vectorSize = value.size() * 4; //Received vector size in bytes
  timespec actualTime;
  std::uint64_t currentTimeNs;
  if(vectorSize==blockSizeInBytes){
          memcpy(m_danBuffer_Ch1, value.data(), vectorSize);
          actualTime = m_DANCP_Ch1.getTimestamp();
          currentTimeNs = actualTime.tv_sec*S2NS + actualTime.tv_nsec;
          m_DANCP_Ch1.putDataBlock(currentTimeNs);
          usleep(50000);
  }else{
          ndsErrorStream(m_DANCP_Ch1) << "DAN buffer size is different to received vector." << std::endl;
  }

}

void*
OscilDANCP::dancp_releaseData_Ch0(void* dataStructure)
{
  free(dataStructure);
  return NULL;
}

void*
OscilDANCP::dancp_releaseData_Ch1(void* dataStructure)
{
  free(dataStructure);
  return NULL;
}

char*
OscilDANCP::dancp_setHeader_Ch0(const ndsdan::dataDAN_t& dataDAN)
{
  return NULL;
}

char*
OscilDANCP::dancp_setHeader_Ch1(const ndsdan::dataDAN_t& dataDAN)
{
  return NULL;
}

void
OscilDANCP::initializePVs(void)
{
  timespec timestamp = {0, 0};

  std::string initSourceName_Ch0("OscilSourceName_Ch0");
  std::string initICProgramName_Ch0("nds-dan-oscil_Ch0");
  std::string initSourceName_Ch1("OscilSourceName_Ch1");
  std::string initICProgramName_Ch1("nds-dan-oscil_Ch1");
  std::int32_t initSamplingRate(80000000);

  //the size of the int32_t in Bytes
  std::int32_t initSampleSize(4);

  std::int32_t initBlockNumberOfSamples(800768);


  std::uint64_t initTimeBetweenBlocks(10);
  std::uint64_t initMaxAcquisitionTime(80000000);
  std::int32_t initNumberOfBlocks(20480);
  std::int32_t initMaxChannels(1);
  std::int32_t initBufferSize(initBlockNumberOfSamples*initMaxChannels*initSampleSize);

  std::string initPathXML_Ch0("../../../c++/DANDrivers/OscilDANCP/conf/oscilDanConfig_Ch0.xml");
  std::string initPathXML_Ch1("../../../c++/DANDrivers/OscilDANCP/conf/oscilDanConfig_Ch1.xml");

  m_DANCP_Ch0.setSourceName(timestamp, initSourceName_Ch0);
  m_DANCP_Ch0.setICProgramName(timestamp, initICProgramName_Ch0);
  m_DANCP_Ch0.setSamplingRate(timestamp, initSamplingRate);
  m_DANCP_Ch0.setBufferSize(timestamp, initBufferSize);
  m_DANCP_Ch0.setSampleSize(timestamp, initSampleSize);
  m_DANCP_Ch0.setBlockNumberOfSamples(timestamp, initBlockNumberOfSamples);

  m_DANCP_Ch0.setTimeBetweenBlocks(timestamp, static_cast<double>(initTimeBetweenBlocks));
  m_DANCP_Ch0.setMaxAcquisitionTime(timestamp, static_cast<double>(initMaxAcquisitionTime));
  m_DANCP_Ch0.setNumberOfBlocks(timestamp, initNumberOfBlocks);

  m_DANCP_Ch0.setPathXML(timestamp, initPathXML_Ch0);

  setMaxChannels_Ch0(timestamp, initMaxChannels);

  m_DANCP_Ch1.setSourceName(timestamp, initSourceName_Ch1);
  m_DANCP_Ch1.setICProgramName(timestamp, initICProgramName_Ch1);
  m_DANCP_Ch1.setSamplingRate(timestamp, initSamplingRate);
  m_DANCP_Ch1.setBufferSize(timestamp, initBufferSize);
  m_DANCP_Ch1.setSampleSize(timestamp, initSampleSize);
  m_DANCP_Ch1.setBlockNumberOfSamples(timestamp, initBlockNumberOfSamples);

  m_DANCP_Ch1.setTimeBetweenBlocks(timestamp, static_cast<double>(initTimeBetweenBlocks));
  m_DANCP_Ch1.setMaxAcquisitionTime(timestamp, static_cast<double>(initMaxAcquisitionTime));
  m_DANCP_Ch1.setNumberOfBlocks(timestamp, initNumberOfBlocks);

  m_DANCP_Ch1.setPathXML(timestamp, initPathXML_Ch1);

  setMaxChannels_Ch1(timestamp, initMaxChannels);
}