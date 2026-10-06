//+======================================================================
// $HeadURL: https://svnpub.codac.iter.org/codac/iter/codac/dev/units/m-codac-unit-templates/branches/codac-core-6.3/templates/cpp/main/c++/lib/lib.h.template $
// $Id: lib.h.template 116150 2021-01-04 09:45:04Z kimh19 $
//
// Project       : CODAC Core System
//
// Description   : $progName program
//
// Author        : vcosta
//
// Copyright (c) : 2010-2021 ITER Organization,
//                 CS 90 046
//                 13067 St. Paul-lez-Durance Cedex
//                 France
//
// This file is part of ITER CODAC software.
// For the terms and conditions of redistribution or use of this software
// refer to the file ITER-LICENSE.TXT located in the top level directory
// of the distribution package.
//
//-======================================================================
#ifndef NDS_DAN_PLUGIN_H_
#define NDS_DAN_PLUGIN_H_

#include <nds3/nds.h>
#include <nds-dan/nds-dan.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

template<typename T>
struct is_vector {
	static constexpr bool value = false;
};

template<template<typename ...> class C, typename U>
struct is_vector<C<U>>
{
	static constexpr bool value =
	std::is_same<C<U>,std::vector<U>>::value;
};

/**
 * @brief Class used to handle NDS-DAN-Plugin
 *
 * It contains the port implementation and all the nodes pointers to control the board.
 *
 * @ingroup NDS-DAN-Plugin
 */
class PluginNDSDAN {
public:
	/**
	 *
	 * @brief Constructor of the class.
	 *
	 * It creates the necessary NDS-PVs and NDS-DAN nodes for the correct operation of the plugin
	 *
	 * @param factory    		The control system factory that requested the creation of the device
	 * @param deviceName    	The name given to the device
	 * @param namedParameters 	Optional parameters passed to the device
	 */
	PluginNDSDAN(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t &namedParameters);

	/**
	 * @brief Destructor of the class.
	 */
	~PluginNDSDAN();

private:

	/**********************************************************
	 * STATE MACHINE FUNCTIONS
	 **********************************************************/
	/**
	 * @brief Switch on the device driver.
	 * This method is required by the StateMachine's constructor and its implementation is empty in this example.
	 */
	void stm_SwitchOn(void);
	/**
	 * @brief Switch off the device driver.
	 * This method is required by the StateMachine's constructor and its implementation is empty in this example.
	 */
	void stm_SwitchOff(void);
	/**
	 * @brief Start the device driver.
	 * This method is required by the StateMachine's constructor and it launches the thread that emulates
	 * the data acquisition.
	 */
	void stm_Start(void);
	/**
	 * @brief Stop the device driver.
	 * This method is required by the StateMachine's constructor and it stops the thread that emulates
	 * the data acquisition.
	 */
	void stm_Stop(void);
	/**
	 * @brief Recover the device driver from a FAULT state.
	 * This method is required by the StateMachine's constructor and its implementation is empty in this example.
	 */
	void stm_recover(void);
	/**
	 * @brief Check if state transitions is allowed.
	 * @return <code>TRUE</code> when transition is allowed, <code>FALSE</code> otherwise.
	 * This method is required by the StateMachine's constructor and it verifies if the required state transition is allowed.
	 */
	bool stm_allowChange(const nds::state_t currentState, const nds::state_t currentGlobalState,
			const nds::state_t desiredState);
	//END STATE MACHINE FUNCTIONS///////////////////////////////

	/**********************************************************
	 * PARSING AND NODE CONFIGURATION
	 **********************************************************/
	/** @struct sourceDataCP_t
	 * @brief Structure to store needed data for CP method
	 * @ingroup NDS-DAN-CP
	 * @var sourceDataCP_t::node
	 * Contains the DANCP node
	 * @var sourceDataCP_t::ptr
	 * Pointer to the data to put in the data block
	 * @var sourceDataCP_t::elemPushed
	 * Number of elements pushed
	 */
	struct sourceDataCP_t {
		ndsdan::DANCP node;
		std::shared_ptr<void> ptr;
		std::uint64_t elemPushed = 0;
	};

