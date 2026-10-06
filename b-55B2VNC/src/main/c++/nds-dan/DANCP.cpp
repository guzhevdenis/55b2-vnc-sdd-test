
/**
 * @file DANCP.cpp
 *
 * @author Javier Moreno (GMV)
 * @date 10 Sep 2018
 */


#include "include/nds-dan/DANCP.h"

#include "include/nds-dan/impl/DANCPImpl.h"

namespace ndsdan
{

  ndsdan::DANCP::DANCP () : DAN()
  {
  }

  ndsdan::DANCP::DANCP (const DANCP& right) : DAN(std::static_pointer_cast<DANImpl>(right.m_pImplementation))
  {
  }

  ndsdan::DANCP::DANCP (const std::string& name,
                        genericDANFunction_t dataStructureInitializer,
                        genericFunction_t dataStructureReleaser,
                        charDANFunction_t blockHeaderSetter,
						nds::autoEnable_t autoEnable) :
        DAN(std::shared_ptr<DANCPImpl>(new DANCPImpl(name,
                                                     dataStructureInitializer,
                                                     dataStructureReleaser,
                                                     blockHeaderSetter,
													 autoEnable)))
  {
  }

  ndsdan::DANCP::~DANCP ()
  {
  }

  DANCP&
  ndsdan::DANCP::operator = (const DANCP& right)
  {
    m_pImplementation = right.m_pImplementation;
    return *this;
  }

  std::uint64_t
  ndsdan::DANCP::getTimeBetweenBlocks ()
  {
    return std::static_pointer_cast<DANCPImpl>(m_pImplementation)->getTimeBetweenBlocks();
  }

  double
  ndsdan::DANCP::getMaxAcquisitionTime ()
  {
    return std::static_pointer_cast<DANCPImpl>(m_pImplementation)->getMaxAcquisitionTime();
  }

  std::int32_t
  ndsdan::DANCP::getNumberOfBlocks ()
  {
    return std::static_pointer_cast<DANCPImpl>(m_pImplementation)->getNumberOfBlocks();
  }

  void
  ndsdan::DANCP::setTimeBetweenBlocks (const timespec& timestamp,
                                       const double& timeBetweenBlocks)
  {
    std::static_pointer_cast<DANCPImpl>(m_pImplementation)->setTimeBetweenBlocks(timestamp, timeBetweenBlocks);
  }

  void
  ndsdan::DANCP::setMaxAcquisitionTime (const timespec& timestamp,
                                        const double& maxAcquisitionTime)
  {
    std::static_pointer_cast<DANCPImpl>(m_pImplementation)->setMaxAcquisitionTime(timestamp, maxAcquisitionTime);
  }

  void
  ndsdan::DANCP::setNumberOfBlocks (const timespec& timestamp,
                                    const std::int32_t& numberOfBlocks)
  {
    std::static_pointer_cast<DANCPImpl>(m_pImplementation)->setNumberOfBlocks(timestamp, numberOfBlocks);
  }

}


