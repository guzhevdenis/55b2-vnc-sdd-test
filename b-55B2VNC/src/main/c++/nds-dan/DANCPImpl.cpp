/**
 * @file DANCPImpl.cpp
 * 
 * @author Javier Moreno (GMV)
 * @date 6 Sep 2018
 */

#include "include/nds-dan/impl/DANCPImpl.h"

#define S2NS 1000000000 //Seconds to nanoSeconds

namespace ndsdan
{

  ndsdan::DANCPImpl::DANCPImpl (const std::string& name,
                                genericDANFunction_t dataStructureInitializer,
                                genericFunction_t dataStructureReleaser,
                                charDANFunction_t blockHeaderSetter,
								nds::autoEnable_t autoEnable) :
        DANImpl(name,
                dataStructureInitializer,
                dataStructureReleaser,
                blockHeaderSetter,
				autoEnable),
        m_DataDANCP(),
        m_canAcquire(true),
        m_canPublish(false)
  {


    m_TimeBetweenBlocks_PV.reset(new nds::PVDelegateOutImpl<double>("TimeBetweenBlocks",
                                 std::bind(&DANCPImpl::setTimeBetweenBlocks, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2)));
    m_TimeBetweenBlocks_PV->setDescription("Time between consecutive data blocks");
    m_TimeBetweenBlocks_PV->setUnits("ns");
    addChild(m_TimeBetweenBlocks_PV);

    m_TimeBetweenBlocks_RBVPV.reset(new nds::PVDelegateInImpl<double>("TimeBetweenBlocks_RBV",
                                 std::bind(&DANCPImpl::readTimeBetweenBlocks, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2)));
    m_TimeBetweenBlocks_RBVPV->setDescription("Readback value of time between blocks");
    m_TimeBetweenBlocks_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
    m_TimeBetweenBlocks_RBVPV->setUnits("ns");
    addChild(m_TimeBetweenBlocks_RBVPV);

    m_MaxAcquisitionTime_PV.reset(new nds::PVDelegateOutImpl<double>("MaxAcquisitionTime",
                                 std::bind(&DANCPImpl::setMaxAcquisitionTime, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2)));
    m_MaxAcquisitionTime_PV->setDescription("Maximum acquisition time");
    m_MaxAcquisitionTime_PV->setUnits("s");
    addChild(m_MaxAcquisitionTime_PV);

    m_MaxAcquisitionTime_RBVPV.reset(new nds::PVDelegateInImpl<double>("MaxAcquisitionTime_RBV",
                                 std::bind(&DANCPImpl::readMaxAcquisitionTime, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2)));
    m_MaxAcquisitionTime_RBVPV->setDescription("Readback value of Maximum ACQ time");
    m_MaxAcquisitionTime_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
    m_MaxAcquisitionTime_RBVPV->setUnits("s");
    addChild(m_MaxAcquisitionTime_RBVPV);

    m_NumberOfBlocks_PV.reset(new nds::PVDelegateOutImpl<std::int32_t>("NumberOfBlocks",
                                 std::bind(&DANCPImpl::setNumberOfBlocks, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2)));
    m_NumberOfBlocks_PV->setDescription("Number of data blocs in DAN buffer");
    addChild(m_NumberOfBlocks_PV);

    m_NumberOfBlocks_RBVPV.reset(new nds::PVDelegateInImpl<std::int32_t>("NumberOfBlocks_RBV",
                                 std::bind(&DANCPImpl::readNumberOfBlocks, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2)));
    m_NumberOfBlocks_RBVPV->setDescription("Readback number of blocks in DAN buffer");
    m_NumberOfBlocks_RBVPV->setScanType(nds::scanType_t::interrupt, 0);
    addChild(m_NumberOfBlocks_RBVPV);

  }

  ndsdan::DANCPImpl::~DANCPImpl ()
  {
  }


  std::uint64_t
  DANCPImpl::getTimeBetweenBlocks ()
  {
    std::lock_guard<std::recursive_mutex> lock(m_Mutex);
    return m_DataDANCP.timeBetweenBlocks;
  }

  void
  DANCPImpl::readTimeBetweenBlocks (timespec* timestamp, double* timeBetweenBlocks)
  {
    *timeBetweenBlocks = static_cast<double> (getTimeBetweenBlocks());
    *timestamp = getTimestamp();
  }

  double
  DANCPImpl::getMaxAcquisitionTime ()
  {
    std::lock_guard<std::recursive_mutex> lock(m_Mutex);
    return m_DataDANCP.maxAcquisitionTimeNs;
  }

  void
  DANCPImpl::readMaxAcquisitionTime (timespec* timestamp, double* maxAcquisitionTime)
  {
    *maxAcquisitionTime = static_cast<double> (getMaxAcquisitionTime());
    *timestamp = getTimestamp();
  }

  std::int32_t
  ndsdan::DANCPImpl::getNumberOfBlocks ()
  {
    std::lock_guard<std::recursive_mutex> lock(m_Mutex);
    return m_DataDANCP.numberOfBlocks;
  }

  void
  ndsdan::DANCPImpl::readNumberOfBlocks (timespec* timestamp,
                                         std::int32_t* numberOfBlocks)
  {
    *numberOfBlocks = getNumberOfBlocks();
    *timestamp = getTimestamp();
  }