	/**
	 * @brief Valid options for NDS-DAN methods
	 * @ingroup NDS-DAN-NODE
	 */
	enum class methodDAN {CP, DA};

	/** @struct sourceConfig_t
	 * @brief Structure to store parsed configuration data for the node from the XML
	 * @ingroup NDS-DAN-NODE-SOURCE
	 * @var sourceConfig_t::method
	 * DAN method to set data, CP or DA
	 * @var sourceConfig_t::ndsPVName
	 * NDS-PV's name from which the data will be obtained
	 * @var sourceConfig_t::ndsPVType
	 * NDS-PV's type from which the data will be obtained
	 * @var sourceConfig_t::xmlPath
	 * Path to the DAN ICProgram file
	 * @var sourceConfig_t::sourceName
	 * Name of the DAN data source to publish data.
	 * @var sourceConfig_t::ICProgram
	 * Name of the ICProgram
	 * @var sourceConfig_t::samplingRate
	 * Sampling rate to publish data, in samples per second.
	 * @var sourceConfig_t::sampleNDSSize
	 * Size of each element in the NDS-PV, in Bytes
	 * @var sourceConfig_t::sampleDANSize
	 * Size of the sample size in the DAN data buffer, in Bytes
	 * @var sourceConfig_t::blockNumberOfSamples
	 * Size of a data block in the DAN buffer, in number of samples
	 * @var sourceConfig_t::bufferSize
	 * Size of the DAN DAQ buffer, in Bytes.
	 * @var sourceConfig_t::numBlocks
	 * [Only DANCP] Number of blocks in the internal DAQ buffer of the DAN API.
	 */
	struct sourceConfig_t{
		methodDAN method;
		std::string ndsPVName;
		nds::dataType_t ndsPVType;

		std::string xmlPath;
		std::string sourceName;
		std::string ICProgram;
		std::int32_t samplingRate;
		std::int32_t sampleNDSSize;
		std::int32_t sampleDANSize;
		std::int32_t blockNumberOfSamples;
		std::int32_t bufferSize;

		std::int32_t numBlocks;
	};

	/** @struct attrData_t
	 * @brief Structure to store attribute info
	 * @ingroup NDS-DAN-ATTR
	 * @var attrData_t::name
	 * Name of the attribute to add/update
	 * @var attrData_t::units
	 * Units of the attribute
	 * @var attrData_t::description
	 * Description of the attribute
	 * @var attrData_t::parentSource
	 * Name of the attribute's source
	 * @var attrData_t::ndsPVName
	 * NDS-PV's name from which to get the attribute's value
	 * @var attrData_t::ndsPVType
	 * NDS-PV's type form which to get the attribute's value
	 */
	struct attrData_t {
		std::string name;
		std::string units;
		std::string description;
		std::string parentSource;
		std::string ndsPVName;
		nds::dataType_t ndsPVType;
	};

	/** @struct attrDataWPV_t
	 * @brief Structure to store attribute info and the created PV (to get the name once the rootNode initializes)
	 * @ingroup NDS-DAN-ATTR
	 * @var attrDataWPV_t::attrData
	 * See @ref attrData_t
	 * @var attrDataWPV_t::ndsPVAux
	 * NDS-PV Out created by @ref PluginNDSDAN for each attribute
	 * It is stored because the full name is needed and it can only be obtained after the rootNode has been initialized, and the PV is created before that point
	 */
	struct attrDataWPV_t{
		attrData_t attrData;
		nds::PVBaseOut ndsPVAux;
	};

