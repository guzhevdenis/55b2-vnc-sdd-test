/*
 * DANImpl.cpp
 *
 *  Created on: 3 Sep 2018
 *      Author: jmoreno
 */

#include "nds3/impl/threadBaseImpl.h"

#include "include/nds-dan/impl/DANImpl.h"

namespace ndsdan
{

    ndsdan::DANImpl::DANImpl (const std::string& name,
                              genericDANFunction_t dataStructureInitializer,
                              genericFunction_t dataStructureReleaser,
                              charDANFunction_t blockHeaderSetter,
							  nds::autoEnable_t autoEnable) :
      NodeImpl(name, nds::nodeType_t::generic),
      m_ICProgramLoaded(false),
      m_DANStreamerOpen(false),
      m_CanPublishDataBlocks(false),
      m_InitialisedTCN(false),
      m_DataDAN(),
      m_InitializeDataStructure(dataStructureInitializer),
      m_ReleaseDataStructure(dataStructureReleaser),
      m_SetBlockHeader(blockHeaderSetter)
    {

      m_StateMachine.reset(new nds::StateMachineImpl(false, //Blocking transitions are always set.
                                                std::bind(&DANImpl::STMswitchOn, this),
                                                std::bind(&DANImpl::STMswitchOff, this),
                                                std::bind(&DANImpl::STMstart, this),
                                                std::bind(&DANImpl::STMstop, this),
                                                std::bind(&DANImpl::STMrecover, this),
                                                std::bind(&DANImpl::STMallowStateChange, this,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2,
                                                          std::placeholders::_3),
												autoEnable));
      addChild(m_StateMachine);

      m_Status_PV.reset(new nds::PVVariableInImpl<std::int32_t>("Status"));
      m_Status_PV->setDescription("DAN status");
      m_Status_PV->setScanType(nds::scanType_t::interrupt, 0);
      //TODO: Revise this. Due to statusDAN being longer than 16 elements it causes an array access out of its bounds in nds-epics.
//      nds::enumerationStrings_t enumerationStrings;
//      for(std::uint8_t index(0); index <= static_cast<uint8_t>(statusDAN_t::ERROR_UNKNOWN); ++index)
//      {
//          std::string text = parseStatus(static_cast<statusDAN_t>(index));
//          enumerationStrings.push_back(text);
//      }
//      m_Status_PV->setEnumeration(enumerationStrings);
      m_Status_PV->setValue(static_cast<int32_t>(statusDAN_t::UNKNOWN));
      addChild(m_Status_PV);

      m_StatusStream_PV.reset(new nds::PVVariableInImpl<std::int32_t>("StatusStream"));
      m_StatusStream_PV->setDescription("DAN Stream status");
      m_StatusStream_PV->setScanType(nds::scanType_t::interrupt, 0);
      nds::enumerationStrings_t enumerationStringsStream;
      for(std::uint8_t index(0); index <= static_cast<uint8_t>(statusStreamDAN_t::ERROR_UNKNOWN); ++index)
      {
          std::string text = parseStatusStream(static_cast<statusStreamDAN_t>(index));
          enumerationStringsStream.push_back(text);
      }
      m_StatusStream_PV->setEnumeration(enumerationStringsStream);
      m_StatusStream_PV->setValue(static_cast<int32_t>(statusStreamDAN_t::UNKNOWN));
      addChild(m_StatusStream_PV);

      m_SourceName_PV.reset(new nds::PVDelegateOutImpl<std::string>("SourceName",
                                   std::bind(&DANImpl::setSourceName, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_SourceName_PV->setDescription("Set the name of DAN source");
      m_SourceName_PV->setMaxElements(256);
      addChild(m_SourceName_PV);

      m_SourceName_RBVPV.reset(new nds::PVDelegateInImpl<std::string>("SourceName_RBV",
                                   std::bind(&DANImpl::readSourceName, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_SourceName_RBVPV->setDescription("Readback value of DAN source name");
      m_SourceName_RBVPV->setMaxElements(256);
      m_SourceName_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_SourceName_RBVPV);

      m_ICProgramName_PV.reset(new nds::PVDelegateOutImpl<std::string>("ICProgramName",
                                   std::bind(&DANImpl::setICProgramName, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_ICProgramName_PV->setDescription("Set the name of DAN ICProgram");
      m_ICProgramName_PV->setMaxElements(256);
      addChild(m_ICProgramName_PV);

      m_ICProgramName_RBVPV.reset(new nds::PVDelegateInImpl<std::string>("ICProgramName_RBV",
                                   std::bind(&DANImpl::readICProgramName, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_ICProgramName_RBVPV->setDescription("Readback value of DAN ICProgram name");
      m_ICProgramName_RBVPV->setMaxElements(256);
      m_ICProgramName_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_ICProgramName_RBVPV);


      m_SamplingRate_PV.reset(new nds::PVDelegateOutImpl<std::int32_t>("SamplingRate",
                                   std::bind(&DANImpl::setSamplingRate, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_SamplingRate_PV->setDescription("Set the DAN sampling rate");
      m_SamplingRate_PV->setUnits("S/s");
      addChild(m_SamplingRate_PV);

      m_SamplingRate_RBVPV.reset(new nds::PVDelegateInImpl<std::int32_t>("SamplingRate_RBV",
                                   std::bind(&DANImpl::readSamplingRate, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_SamplingRate_RBVPV->setDescription("Readback value of DAN sampling rate");
      m_SamplingRate_PV->setUnits("S/s");
      m_SamplingRate_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_SamplingRate_RBVPV);

      m_BufferSize_PV.reset(new nds::PVDelegateOutImpl<std::int32_t>("BufferSize",
                                   std::bind(&DANImpl::setBufferSize, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_BufferSize_PV->setDescription("Set the DAN buffer size");
      m_BufferSize_PV->setUnits("Bytes");
      addChild(m_BufferSize_PV);

      m_BufferSize_RBVPV.reset(new nds::PVDelegateInImpl<std::int32_t>("BufferSize_RBV",
                                   std::bind(&DANImpl::readBufferSize, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_BufferSize_RBVPV->setDescription("Readback value of DAN buffer size");
      m_BufferSize_RBVPV->setUnits("Bytes");
      m_BufferSize_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_BufferSize_RBVPV);

      m_SampleSize_PV.reset(new nds::PVDelegateOutImpl<std::int32_t>("SampleSize",
                                   std::bind(&DANImpl::setSampleSize, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_SampleSize_PV->setDescription("Set the sample size of DAN publisher");
      m_SampleSize_PV->setUnits("Bytes");
      addChild(m_SampleSize_PV);

      m_SampleSize_RBVPV.reset(new nds::PVDelegateInImpl<std::int32_t>("SampleSize_RBV",
                                   std::bind(&DANImpl::readSampleSize, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_SampleSize_RBVPV->setDescription("Readback value of DAN sample size");
      m_SampleSize_RBVPV->setUnits("Bytes");
      m_SampleSize_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_SampleSize_RBVPV);

      m_BlockNumberOfSamples_PV.reset(new nds::PVDelegateOutImpl<std::int32_t>("BlockNumberOfSamples",
                                   std::bind(&DANImpl::setBlockNumberOfSamples, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_BlockNumberOfSamples_PV->setDescription("Set the DAN block size");
      m_BlockNumberOfSamples_PV->setUnits("Samples");
      addChild(m_BlockNumberOfSamples_PV);

      m_BlockNumberOfSamples_RBVPV.reset(new nds::PVDelegateInImpl<std::int32_t>("BlockNumberOfSamples_RBV",
                                   std::bind(&DANImpl::readBlockNumberOfSamples, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_BlockNumberOfSamples_RBVPV->setDescription("Readback value of DAN block size");
      m_BlockNumberOfSamples_RBVPV->setUnits("Samples");
      m_BlockNumberOfSamples_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_BlockNumberOfSamples_RBVPV);

      m_NumberOfChannels_PV.reset(new nds::PVDelegateInImpl<std::int32_t>("NumberOfChannels",
                                   std::bind(&DANImpl::readNumberOfChannels, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_NumberOfChannels_PV->setDescription("Number of channels in DAN streamer");
      m_NumberOfChannels_PV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_NumberOfChannels_PV);

      m_ArrayOfChannels_PV.reset(new nds::PVVariableInImpl<std::vector<std::int32_t>>("ArrayOfChannels"));
      m_ArrayOfChannels_PV->setDescription("Array of channels in DAN streamer");
      m_ArrayOfChannels_PV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_ArrayOfChannels_PV);

      m_NumberOfEnabledChannels_PV.reset(new nds::PVDelegateInImpl<std::int32_t>("NumberOfEnabledChannels",
										  std::bind(&DANImpl::readNumberOfEnabledChannels, this,
													std::placeholders::_1,
													std::placeholders::_2)));
      m_NumberOfEnabledChannels_PV->setDescription("Number of enabled channels in DAN streamer");
      m_NumberOfEnabledChannels_PV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_NumberOfEnabledChannels_PV);

      m_ArrayOfEnabledChannels_PV.reset(new nds::PVDelegateInImpl<std::vector<std::int32_t>>("ArrayOfEnabledChannels",
										  std::bind(&DANImpl::readArrayOfEnabledChannels, this,
													std::placeholders::_1,
													std::placeholders::_2)));
      m_ArrayOfEnabledChannels_PV->setDescription("Number of enabled channels in DAN streamer");
      m_ArrayOfEnabledChannels_PV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_ArrayOfEnabledChannels_PV);

      m_PathXML_PV.reset(new nds::PVDelegateOutImpl<std::string>("PathXML",
                                   std::bind(&DANImpl::setPathXML, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_PathXML_PV->setDescription("Set the path of DAN XML file");
      m_PathXML_PV->setMaxElements(256);
      addChild(m_PathXML_PV);

      m_PathXML_RBVPV.reset(new nds::PVDelegateInImpl<std::string>("PathXML_RBV",
                                   std::bind(&DANImpl::readPathXML, this,
                                             std::placeholders::_1,
                                             std::placeholders::_2)));
      m_PathXML_RBVPV->setDescription("Readback value of DAN XML file");
      m_PathXML_RBVPV->setMaxElements(256);
      m_PathXML_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
      addChild(m_PathXML_RBVPV);

    }

    ndsdan::DANImpl::~DANImpl ()
    {
      std::string msg = "Destructor... Start";
      ndsDebugStream(*this) << msg << std::endl << std::flush;
//      ndsDANLogDebug(*this, msg.c_str());
      cleanUpDANResources();
      msg = "Destructor... End";
      ndsDebugStream(*this) << msg << std::endl << std::flush;
//      ndsDANLogDebug(*this, msg.c_str());
    }

    std::string
    ndsdan::DANImpl::getSourceName ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.sourceName;
    }

    void
    ndsdan::DANImpl::readSourceName(timespec* timestamp, std::string* sourceName)
    {
      *sourceName = getSourceName();
      *timestamp = getTimestamp();
    }

    std::string
    ndsdan::DANImpl::getICProgramName ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.ICProgramName;
    }

    void
    ndsdan::DANImpl::readICProgramName(timespec* timestamp, std::string* appName)
    {
      *appName = getICProgramName();
      *timestamp = getTimestamp();
    }

    std::int32_t
    ndsdan::DANImpl::getSamplingRate ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.samplingRate;
    }

    void
    ndsdan::DANImpl::readSamplingRate(timespec* timestamp, std::int32_t* samplingRate)
    {
      *samplingRate = getSamplingRate();
      *timestamp = getTimestamp();
    }

    std::int32_t
    ndsdan::DANImpl::getBufferSize ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.bufferSize;
    }

    void
    ndsdan::DANImpl::readBufferSize(timespec* timestamp, std::int32_t* bufferSize)
    {
      *bufferSize = getBufferSize();
      *timestamp = getTimestamp();
    }

    std::int32_t
    ndsdan::DANImpl::getNumberOfChannels ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.numberOfChannels;
    }

    void
    ndsdan::DANImpl::readNumberOfChannels(timespec* timestamp, std::int32_t* numberOfChannels)
    {
      *numberOfChannels = getNumberOfChannels();
      *timestamp = getTimestamp();
    }

    std::vector<std::int32_t>
    ndsdan::DANImpl::getArrayOfChannels()
    {
		std::unique_ptr<std::int32_t[]> ptrList(dan_publisher_getArrayOfChannels(m_DataDAN.dataSource, &m_DataDAN.numberOfChannels)); //The returned pointer must be freed, so it is delegated to the smart pointer
		std::vector<std::int32_t> vecChannels(ptrList.get(), ptrList.get()+m_DataDAN.numberOfChannels);
		return vecChannels;
     }

    std::int32_t
    ndsdan::DANImpl::getNumberOfEnabledChannels ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      m_DataDAN.numberOfEnabledChannels = dan_publisher_getNumberOfEnabledChannels(m_DataDAN.dataSource);
      return m_DataDAN.numberOfEnabledChannels;
    }

    void
	ndsdan::DANImpl::readNumberOfEnabledChannels(timespec* timestamp, std::int32_t* numberOfEnaChannels){
        *numberOfEnaChannels = getNumberOfEnabledChannels();
    	*timestamp = getTimestamp();
    }

    std::vector<std::int32_t>
    ndsdan::DANImpl::getArrayOfEnabledChannels(){
		std::int32_t&& numCh = getNumberOfEnabledChannels();
		std::unique_ptr<std::int32_t[]> ptrList(dan_publisher_getArrayOfEnabledChannels(m_DataDAN.dataSource, &numCh)); //The returned pointer must be freed, so it is delegated to the smart pointer
		std::vector<std::int32_t> vecChannels(ptrList.get(), ptrList.get()+numCh);
		return vecChannels;
    }

    void
	ndsdan::DANImpl::readArrayOfEnabledChannels(timespec* timestamp, std::vector<std::int32_t>* arrayOfEnaChannels){
    	std::vector<std::int32_t>&& vec = getArrayOfEnabledChannels();
    	arrayOfEnaChannels->swap(vec);
    	*timestamp = getTimestamp();
    }

    std::int32_t
    ndsdan::DANImpl::getSampleSize ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.sampleSize;
    }

    void
    ndsdan::DANImpl::readSampleSize(timespec* timestamp, std::int32_t* sampleSize)
    {
      *sampleSize = getSampleSize();
      *timestamp = getTimestamp();
    }

    std::int32_t
    ndsdan::DANImpl::getBlockNumberOfSamples ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.blockNumberOfSamples;
    }

    void
    ndsdan::DANImpl::readBlockNumberOfSamples(timespec* timestamp, std::int32_t* blockNumberOfSamples)
    {
      *blockNumberOfSamples = getBlockNumberOfSamples();
      *timestamp = getTimestamp();
    }

    std::string
    ndsdan::DANImpl::getPathXML ()
    {
      std::lock_guard<std::recursive_mutex> lock(m_Mutex);
      return m_DataDAN.pathXML;
    }

    void
    ndsdan::DANImpl::readPathXML(timespec* timestamp, std::string* pathXML)
    {
      *pathXML = getPathXML();
      *timestamp = getTimestamp();
    }

    void
    ndsdan::DANImpl::setSourceName (const timespec& timestamp,
                                       const std::string& sourceName)
    {
      nds::state_t currentState = m_StateMachine->getLocalState();
      if (currentState == nds::state_t::off) {
          std::lock_guard<std::recursive_mutex> lock(m_Mutex);
          m_DataDAN.sourceName = sourceName;
          m_SourceName_RBVPV->push(timestamp, sourceName);
      } else {
          ndsErrorStream(*this) << "The DAN source name can only be modified in OFF state." << std::endl;
      }
    }

    void
    ndsdan::DANImpl::setICProgramName (const timespec& timestamp,
                                       const std::string& appName)
    {
      nds::state_t currentState = m_StateMachine->getLocalState();
      if (currentState == nds::state_t::off) {
          std::lock_guard<std::recursive_mutex> lock(m_Mutex);
          m_DataDAN.ICProgramName = appName;
          m_ICProgramName_RBVPV->push(timestamp, appName);
      } else {
          ndsErrorStream(*this) << "The DAN ICProgram name can only be modified in OFF state." << std::endl;
      }
    }

    void
    ndsdan::DANImpl::setSamplingRate (const timespec& timestamp,
                                         const std::int32_t& samplingRate)
    {
      nds::state_t currentState = m_StateMachine->getLocalState();
      if (currentState == nds::state_t::off) {
          std::lock_guard<std::recursive_mutex> lock(m_Mutex);
          m_DataDAN.samplingRate = samplingRate;
          m_SamplingRate_RBVPV->push(timestamp, samplingRate);
      } else {
          ndsErrorStream(*this) << "The DAN sampling rate can only be modified in OFF state." << std::endl;
      }
    }

    void
    ndsdan::DANImpl::setBufferSize (const timespec& timestamp,
                                       const std::int32_t& bufferSize)
    {
      nds::state_t currentState = m_StateMachine->getLocalState();
      if (currentState == nds::state_t::off || currentState == nds::state_t::initializing ) {
          std::lock_guard<std::recursive_mutex> lock(m_Mutex);
          m_DataDAN.bufferSize = bufferSize;
          m_BufferSize_RBVPV->push(timestamp, bufferSize);
      } else {
          ndsErrorStream(*this) << "The DAN buffer size can only be modified in OFF or INITIALIZING states." << std::endl;
      }
    }

    void
    ndsdan::DANImpl::setSampleSize (const timespec& timestamp,
                                       const std::int32_t& sampleSize)
    {
      nds::state_t currentState = m_StateMachine->getLocalState();
      if (currentState == nds::state_t::off) {
          std::lock_guard<std::recursive_mutex> lock(m_Mutex);
          m_DataDAN.sampleSize = sampleSize;
          m_SampleSize_RBVPV->push(timestamp, sampleSize);
      } else {
          ndsErrorStream(*this) << "The DAN sample size can only be modified in OFF state." << std::endl;
      }
    }

    void
    ndsdan::DANImpl::setBlockNumberOfSamples (const timespec& timestamp,
                                       const std::int32_t& blockNumberOfSamples)
    {
      nds::state_t currentState = m_StateMachine->getLocalState();
      if (currentState == nds::state_t::off) {
          std::lock_guard<std::recursive_mutex> lock(m_Mutex);
          m_DataDAN.blockNumberOfSamples = blockNumberOfSamples;
          m_BlockNumberOfSamples_RBVPV->push(timestamp, blockNumberOfSamples);
      } else {
          ndsErrorStream(*this) << "The DAN block size (in number of samples) can only be modified in OFF state." << std::endl;
      }
    }

    void
    ndsdan::DANImpl::setPathXML (const timespec& timestamp,
                                       const std::string& pathXML)
    {
      nds::state_t currentState = m_StateMachine->getLocalState();
      if (currentState == nds::state_t::off) {
          std::lock_guard<std::recursive_mutex> lock(m_Mutex);
          m_DataDAN.pathXML = pathXML;
          m_PathXML_RBVPV->push(timestamp, pathXML);
      } else {
          ndsErrorStream(*this) << "The DAN XML path can only be modified in OFF state." << std::endl;
      }
    }

    bool
	ndsdan::DANImpl::setUserAttribute(const std::string& name, const std::string& value, const std::string& units, const std::string& description)
	{
    	nds::state_t currentState = m_StateMachine->getLocalState();
        if (currentState == nds::state_t::on || currentState == nds::state_t::running) {
        	if(dan_publisher_setStream_UserAttribute(m_DataDAN.dataSource, name.c_str(), value.c_str(), units.c_str(), description.c_str()) < 0){
        		updateStatus(statusDAN_t::ERROR_SET_ATTRIBUTE);
        		return false;
        	}
        	return true;
        } else {
        	ndsErrorStream(*this) << "User attributes can only be changed in ON or RUNNING state" << std::endl;
        	return false;
        }
	}

    void
	ndsdan::DANImpl::setDataBlockHeaderDim(const std::int32_t& dim1, const std::int32_t& dim2){
    	nds::state_t currentState = m_StateMachine->getLocalState();
		if (currentState == nds::state_t::running) {
			dan_publisher_setDataBlockHeader_dimensions(m_DataDAN.dataSource, dim1, dim2);
		} else {
			ndsErrorStream(*this) << "Block header sample dimensions can only be changed in RUNNING state" << std::endl;
		}
    }

    void
	ndsdan::DANImpl::setDataBlockHeaderSampRate(const double& samplingRate){
    	nds::state_t currentState = m_StateMachine->getLocalState();
		if (currentState == nds::state_t::running) {
			dan_publisher_setDataBlockHeader_samplingRate(m_DataDAN.dataSource, samplingRate);
		} else {
			ndsErrorStream(*this) << "Block header sampling rate can only be changed in RUNNING state" << std::endl;
		}
    }

    void
	ndsdan::DANImpl::setDataBlockHeaderOpMode(const std::int64_t& opMode){
    	nds::state_t currentState = m_StateMachine->getLocalState();
		if (currentState == nds::state_t::running) {
			dan_publisher_setDataBlockHeader_operationalMode(m_DataDAN.dataSource, opMode);
		} else {
			ndsErrorStream(*this) << "Block header operational mode can only be changed in RUNNING state" << std::endl;
		}
    }

    std::int32_t
    ndsdan::DANImpl::isChannelEnable(std::int32_t channelNumber)
    {
    	nds::state_t&& currentState = m_StateMachine->getLocalState();
    	if(currentState == nds::state_t::on || currentState == nds::state_t::running){
    		return dan_publisher_getChannelIsEnable(m_DataDAN.dataSource, channelNumber);
    	}else{
    		ndsErrorStream(*this) << "Array of channels can only be obtained in ON or RUNNING states" << std::endl;
    		return -1;
    	}
    }

    bool
	ndsdan::DANImpl::setChannelEnable(std::int32_t channelNumber)
    {
    	nds::state_t&& currentState = m_StateMachine->getLocalState();
		if (currentState == nds::state_t::on) {
			if(!isChannelEnable(channelNumber)){ //If channel is disabled, enabled it
				if(dan_publisher_setChannelEnable(m_DataDAN.dataSource, channelNumber) < 0){
					updateStatus(statusDAN_t::ERROR_ENABLE_CHANNEL);
					return false;
				}
			}
			return true;
		} else {
			ndsErrorStream(*this) << "Enabling a channel can only be changed in ON state" << std::endl;
			return false;
		}
    }

    bool
	ndsdan::DANImpl::setChannelDisable(std::int32_t channelNumber)
    {
    	nds::state_t&& currentState = m_StateMachine->getLocalState();
		if (currentState == nds::state_t::on) {
			if(isChannelEnable(channelNumber)){ //If channel is disabled, enable it
				if(dan_publisher_setChannelDisable(m_DataDAN.dataSource, channelNumber) < 0){
					updateStatus(statusDAN_t::ERROR_DISABLE_CHANNEL);
					return false;
				}
			}
			return true;
		} else {
			ndsErrorStream(*this) << "Disabling a channel can only be changed in ON state" << std::endl;
			return false;
		}
    }

    void
    ndsdan::DANImpl::STMswitchOn (void)
    {

      std::string msg;
      std::int32_t errorCode;
      statusDAN_t sourceStatus = statusDAN_t::ERROR_UNKNOWN;

      timespec startTime = getTimestamp();
      std::string name = this->getFullExternalName();
      msg = name + ": Switching on... Start";
      ndsDANLogDebug(*this, msg.c_str());

      if (m_DataDAN.pathXML.empty()) {
          msg = "XML file not provided. Command 'danApiTool api init file.xml' shall be run.";
          ndsDANLogWarning(*this, msg.c_str());
      } else {
        msg = "Loading DAN configuration file from '" + m_DataDAN.pathXML + " '...";
        ndsDANLogDebug(*this, msg.c_str());
        errorCode = dan_admin_api_load_fromFile(m_DataDAN.pathXML.c_str());
        if (errorCode < 0) {
          updateStatus(statusDAN_t::ERROR_XML);
          ndsDANLogError(*this, "Error Loading DAN API ICprogram configuration XML file.");
        }
      }

      m_ICProgramLoaded = false;
      m_DataDAN.dataCore = m_DataDAN.ICProgramName.empty() ? dan_initLibrary() : dan_initLibrary_icprog(m_DataDAN.ICProgramName.c_str());
      if (m_DataDAN.dataCore == NULL) {
        timespec stopTime = getTimestamp();
        msg = "Switching on... Broken! [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
        updateStatus(statusDAN_t::ERROR_DAN_INIT);
        msg += "\nError during DAN library initialisation";
        ndsDANLogError(*this, msg.c_str());
        cleanUpDANResources();
        ndsDANLogDebug(*this, msg.c_str());
        throw std::runtime_error(msg);
      } else {
          m_ICProgramLoaded = !m_DataDAN.ICProgramName.empty();
      }
      ndsDANLogInfo(*this, "DAN library successfully initialised.");

//      errorCode = tcn_init();
//      if (errorCode != TCN_SUCCESS) {
//        timespec stopTime = getTimestamp();
//        msg = "Switching on... Broken! [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
//        updateStatus(statusDAN_t::ERROR_TCN_INIT);
//        std::string msg = "Error during TCN library initialisation (Error code: " + std::to_string(errorCode) + ")";
//        ndsDANLogError(*this, msg.c_str());
//        cleanUpDANResources();
//        ndsDANLogDebug(*this, msg.c_str());
//        throw std::runtime_error(msg);
//      } else {
//          m_InitialisedTCN = true;
//      }

      m_DataDAN.dataStructure = m_InitializeDataStructure(&m_DataDAN);
      if (m_DataDAN.dataStructure == NULL) {
          timespec stopTime = getTimestamp();
          msg = "Switching on... Broken! [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
          ndsDANLogDebug(*this, msg.c_str());
          updateStatus(statusDAN_t::ERROR_DATA_INIT);
          msg = "Error initialising DAN data structure ";
          ndsDANLogError(*this, msg.c_str());
          throw std::runtime_error(msg);
      }
      ndsDANLogDebug(*this, "DAN node has been successfully switched on.");

      sourceStatus = publishSource();
      if (sourceStatus !=  statusDAN_t::SUCCESS) {
        timespec stopTime = getTimestamp();
        msg = "Switching on... Broken! [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
        updateStatus(sourceStatus);
        msg += "\nError publishing DAN source " + m_DataDAN.sourceName;
        ndsDANLogError(*this, msg.c_str());
        ndsDANLogDebug(*this, msg.c_str());
        throw std::runtime_error(msg);
      }
      msg = "DAN source " + m_DataDAN.sourceName + " is ready to publish";
      ndsDANLogInfo(*this, msg.c_str());

      m_DataDAN.numberOfChannels = dan_publisher_getNumberOfChannels(m_DataDAN.dataSource);
      if (m_DataDAN.numberOfChannels < 0) {
          updateStatus(statusDAN_t::UNKNOWN);
      }
      m_NumberOfChannels_PV->push(getTimestamp(), m_DataDAN.numberOfChannels);

      std::vector<std::int32_t> arrrayChannels = getArrayOfChannels();
      m_ArrayOfChannels_PV->push(getTimestamp(), arrrayChannels);

      m_DataDAN.numberOfEnabledChannels = dan_publisher_getNumberOfEnabledChannels(m_DataDAN.dataSource);
      if (m_DataDAN.numberOfEnabledChannels < 0) {
          updateStatus(statusDAN_t::UNKNOWN);
      }
      m_NumberOfEnabledChannels_PV->push(getTimestamp(), m_DataDAN.numberOfEnabledChannels);

      std::vector<std::int32_t> arrrayEnaChannels = getArrayOfEnabledChannels();
      m_ArrayOfEnabledChannels_PV->push(getTimestamp(), arrrayEnaChannels);


      msg = "DAN publisher initialised with " + std::to_string(m_DataDAN.numberOfChannels) + " channels, " + std::to_string(m_DataDAN.numberOfEnabledChannels) + " enabled.";
      ndsDANLogDebug(*this, msg.c_str());

      updateStatus(statusDAN_t::SUCCESS);
      updateStatusStream(statusStreamDAN_t::CLOSED);

      timespec stopTime = getTimestamp();
      msg = name + ": Switching on... End [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
      ndsDANLogDebug(*this, msg.c_str());
    }

    void
    ndsdan::DANImpl::STMswitchOff (void)
    {
      timespec startTime = getTimestamp();
      std::string name = this->getFullExternalName();
      std::string msg = name + ": Switching off... Start";
      ndsDANLogDebug(*this, msg.c_str());
      cleanUpDANResources();
      updateStatusStream(statusStreamDAN_t::UNKNOWN);
      timespec stopTime = getTimestamp();
      msg = name + ": Switching off... End [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
      ndsDANLogDebug(*this, msg.c_str());
    }

    void
    ndsdan::DANImpl::STMstart (void)
    {
      timespec startTime = getTimestamp();
      std::string name = this->getFullExternalName();
      m_DataDAN.numberOfEnabledChannels = dan_publisher_getNumberOfEnabledChannels(m_DataDAN.dataSource);
      if (m_DataDAN.numberOfEnabledChannels < 0) {
          updateStatus(statusDAN_t::UNKNOWN);
      }
      std::string msg = name + ": Opening streamer... Start";
      ndsDANLogDebug(*this, msg.c_str());
      msg = name + ": " +  std::to_string(m_DataDAN.numberOfEnabledChannels) + " channels enabled";
      ndsDANLogDebug(*this, msg.c_str());

      std::int32_t errCode = dan_publisher_openStream(m_DataDAN.dataSource, m_DataDAN.samplingRate, m_DataDAN.offsetData);
      if (errCode < 0) {
          updateStatus(statusDAN_t::ERROR_OPEN_STREAM);
          updateStatusStream(statusStreamDAN_t::ERROR_UNKNOWN);
          std::string msg = "Error opening DAN streamer (Error code: " + std::to_string(errCode) + ")";
          ndsDANLogError(*this, msg.c_str());
      } else {
          m_DANStreamerOpen = true;
          updateStatus(statusDAN_t::SUCCESS);
          updateStatusStream(statusStreamDAN_t::IDLE);
          m_CanPublishDataBlocks = true;
      }

      timespec stopTime = getTimestamp();
      msg = name + ": Opening streamer... End [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
      ndsDANLogDebug(*this, msg.c_str());
    }

    void
    ndsdan::DANImpl::STMstop (void)
    {
      timespec startTime = getTimestamp();
      std::string name = this->getFullExternalName();
      std::string msg =  name + ": Stopping streamer... Start";
      ndsDANLogDebug(*this, msg.c_str());
      std::int32_t errCode = dan_publisher_closeStream(m_DataDAN.dataSource);
      if (errCode < 0) {
          updateStatus(statusDAN_t::ERROR_CLOSE_STREAM);
          updateStatusStream(statusStreamDAN_t::ERROR_UNKNOWN);
          msg = "Error closing DAN streamer (Error code: " + std::to_string(errCode) + ")";
          ndsDANLogError(*this, msg.c_str());
      } else {
          m_DANStreamerOpen = false;
          updateStatusStream(statusStreamDAN_t::CLOSED);
          m_CanPublishDataBlocks = false;
      }
      timespec stopTime = getTimestamp();
      msg = name + ": Stopping streamer... End [diff=" + std::to_string(stopTime.tv_sec-startTime.tv_sec) + "s]";
      ndsDANLogDebug(*this, msg.c_str());
    }

    void
    ndsdan::DANImpl::STMrecover (void)
    {
      cleanUpDANResources();
    }

    bool
    ndsdan::DANImpl::STMallowStateChange(const nds::state_t /*currentState*/, const nds::state_t /*currentGlobalState*/, const nds::state_t /*desiredState*/)
    {
      return true;
    }

    void
    ndsdan::DANImpl::cleanUpDANResources (void)
    {

      std::string msg;
      std::int32_t errCode;
      if (m_DataDAN.dataStructure != NULL && m_ReleaseDataStructure != nullptr) {
          if (m_ReleaseDataStructure(m_DataDAN.dataStructure) != NULL) {
              updateStatus(statusDAN_t::ERROR_DATA_RELEASE);
              msg = "Error releasing data structure";
              ndsDANLogError(*this, msg.c_str());
          } else {
              m_DataDAN.dataStructure = NULL;
              m_ReleaseDataStructure = nullptr;
          }
      }


      /** Stop publishing in source from DAN API */
      if (m_DataDAN.dataSource != NULL) {
          if (m_DANStreamerOpen) {
              errCode = dan_publisher_closeStream(m_DataDAN.dataSource);
              if (errCode < 0) {
                  updateStatus(statusDAN_t::ERROR_CLOSE_STREAM);
                  msg = "Error closing DAN streamer (Error code: " + std::to_string(errCode) + ")";
                  ndsDANLogError(*this, msg.c_str());
              } else {
                  m_DANStreamerOpen = false;
              }
          }
          if (m_DataDAN.dataCore != NULL) {
            dan_publisher_unpublishSource(m_DataDAN.dataCore, m_DataDAN.dataSource);
            if (m_DataDAN.dataSource != NULL) {
                updateStatus(statusDAN_t::ERROR_UNPUBLISH_SOURCE);
                msg = "Error unpublishing DAN source ";
                ndsDANLogError(*this, msg.c_str());
            } else {
                m_DataDAN.dataSource = NULL;
            }
          }
      }

      if (m_DataDAN.dataCore != NULL) {
          dan_closeLibrary(m_DataDAN.dataCore);
          if (m_DataDAN.dataCore != NULL) {
              updateStatus(statusDAN_t::ERROR_DAN_CLOSE);
              msg = "Error closing DAN library ";
              ndsDANLogError(*this, msg.c_str());
          } else {
              m_DataDAN.dataCore = NULL;
          }
      }

//      if (m_InitialisedTCN) {
//        errCode = tcn_finalize();
//        if (errCode != TCN_SUCCESS) {
//            updateStatus(statusDAN_t::ERROR_TCN_CLOSE);
//            const char* tcnErr = tcn_strerror(errCode);
//            msg = "Error finalising TCN (" +  std::string(tcnErr) + ")";
//            ndsDANLogError(*this, msg.c_str());
//        } else {
//            m_InitialisedTCN = false;
//        }
//      }

      if (m_ICProgramLoaded) {
          errCode = dan_admin_api_unload_icprog(m_DataDAN.ICProgramName.c_str());
          if (errCode < 0) {
              updateStatus(statusDAN_t::ERROR_ICPROGRAM_UNLOAD);
              ndsDANLogError(*this, "Error Unloading DAN API ICprogram.");
          } else {
              m_ICProgramLoaded = false;
          }
      }

      log_flush_queue();
      log_finalize();

    }

    void
    DANImpl::updateStatus (const statusDAN_t& status)
    {
      updateStatus(getTimestamp(), status);
    }

    void
    DANImpl::updateStatus (const timespec& timestamp, const statusDAN_t& status)
    {
      std::int32_t value = static_cast<std::int32_t>(status);
      m_Status_PV->setValue(timestamp, value);
      m_Status_PV->push(timestamp, value);
    }

  std::string
  DANImpl::parseStatus (const statusDAN_t& status)
  {
    switch(status)
    {
      case statusDAN_t::SUCCESS:
        return "SUCCESS";
      case statusDAN_t::ERROR_XML:
        return "ERROR_XML";
      case statusDAN_t::ERROR_DAN_INIT:
        return "ERROR_DAN_INIT";
      case statusDAN_t::ERROR_DAN_CLOSE:
        return "ERROR_DAN_CLOSE";
      case statusDAN_t::ERROR_ICPROGRAM_UNLOAD:
        return "ERROR_ICPROGRAM_UNLOAD";
      case statusDAN_t::ERROR_TCN_INIT:
        return "ERROR_TCN_INIT";
      case statusDAN_t::ERROR_TCN_CLOSE:
        return "ERROR_TCN_CLOSE";
      case statusDAN_t::ERROR_DATA_INIT:
        return "ERROR_DATA_INIT";
      case statusDAN_t::ERROR_DATA_RELEASE:
        return "ERROR_DATA_RELEASE";
      case statusDAN_t::ERROR_PUBLISH_SOURCE:
        return "ERROR_PUBLISH_SOURCE";
      case statusDAN_t::ERROR_UNPUBLISH_SOURCE:
        return "ERROR_UNPUBLISH_SOURCE";
      case statusDAN_t::ERROR_OPEN_STREAM:
        return "ERROR_OPEN_STREAM";
      case statusDAN_t::ERROR_CLOSE_STREAM:
        return "ERROR_CLOSE_STREAM";
      case statusDAN_t::ERROR_PUT_DATABLOCK:
        return "ERROR_PUT_DATABLOCK";
      case statusDAN_t::ERROR_SET_ATTRIBUTE:
        return "ERROR_SET_ATTRIBUTE";
      case statusDAN_t::ERROR_ENABLE_CHANNEL:
    	return "ERROR_ENABLE_CHANNEL";
      case statusDAN_t::ERROR_DISABLE_CHANNEL:
        return "ERROR_DISABLE_CHANNEL";
      case statusDAN_t::ERROR_UNKNOWN:
        return "ERROR_UNKNOWN";
      case statusDAN_t::UNKNOWN:
      default:
        return "UNKNOWN";
    }
  }

  void
  DANImpl::updateStatusStream (const statusStreamDAN_t& status)
  {
    updateStatusStream(getTimestamp(), status);
  }

  void
  DANImpl::updateStatusStream (const timespec& timestamp,
                               const statusStreamDAN_t& status)
  {
    std::int32_t value = static_cast<std::int32_t>(status);
    m_StatusStream_PV->setValue(timestamp, value);
    m_StatusStream_PV->push(timestamp, value);
  }

  std::string
  DANImpl::parseStatusStream (const statusStreamDAN_t& status)
  {
    switch(status)
    {
      case statusStreamDAN_t::SUCCESS_BLOCK:
        return "SUCCESS_BLOCK";
      case statusStreamDAN_t::CLOSED:
        return "CLOSED";
      case statusStreamDAN_t::IDLE:
        return "IDLE";
      case statusStreamDAN_t::ERROR_OFFSET_OUT_RANGE:
        return "ERROR_OFFSET_OUT_RANGE";
      case statusStreamDAN_t::ERROR_NOT_OPEN_STREAM:
        return "ERROR_NOT_OPEN_STREAM";
      case statusStreamDAN_t::ERROR_OVERFLOW_ALL:
        return "ERROR_OVERFLOW_ALL";
      case statusStreamDAN_t::ERROR_OVERFLOW:
        return "ERROR_OVERFLOW";
      case statusStreamDAN_t::ERROR_UNKNOWN:
        return "ERROR_UNKNOWN";
      case statusStreamDAN_t::UNKNOWN:
      default:
        return "UNKNOWN";
    }
  }




}

