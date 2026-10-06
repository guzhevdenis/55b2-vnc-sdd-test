
/**
 * @file DAN.h
 * @brief Header file that defines the ndsdan::DAN class.
 * @author Javier Moreno (GMV)
 * @ingroup NDS-DAN
 * @date September 2018
 * @version 1.0
 */


#ifndef NDS_DAN_INCLUDE_DAN_H_
#define NDS_DAN_INCLUDE_DAN_H_

#include <nds3/nds.h>

#include "../nds-dan/definitions.h"

namespace ndsdan
{

class DANImpl;

/**
 * @brief Class to deal with any kind of DAN node.
 * This class defines the common interface to any child DAN
 * node. It provides access to the features shared by DAN nodes
 * by means of its underlying PVs. @n
 * Note that instantiation of this class is not directly allowed.
 * For including a DAN node within your device driver you must add
 * a @ref DANCP or a @ref DANDA node that inherits from this class
 * and are focused on each particular application.
 * @ingroup NDS-DAN
 */
class NDS_DAN_API DAN : public nds:: Node
{
public:

  /**
   * @brief Overloading of @e = operator to allow copying DAN instances.
   * @param right Reference operator.
   * @return Reference to the copied element.
   */
  DAN& operator=(const DAN& right);

  /**
   * @brief Provide the name of the %DAN data source to publish data.
   * @return String with the %DAN data source name.
   *
   * See @ref dataDAN_t::sourceName for further details.
   */
  std::string getSourceName();

  /**
   * @brief Provide the name of the %DAN ICProgram that publishes data.
   * @return String with the %DAN ICProgram name.
   *
   * See @ref dataDAN_t::ICProgramName for further details.
   */
  std::string getICProgramName();

  /**
   * @brief Provide the sampling rate to publish data, in samples per second.
   * @return Sampling rate to publish data, in samples per second.
   *
   * See @ref dataDAN_t::samplingRate for further details.
   */
  std::int32_t getSamplingRate();

  /**
   * @brief Provide the size of the %DAN DAQ buffer, in Bytes.
   * @return Size of the internal %DAN DAQ buffer, in Bytes.
   *
   * See @ref dataDAN_t::bufferSize for further details.
   */
  std::int32_t getBufferSize();

  /**
   * @brief Provide the number of channels in the %DAN source.
   * @return Number of channels defined in the configuration of the %DAN source.
   *
   * See @ref dataDAN_t::numberOfChannels for further details.
   */
  std::int32_t getNumberOfChannels();

  /**
   * @brief Provide the number of enabled channels at this moment in the %DAN source.
   * @return Number of enabled channels.
   */
  std::int32_t getNumberOfEnabledChannels();

  /**
   * @brief Provide the size of a single sample in the %DAN data buffer, in Bytes.
   * @return Size of a single sample in %DAN buffer, in Bytes..
   *
   * See @ref dataDAN_t::sampleSize for further details.
   */
  std::int32_t getSampleSize();

  /**
   * @brief Provide the size of a data block in the %DAN buffer, in number of samples.
   * @return Number of samples of a single data block in the %DAN buffer.
   *
   * See @ref dataDAN_t::blockNumberOfSamples for further details.
   */
  std::int32_t getBlockNumberOfSamples();

  /**
   * @brief Provide the file path of the XML required to initialise the @e danAPItool.
   * @return Path and file name of the XML file to configure the @e danAPItool.
   *
   * See @ref dataDAN_t::blockNumberOfSamples for further details.
   */
  std::string getPathXML();

  /**
   * @brief Set the name of the %DAN data source to publish data.
   * @param timestamp Timestamp to indicate when the function is called.
   * @param sourceName Name of the %DAN data source to be set.
   *
   * It can only be called when the state machine is in nds::state_t::OFF state. @n
   * See @ref dataDAN_t::sourceName for further details.
   */
  void setSourceName(const timespec& timestamp, const std::string& sourceName);

  /**
   * @brief Set the name of the %DAN ICProgram that publishes data.
   * @param timestamp Timestamp to indicate when the function is called.
   * @param ICProgramName Name of the %DAN ICProgram to be set.
   *
   * It can only be called when the state machine is in nds::state_t::OFF state. @n
   * See @ref dataDAN_t::ICProgramName for further details.
   */
  void setICProgramName(const timespec& timestamp, const std::string& ICProgramName);

  /**
   * @brief Set the sampling rate to publish data, in samples per second.
   * @param timestamp Timestamp to indicate when the function is called.
   * @param samplingRate Sampling rate to publish data, in samples per second.
   *
   * It can only be called when the state machine is in nds::state_t::OFF state. @n
   * See @ref dataDAN_t::samplingRate for further details.
   */
  void setSamplingRate(const timespec& timestamp, const std::int32_t& samplingRate);

  /**
   * @brief Set the size of the %DAN DAQ buffer, in Bytes.
   * @param timestamp Timestamp to indicate when the function is called.
   * @param bufferSize Size of the %DAN DAQ buffer to be set, in Bytes.
   *
   * It can only be called when the state machine is in nds::state_t::OFF state. @n
   * See @ref dataDAN_t::bufferSize for further details.
   */
  void setBufferSize(const timespec& timestamp, const std::int32_t& bufferSize);