	/**
	 * @brief Structure to store datablock metadata info.  Can be related to sample dimensions, sampling rate and/or operational mode.
	 * @ingroup NDS-DAN-DB
	 */
	struct datablockMetadata_t{
		std::string ndsPVDim;				/**< Name of the NDS-PV that will push the dimensions value */
		std::string ndsPVSampRate;			/**< Name of the NDS-PV that will push the sampling rate value */
		std::string ndsPVOpMode;			/**< Name of the NDS-PV that will push the operational mode value */

		nds::dataType_t ndsTypeDim;			/**< NDS-PV type of the dimensions value */
		nds::dataType_t ndsTypeSampRate;	/**< NDS-PV type of the sampling rate value */
		nds::dataType_t ndsTypeOpMode;		/**< NDS-PV type of the operational mode value */
	};

	/**
	 * @brief Structure to hold datablock metadata configuration and the created PVs
	 * @ingroup NDS-DAN-DB
	 */
	struct datablockMetadataWPV_t{
		datablockMetadata_t datablockMetadata; /**< See @ref datablockMetadata_t*/

		nds::PVBaseOut ndsPVDimAux;			/**< NDS-PV Out created by @ref PluginNDSDAN responsible of updating the value in the DAN*/
		nds::PVBaseOut ndsPVSampRateAux;	/**< NDS-PV Out created by @ref PluginNDSDAN responsible of updating the value in the DAN*/
		nds::PVBaseOut ndsPVOpModeAux;		/**< NDS-PV Out created by @ref PluginNDSDAN responsible of updating the value in the DAN*/
	};

	struct channelEnableDisable_t {
		std::string ndsPVName;
		nds::dataType_t ndsType;
	};

	struct channelEnableDisableWPV_t {
		channelEnableDisable_t channelEnableDisable;

		nds::PVBaseOut ndsPVAux;
	};

	/** @struct danXMLData_t
	 * @brief Structure to store node configuration and info for a source and attribute info for such source parsed from the XML
	 * @ingroup XMLParsing
	 * @var danXMLData_t::sourceConfig
	 * Structure with configuration/info for the node
	 * @var danXMLData_t::sourcePVAux
	 * NDS-PV Out created by the @ref PluginNDSDAN to subscribe the source PV.
	 * Is is stored because the full name is needed and it can only be obtained after the rootNode has been initialized, and the PV is created before that point.
	 * @var danXMLData_t::ndsDANPVName
	 * NDS-PV name created by the @ref PluginNDSDAN to subscribe the source PV
	 * @var danXMLData_t::attrsList
	 * Vector containing configuration/info related to attributes of a source and the created PV
	 * It is stored because the full name is needed and it can only be obtained after the rootNode has been initialized, and the PV is created before that point
	 * @var danXMLData_t::datablockConfig
	 * See @ref datablockMetadataWPV_t
	 */
	struct danXMLData_t{
		sourceConfig_t sourceConfig;
		nds::PVBaseOut sourcePVAux;

		std::string ndsDANPVName;

		std::vector<attrDataWPV_t> attrsList;

		datablockMetadataWPV_t datablockConfig;

		channelEnableDisableWPV_t chEnDisConfig;
	};

	/**
	 * @brief Stores the info needed for each nds-dan element in the XML
	 * @ingroup XMLParsing
	 */
	std::vector<danXMLData_t> m_danXMLData;
	//PARSING AND NODE CONFIGURATION////////////////////////////

	/**********************************************************
	 * SUB/UNSUB
	 **********************************************************/
	/**
	 * @brief Structure that holds the origin and destination of subscription for one NDS-PV
	 * @ingroup SUBUNSUB
	 */
	struct subRel_t{
		std::string pushFrom;
		std::string pushTo;
	};
	/**
	 * @brief Stores the information to subscribe or unsubscribe NDS-PVs
	 * @ingroup SUBUNSUB
	 */
	std::vector<subRel_t> m_subList;

	/**
	 * @brief Subscribes the NDS-PVs
	 * @ingroup SUBUNSUB
	 */
	void subscribePVs();

