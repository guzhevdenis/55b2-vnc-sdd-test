
#include "include/nds-dan/DAN.h"

#include "nds3/impl/nodeImpl.h"
#include "include/nds-dan/impl/DANImpl.h"


namespace ndsdan
{

  ndsdan::DAN::DAN ():Node()
  {
  }

  ndsdan::DAN::DAN (const DAN& right) :
    Node(std::static_pointer_cast<nds::NodeImpl>(right.m_pImplementation))
  {
  }

  ndsdan::DAN::DAN (std::shared_ptr<DANImpl> pvImpl): Node(std::static_pointer_cast<nds::NodeImpl>(pvImpl))
  {
  }
  ndsdan::DAN::~DAN ()
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->cleanUpDANResources();
  }


  DAN&
  ndsdan::DAN::operator = (const DAN& right)
  {
    m_pImplementation = right.m_pImplementation;
    return *this;
  }

  std::string
  DAN::getSourceName ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getSourceName();
  }

  std::string
  DAN::getICProgramName ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getICProgramName();
  }

  std::int32_t
  DAN::getSamplingRate ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getSamplingRate();
  }

  std::int32_t
  DAN::getBufferSize ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getBufferSize();
  }

  std::int32_t
  DAN::getNumberOfChannels ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getNumberOfChannels();
  }

  std::int32_t
  DAN::getNumberOfEnabledChannels ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getNumberOfEnabledChannels();
  }

  std::int32_t
  DAN::getSampleSize ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getSampleSize();
  }

  std::int32_t
  DAN::getBlockNumberOfSamples ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getBlockNumberOfSamples();
  }

  std::string
  DAN::getPathXML ()
  {
    return std::static_pointer_cast<DANImpl>(m_pImplementation)->getPathXML();
  }

  void
  DAN::setSourceName (const timespec& timestamp, const std::string& sourceName)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->setSourceName(timestamp, sourceName);
  }

  void
  DAN::setICProgramName (const timespec& timestamp,
                         const std::string& ICProgramName)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->setICProgramName(timestamp, ICProgramName);
  }

  void
  DAN::setSamplingRate (const timespec& timestamp,
                        const std::int32_t& samplingRate)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->setSamplingRate(timestamp, samplingRate);
  }

  void
  DAN::setBufferSize (const timespec& timestamp, const std::int32_t& bufferSize)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->setBufferSize(timestamp, bufferSize);
  }

  void
  DAN::setSampleSize (const timespec& timestamp, const std::int32_t& sampleSize)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->setSampleSize(timestamp, sampleSize);
  }

  void
  DAN::setBlockNumberOfSamples (const timespec& timestamp,
                                const std::int32_t& blockNumberOfSamples)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->setBlockNumberOfSamples(timestamp, blockNumberOfSamples);
  }
  void
  DAN::setPathXML(const timespec& timestamp, const std::string& pathXML)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->setPathXML(timestamp, pathXML);
  }

  bool
  DAN::setUserAttribute(const std::string& name, const std::string& value, const std::string& units, const std::string& description)
  {
	  return std::static_pointer_cast<DANImpl>(m_pImplementation)->setUserAttribute(name, value, units, description);
  }

  void
  DAN::setDataBlockHeaderDim(const std::int32_t& dim1, const std::int32_t& dim2)
  {
	  std::static_pointer_cast<DANImpl>(m_pImplementation)->setDataBlockHeaderDim(dim1, dim2);
  }

  void
  DAN::setDataBlockHeaderSampRate(const double& samplingRate)
  {
	  std::static_pointer_cast<DANImpl>(m_pImplementation)->setDataBlockHeaderSampRate(samplingRate);
  }

  void
  DAN::setDataBlockHeaderOpMode(const std::int64_t& opMode)
  {
	  std::static_pointer_cast<DANImpl>(m_pImplementation)->setDataBlockHeaderOpMode(opMode);
  }

  bool
  DAN::setChannelEnable(std::int32_t channelNumber){
	  return std::static_pointer_cast<DANImpl>(m_pImplementation)->setChannelEnable(channelNumber);
  }

  bool
  DAN::setChannelDisable(std::int32_t channelNumber){
	  return std::static_pointer_cast<DANImpl>(m_pImplementation)->setChannelDisable(channelNumber);
  }

  void
  DAN::putDataBlock (const std::uint64_t& timestamp)
  {
    std::static_pointer_cast<DANImpl>(m_pImplementation)->putDataBlock(timestamp);
  }

}