  void
  DANCPImpl::setTimeBetweenBlocks (const timespec& timestamp,
                                   const double& timeBetweenBlocks)
  {
    nds::state_t currentState = m_StateMachine->getLocalState();
    if (currentState == nds::state_t::off) {
        std::lock_guard<std::recursive_mutex> lock(m_Mutex);
        m_DataDANCP.timeBetweenBlocks = static_cast<std::uint64_t>(timeBetweenBlocks);
        m_TimeBetweenBlocks_RBVPV->push(timestamp, timeBetweenBlocks);
    } else {
        ndsErrorStream(*this) << "The time interval between DAN data blocks can only be modified in OFF state." << std::endl;
    }
  }

  void
  DANCPImpl::setMaxAcquisitionTime (const timespec& timestamp,
                                    const double& maxAcquisitionTime)
  {
    nds::state_t currentState = m_StateMachine->getLocalState();
    if (currentState == nds::state_t::off) {
        std::lock_guard<std::recursive_mutex> lock(m_Mutex);
        m_DataDANCP.maxAcquisitionTimeNs = maxAcquisitionTime;
        m_MaxAcquisitionTime_RBVPV->push(timestamp, maxAcquisitionTime);
    } else {
        ndsErrorStream(*this) << "The DAN maximum acquisition time can only be modified in OFF state." << std::endl;
    }
  }

  void
  ndsdan::DANCPImpl::setNumberOfBlocks (const timespec& timestamp,
                                        const std::int32_t& numberOfBlocks)
  {
    nds::state_t currentState = m_StateMachine->getLocalState();
    if (currentState == nds::state_t::off) {
        std::lock_guard<std::recursive_mutex> lock(m_Mutex);
        m_DataDANCP.numberOfBlocks = numberOfBlocks;
        m_NumberOfBlocks_RBVPV->push(timestamp, m_DataDANCP.numberOfBlocks);
    } else {
        ndsErrorStream(*this) << "The DAN number of blocks can only be modified in OFF state." << std::endl;
    }
  }

  statusDAN_t
  DANCPImpl::publishSource (void)
  {
    //std::string msg;
    //m_DataDAN.bufferSize = m_DataDAN.blockNumberOfSamples * m_DataDANCP.numberOfBlocks * m_DataDAN.sampleSize * m_DataDAN.numberOfChannels;
    //Number of channels only available after publishing the dan Source. Can be retrieved using dan_publisher_getNumberOfChannels() function.
    //m_DataDAN.bufferSize = m_DataDAN.blockNumberOfSamples * m_DataDANCP.numberOfBlocks * m_DataDAN.sampleSize * 16;
    //setBufferSize(getTimestamp(), m_DataDAN.bufferSize);
    genericPtr_t ptr = dan_publisher_publishSource_withDAQBuffer(m_DataDAN.dataCore,
                                                                 m_DataDAN.sourceName.c_str(),
                                                                 m_DataDAN.bufferSize);
    if (ptr == NULL) {
        updateStatus(statusDAN_t::ERROR_PUBLISH_SOURCE);
        ndsErrorStream(*this) << "Error while publishing DAN source. Check parameters: BlockNumberOfSamples, NumberOfBlocks, NumberOfChannels and SourceName." << std::endl;
        return statusDAN_t::ERROR_PUBLISH_SOURCE;
    } else {
        m_DataDAN.dataSource = ptr;
        return statusDAN_t::SUCCESS;
    }
  }

  void
  DANCPImpl::putDataBlock(const std::uint64_t& timestamp)
  {
    if (m_CanPublishDataBlocks) {
      std::int64_t errorCode = 0;
      m_DataDAN.blockHeader = m_SetBlockHeader(m_DataDAN);
      errorCode = dan_publisher_putDataBlock(m_DataDAN.dataSource,
                                             (timestamp),
                                             (char *) m_DataDAN.dataStructure,
                                             m_DataDAN.blockNumberOfSamples * m_DataDAN.numberOfEnabledChannels * m_DataDAN.sampleSize,
                                             m_DataDAN.blockHeader);
      if (errorCode < 0) {
        updateStatus(statusDAN_t::ERROR_PUT_DATABLOCK);
        std::string msg = "";
        switch (errorCode) {
          case DAN_OFFSET_OUT_OF_RANGE:
            updateStatusStream(statusStreamDAN_t::ERROR_OFFSET_OUT_RANGE);
            msg = "Error putting block reference: offset is out of DAQ buffer range";
            ndsDANLogError(*this, msg.c_str());
            break;
          case DAN_NOT_OPEN_STREAM:
            updateStatusStream(statusStreamDAN_t::ERROR_NOT_OPEN_STREAM);
            msg = "Error putting block reference: stream has not been previously open";
            ndsDANLogError(*this, msg.c_str());
            break;
          case DAN_OVERFLOW_ALL:
            updateStatusStream(statusStreamDAN_t::ERROR_OVERFLOW_ALL);
            msg = "Error putting block reference: buffer overflow for all DAN streamers";
            ndsDANLogError(*this, msg.c_str());
            break;
          case DAN_OVERFLOW:
            updateStatusStream(statusStreamDAN_t::ERROR_OVERFLOW);
            msg = "Error putting block reference: buffer overflow for not all DAN streamers";
            ndsDANLogError(*this, msg.c_str());
            break;
          default:
            updateStatusStream(statusStreamDAN_t::ERROR_UNKNOWN);
            msg = "Error putting block reference: unknown error";
            ndsDANLogError(*this, msg.c_str());
            break;
        }
      } else {
          updateStatus(statusDAN_t::SUCCESS);
          updateStatusStream(statusStreamDAN_t::SUCCESS_BLOCK);
          updateStatusStream(statusStreamDAN_t::IDLE);
      }
      m_DataDAN.samplesCounter += m_DataDAN.blockNumberOfSamples;
    }
  }

}