	/**
	 * @brief Unsubscribes the NDS-PVs
	 * @ingroup SUBUNSUB
	 */
	void unsubscribePVs();
	//END SUB/UNSUB//////////////////////////////////////

	/**********************************************************
	 * PARSING
	 **********************************************************/
	/**
	 * @brief Parses from an XML the information needed by @ref PluginNDSDAN
	 * @param path Path to the XML to parse. Can be relative.
	 * @return Vector containing the info for each nds-dan element in the XML
	 *
	 * @ingroup XMLParsing
	 */
	std::vector<danXMLData_t> parseXML(const std::string &path);

	/**
	 * @brief Parses the XML information related to an \<nds-dan\> elements needed for configure the NDS-DAN-Plugin and the NDS-DAN node
	 * @param ptreeNDSDAN ptree::value_type to the \<nds-dan\> element
	 * @param xmlData danXMLData_t to write the parsed information
	 *
	 * @ingroup XMLParsing
	 */
	void parseXMLConfig(const boost::property_tree::ptree::value_type& ptreeNDSDAN, danXMLData_t& xmlData);

	/**
	 * @brief Parses the XML information related to the \<attributes\> found in an \<nds-dan\> element
	 * @param ptreeNDSDAN ptree::value_type to the \<nds-dan\> element
	 * @param xmlData danXMLData_t to write the parsed information
	 *
	 * @ingroup XMLParsing
	 */
	void parseXMLAttr(const boost::property_tree::ptree::value_type& ptreeNDSDAN, danXMLData_t& xmlData);

	/**
	 * @brief Parses the XML information related to \<datablockHeader\> found in an \<nds-dan\> element
	 * @param ptreeNDSDAN ptree::value_type to the \<nds-dan\> element
	 * @param xmlData danXMLData_t to write the parsed information
	 *
	 * @ingroup XMLParsing
	 */
	void parseXMLDB(const boost::property_tree::ptree::value_type &ptreeNDSDAN, danXMLData_t &xmlData);

	/**
	 * @brief Parses the XML information related to \<ch-pv\> found in an \<nds-dan\> element
	 * @param ptreeNDSDAN ptree::value_type to the \<nds-dan\> element
	 * @param xmlData danXMLData_t to write the parsed information
	 *
	 * @ingroup XMLParsing
	 */
	void parseCHEnDis(const boost::property_tree::ptree::value_type &ptreeNDSDAN, danXMLData_t &xmlData);

	//END PARSING//////////////////////////////////////

	/**********************************************************
	 * DOUT FUNCTIONS
	 **********************************************************/
	/**
	 * @brief Function to be executed when a value is received through subscription to write to DAN
	 * @tparam T Scalar values supported by NDS-CORE
	 * @param timestamp Timespec
	 * @param value Value
	 * @param pvName Name of the NDS-PV that executed this function
	 *
	 * @ingroup NDS-DAN-CP
	 */
	template<typename T>
	typename std::enable_if<!is_vector<T>::value>::type
	func_dOutSourceCP(const timespec &timestamp, const T &value, const std::string &pvName);
	/**
	 * @brief Function to be executed when a value is received through subscription to write to DAN
	 * @tparam T Vector values supported by NDS-CORE
	 * @param timestamp Timespec
	 * @param value Value
	 * @param pvName Name of the NDS-PV that executed this function
	 *
	 * @ingroup NDS-DAN-CP
	 */
	template<typename T>
	typename std::enable_if<is_vector<T>::value>::type
	func_dOutSourceCP(const timespec &timestamp, const T &value, const std::string &pvName);