  /**
   * @brief Set the size of the sample size in the %DAN data buffer, in Bytes.
   * @param timestamp Timestamp to indicate when the function is called.
   * @param sampleSize Size of the sample in %DAN buffer to be set, in Bytes.
   *
   * It can only be called when the state machine is in nds::state_t::OFF state. @n
   * See @ref dataDAN_t::sampleSize for further details.
   */
  void setSampleSize(const timespec& timestamp, const std::int32_t& sampleSize);

  /**
   * @brief Set the size of a data block in the %DAN buffer, in number of samples.
   * @param timestamp Timestamp to indicate when the function is called.
   * @param blockNumberOfSamples Size of the data block in %DAN buffer to be set, in number of samples.
   *
   * It can only be called when the state machine is in nds::state_t::OFF state. @n
   * See @ref dataDAN_t::blockNumberOfSamples for further details.
   */
  void setBlockNumberOfSamples(const timespec& timestamp, const std::int32_t& blockNumberOfSamples);


  /**
   * @brief Set the file path of the XML required to initialise the @e danAPItool.
   * @param timestamp Timestamp to indicate when the function is called.
   * @param pathXML Path of the %DAN XML configuration to be set.
   *
   * It can only be called when the state machine is in nds::state_t::OFF state. @n
   * See @ref dataDAN_t::pathXML for further details.
   */
  void setPathXML(const timespec& timestamp, const std::string& pathXML);

  /**
   * @brief Adds/updates a user defined stream attribute.
   *
   * Adds and/or overwrites (if exists) a user defined stream attribute. This change will only be reflected in
   * new opened streams. This function makes possible to change the value of a stream attribute
   * or to add a new stream attribute for future streams.
   *
   * It can only be called when the state machine is in nds::state_t::ON or nds::state_t::RUNNING
   *
   * @param name String containing the name of the attribute
   * @param value String containing the value to assign to the attribute
   * @param units String with the units of the value (default = "")
   * @param description String with a description of the attribute (default = "")
   * @return True in case of success. False value in case of error.
   */
  bool setUserAttribute(const std::string& name, const std::string& value, const std::string& units="", const std::string& description="");

  /**
   * @brief Sets the new dimensionality of the samples. Only in RUNNING state.
   *
   * Changes the dimensions of a sample into a datablock. Once the dimensions are changed using this function,
   * following datablocks will have the new dimensions until they are changed. The sample dimensions can only
   * be changed in RUNNING state
   *
   * @param dim1 new value to be used as first dimension of a sample
   * @param dim2 new value to be used as second dimension of a sample
   */
  void setDataBlockHeaderDim(const std::int32_t& dim1, const std::int32_t& dim2);

  /**
   * @brief Sets the new sampling rate. Only in RUNNING state.
   *
   * Changes the sampling rate of the following datablocks. Once the sampling rate is changed, following datablocks
   * will have the new sampling rate until it is changed. The sampling rate can only be changed in RUNNING state
   *
   * @param samplingRate new value to be used as sampling rate for following datablocks
   */
  void setDataBlockHeaderSampRate(const double& samplingRate);

  /**
   * @brief Sets the new operational mode. Only in RUNNING state.
   *
   * Changes the device operational mode of the following datablocks. Once the device operational mode is changed, following datablocks
   * will have the new device operational mode until it is changed. The device operational mode can only be changed
   * in RUNNING state.
   *
   * @param opMode new value to be used as device operational mode for following datablocks
   */
  void setDataBlockHeaderOpMode(const std::int64_t& opMode);

  /**
   * @brief Sets to enable the channel specified.
   * @param channelNumber Channel to enable
   * @return True if success, false otherwise.
   */
  bool setChannelEnable(std::int32_t channelNumber);

  /**
   * @brief Sets to disable the channel specified.
   * @param channelNumber Channel to disable
   * @return True if success, false otherwise.
   */
  bool setChannelDisable(std::int32_t channelNumber);

  /**
   * @brief Function to put data blocks in the DAN streamer.
   * It can only publish data blocks when the state machine is in nds::state_t::RUNNING state. @n
   * @param timestamp Timestamp to indicate when the function is called.
   */
  void putDataBlock(const std::uint64_t& timestamp);

protected:


  /**
   * @brief Default constructor to initialise the DAN node by means of its parent constructor.
   *
   * It is a protected constructor to forbid instantiation of this class from user applications.
   */
  DAN();

  /**
   * @brief Constructor to initialise the DAN node from another to be copied by the parent constructor.
   * @param right Reference node to be copied in instantiation.
   *
   * It is a protected constructor to forbid instantiation of this class from user applications.
   */
  DAN(const DAN& right);

  /**
   * @brief Constructor to initialise the DAN node from another pointer to its implementation node to be copied by the parent constructor.
   * @param pvImpl Pointer to the implementation node to be copied in instantiation.
   *
   * It is a protected constructor to forbid instantiation of this class from user applications.
   */
  DAN(std::shared_ptr<DANImpl> pvImpl);

  /**
   * @brief Destructor to release allocated resources.
   */
  ~DAN();

};

}


#endif /* NDS_DAN_INCLUDE_DAN_H_ */