	/**
	 * @brief Function to be executed when a value is received through subscription to set/update an attribute of a DAN Source
	 * @tparam T Scalar values supported by NDS-CORE
	 * @param timestamp Timespec
	 * @param value Value of the new attribute
	 * @param pvName Name of the NDS-PV that executed this function
	 * @ingroup NDS-DAN-CP
	 */
	template<typename T>
	typename std::enable_if<!is_vector<T>::value>::type
	func_dOutAttrCP(const timespec &timestamp, const T &value, const std::string &pvName);
	/**
	 * @brief Function to be executed when a value is received through subscription to set/update an attribute of a DAN Source
	 * @tparam T Vector values supported by NDS-CORE
	 * @param timestamp Timespec
	 * @param value Value of the new attribute
	 * @param pvName Name of the NDS-PV that executed this function
	 * @ingroup NDS-DAN-ATTR
	 */
	template<typename T>
	typename std::enable_if<is_vector<T>::value>::type
	func_dOutAttrCP(const timespec &timestamp, const T &value, const std::string &pvName);

	/**
	 * @brief Function to be executed when a value is received through subscription to update the dimension datablock metadata. Can only be changed in RUNNING state.
	 * @tparam T Type of element in the vector containing the dim values. Can be int32, int16 or int8
	 * @param timestamp Timespec
	 * @param value	Vector size 2 with the new dimensions
	 * @param pvName Name of the NDS-PV that executed this function
	 * @ingroup NDS-DAN-DB
	 */
	template<typename T>
	void func_dOutDim(const timespec &timestamp, const std::vector<T> &value, const std::string &pvName);

	/**
	 * @brief Function to be executed when a value is received through subscription to update the sampling rate datablock metadata. Can only be changed in RUNNING state.
	 * @tparam T Can be double, float or int32
	 * @param timestamp Timespec
	 * @param value	Value with the new sampling rate
	 * @param pvName Name of the NDS-PV that executed this function
	 * @ingroup NDS-DAN-DB
	 */
	template<typename T>
	void func_dOutSampRate(const timespec &timestamp, const T &value, const std::string &pvName);

	/**
	 * @brief Function to be executed when a value is received through subscription to update the operationa mode datablock metadata. Can only be changed in RUNNING state.
	 * @tparam T Can be int32 or int64
	 * @param timestamp Timespec
	 * @param value	Value with the new operational mode
	 * @param pvName Name of the NDS-PV that executed this function
	 * @ingroup NDS-DAN-DB
	 */
	template<typename T>
	void func_dOutOpMode(const timespec &timestamp, const T &value, const std::string &pvName);

	/**
	 * @brief Function to be executed when a value is received through subscription to enable or disable channels. The operation is only allowed in ON state.
	 *
	 * Received data must be a vector and the data must be organized as follows:
	 * chNum0 - State0 - chNum1 - State1 ··· chNumN - StateN
	 *
	 * State is 0 for disable, anything else is enable.
	 *
	 * @tparam T Can be int8, int16 or int32
	 * @param timestamp Timespec
	 * @param value Vector containing the channels and the status of each of this channels
	 * @param pvName name of the NDS-PV that executed this function
	 *
	 * @ingroup NDS-DAN-CH
	 */
	template<typename T>
	void func_dOutChEnableDisable(const timespec &timestamp, const std::vector<T> &value, const std::string &pvName);

	//END DOUT FUNCTIONS//////////////////////////////////////

	/**********************************************************
	 * PV CREATION
	 **********************************************************/
	/**
	 * @brief It receives a vector containing the data needed for creating the PVs. It must be called before rootNode initialization
	 * @param danXMLData Vector containing the elements needed for configuring/creating the NDS-DAN nodes and their sources and attributes
	 *
	 * @ingroup PVCREATION
	 */
	void createPVs(std::vector<danXMLData_t> &danXMLData);

	/**
	 * @brief Creates a DelegateOut NDS-PV for a DAN Source
	 * @tparam T Type of the DelegateOut NDS-PV
	 * @param info Name of the NDS-PV
	 * @return A reference to the NDS-PV
	 *
	 * @ingroup PVCREATION
	 */
	template<typename T>
	nds::PVBaseOut createDOutSource(const std::string& info);
	/**
	 * @brief Creates a DelegateOut NDS-PV for a DAN attribute
	 * @tparam T Type of the DelegateOut NDS-PV
	 * @param info Name of the NDS-PV
	 * @return A reference to the NDS-PV
	 *
	 * @ingroup PVCREATION
	 */
	template<typename T>
	nds::PVBaseOut createDOutAttr(const std::string& info);

	/**
	 * @ingroup PVCREATION
	 */
	template<typename T>
	struct functorSourceCP;
	/**
	 * @ingroup PVCREATION
	 */
	template<typename T>
	struct functorAttribute;

	/**
	 * @ingroup PVCREATION
	 */
	template<template<typename> class TFunctor, class T>
	nds::PVBaseOut createDOut(const std::string& name);

	/**
	 * @ingroup PVCREATION
	 */
	template<template<typename> class TFunctor>
	nds::PVBaseOut selectTypeAndCreate(const nds::dataType_t& type, const std::string& name);

	/**
	 * @ingroup PVCREATION
	 */
	nds::PVBaseOut createDOutDim(const nds::dataType_t& type, const std::string& name);
	/**
	 * @ingroup PVCREATION
	 */
	nds::PVBaseOut createDOutSampRate(const nds::dataType_t& type, const std::string& name);
	/**
	 * @ingroup PVCREATION
	 */
	nds::PVBaseOut createDOutOpMode(const nds::dataType_t& type, const std::string& name);
	/**
	 * @ingroup PVCREATION
	 */
	nds::PVBaseOut createDOutChEnableDisable(const nds::dataType_t& type, const std::string& name);

	//END PV CREATION //////////////////////////////////////

	/**********************************************************
	 * NDS-DAN NODE CREATION AND CONFIGURATION
	 **********************************************************/

	/**
	 * @brief Creates the NDS-DAN nodes (Both CP and DA)
	 * @param danXMLData Vector with the info to create the NDS-DAN nodes and store them. Each element is an NDS-DAN node
	 * @param mapNodeDANCP Map on which the created ndsdan::DANCP nodes will be stored using as key the following: ICProgram + sourceName + ndsPVName
	 *
	 * @ingroup NDS-DAN-NODE
	 */
	void createDANNodes(std::vector<danXMLData_t> &danXMLData, std::map<std::string, ndsdan::DANCP>& mapNodeDANCP);
	/**
	 * @brief Configures NDS-DAN CP nodes
	 * @param danXMLData Vector with the information to configure the CP nodes, each element is an NDS-DAN CP node
	 *
	 * @ingroup NDS-DAN-CP
	 */
	void configDANCPNodes(std::vector<danXMLData_t> &danXMLData);

	//END NDS-DAN NODE CREATION AND CONFIGURATION///////////////


	/**********************************************************
	 * CONVERSION MAPS
	 **********************************************************/
	//@formatter:off
	struct CaseInsensitiveComparator {
		bool operator()(const std::string &a, const std::string &b) const noexcept {
			return ::strcasecmp(a.c_str(), b.c_str()) < 0;
		}
	};
	/**
	 * Map with the relationships between string and nds::dataType
	 */
	std::map<std::string, nds::dataType_t, CaseInsensitiveComparator> mapString2NDSType {
		{ "int32", nds::dataType_t::dataInt32 },
		{ "int64", nds::dataType_t::dataInt64 },
		{ "float32", nds::dataType_t::dataFloat32 },
		{ "float64", nds::dataType_t::dataFloat64 },
		{ "boolA", nds::dataType_t::dataBoolArray },
		{ "uint8A", nds::dataType_t::dataUint8Array },
		{ "uint16A", nds::dataType_t::dataUint16Array },
		{ "uint32A", nds::dataType_t::dataUint32Array },
		{ "int8A", nds::dataType_t::dataInt8Array },
		{ "int16A", nds::dataType_t::dataInt16Array },
		{ "int32A", nds::dataType_t::dataInt32Array },
		{ "int64A", nds::dataType_t::dataInt64Array },
		{ "float32A", nds::dataType_t::dataFloat32Array },
		{ "float64A", nds::dataType_t::dataFloat64Array },
		{ "string", nds::dataType_t::dataString },
		{ "timespec", nds::dataType_t::dataTimespec },
		{ "timespecA", nds::dataType_t::dataTimespecArray },
		{ "timestamp", nds::dataType_t::dataTimestamp }
	};

	std::map<std::string, size_t, CaseInsensitiveComparator> mapString2DANSize {
		{ "int8", sizeof(std::int8_t) },
		{ "int16", sizeof(std::int16_t) },
		{ "int32", sizeof(std::int32_t) },
		{ "int64", sizeof(std::int64_t) },
		{ "uint8", sizeof(std::uint8_t) },
		{ "uint16", sizeof(std::uint16_t) },
		{ "uint32", sizeof(std::uint32_t) },
		{ "float", sizeof(float) },
		{ "double", sizeof(double) }
	};

	/**
	 * Map with the sizes for nds::dataType_t
	 */
	std::map<nds::dataType_t, size_t> mapNDSType2SampleSize {
		{ nds::dataType_t::dataInt32, sizeof(std::int32_t) },
		{ nds::dataType_t::dataInt64, sizeof(std::int64_t) },
		{ nds::dataType_t::dataFloat32, sizeof(float) },
		{ nds::dataType_t::dataFloat64, sizeof(double) },
		{ nds::dataType_t::dataBoolArray, sizeof(bool) },
		{ nds::dataType_t::dataUint8Array, sizeof(std::uint8_t) },
		{ nds::dataType_t::dataUint16Array, sizeof(std::uint16_t) },
		{ nds::dataType_t::dataUint32Array,	sizeof(std::uint32_t) },
		{ nds::dataType_t::dataInt8Array, sizeof(std::int8_t) },
		{ nds::dataType_t::dataInt16Array, sizeof(std::int16_t) },
		{ nds::dataType_t::dataInt32Array, sizeof(std::int32_t) },
		{ nds::dataType_t::dataInt64Array, sizeof(std::int64_t) },
		{ nds::dataType_t::dataFloat32Array, sizeof(float) },
		{ nds::dataType_t::dataFloat64Array, sizeof(double) },
		{ nds::dataType_t::dataString, sizeof(char) }, //This is the size of each element
		{ nds::dataType_t::dataTimespec, sizeof(timespec) },
		{ nds::dataType_t::dataTimespecArray, sizeof(timespec) },
		{ nds::dataType_t::dataTimestamp, sizeof(nds::timestamp_t) }
	};

	std::map<nds::state_t, std::string> ndsState2String {
		{nds::state_t::fault, "FAULT"},
		{nds::state_t::initializing, "INITIALIZING"},
		{nds::state_t::off, "OFF"},
		{nds::state_t::on, "ON"},
		{nds::state_t::running, "RUNNING"},
		{nds::state_t::starting, "STARTING"},
		{nds::state_t::stopping, "STOPPING"},
		{nds::state_t::switchingOff, "SWITCHING OFF"}
	};
		//@formatter:on
	//CONVERSION MAPS////////////////////////////////////

	// Key: PV name DOut enable/disable channels
	std::map<std::string, std::string> m_mapCh;

	// Key: PV name DOut Attribute
	std::map<std::string, attrData_t> m_mapAttr;

	// Key: PV name DOut datablock metadata
	std::map<std::string, std::string> m_mapDB;

	// Key: PV name DOut Source
	std::map<std::string, sourceDataCP_t> m_mapSrcDANCP;

	/**
	 * @brief Root name of the device driver.
	 */
	std::string m_deviceName;
	/**
	 * @brief Root node of the device driver.
	 */
	nds::Port rootNode;

	/**
	 * @brief NDS Factory
	 */
	nds::Factory m_factory;

	/**
	 * @brief State machine
	 */
	nds::StateMachine m_stateMachine;
};

#endif  // NDS_DAN_PLUGIN_H_
