//+======================================================================
// $HeadURL: https://svnpub.codac.iter.org/codac/iter/codac/dev/units/m-codac-unit-templates/branches/codac-core-6.3/templates/cpp/main/c++/lib/lib.cpp.template $
// $Id: lib.cpp.template 116150 2021-01-04 09:45:04Z kimh19 $
//
// Project       : CODAC Core System
//
// Description   : $progName program
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
#include <fstream>

#include <boost/algorithm/string/predicate.hpp>

#include <boost/filesystem.hpp>

#include "nds-dan-plugin.h"

#include <iostream>

static std::map<std::string, PluginNDSDAN*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

PluginNDSDAN::PluginNDSDAN(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t &parameters) :
		m_deviceName(deviceName), rootNode(deviceName), m_factory(factory) {
	{
		std::lock_guard<std::mutex> lock(m_lockDevicesMap);
		if (m_DevicesMap.find(m_deviceName) != m_DevicesMap.end()) {
			throw std::logic_error("Device with the same name already allocated. This should not happen");
		}
		m_DevicesMap[m_deviceName] = this;
	}
	try {
		std::string xmlFile;
		auto findParam = parameters.find("DAN_PLUGIN_XML");
		if (findParam == parameters.end()) {
			throw std::runtime_error(m_deviceName + ": DAN_PLUGIN_XML not defined.");
		}
		xmlFile = findParam->second;


		// Vector to store each nds-dan config/info in the XML file
		m_danXMLData = parseXML(xmlFile);

		createPVs(m_danXMLData);

		std::map<std::string, ndsdan::DANCP> mapNodeDANCP;
		createDANNodes(m_danXMLData, mapNodeDANCP);

		m_stateMachine = rootNode.addChild(
				nds::StateMachine( false, std::bind(&PluginNDSDAN::stm_SwitchOn, this),
						std::bind(&PluginNDSDAN::stm_SwitchOff, this), std::bind(&PluginNDSDAN::stm_Start, this),
						std::bind(&PluginNDSDAN::stm_Stop, this), std::bind(&PluginNDSDAN::stm_recover, this),
						std::bind(&PluginNDSDAN::stm_allowChange, this, std::placeholders::_1, std::placeholders::_2,
								std::placeholders::_3)));

		rootNode.initialize(this, factory);

		/**
		 * Now, the rootNode has been initialized, so the the full name of the PVs and Nodes added to it
		 * can be retrieved. Now the NDSDAN node would be linked using the PV Name as key in a map.
		 * This is required due to when the delegate out activates to know which NDSDAN node to use
		 * only has the name of the PV that activated it.
		 */
		for(auto&& elem: m_danXMLData){
			elem.ndsDANPVName = elem.sourcePVAux.getFullExternalName();

			//Source
			{
				sourceDataCP_t auxMap;
				auxMap.node = std::move(mapNodeDANCP.at(elem.sourceConfig.ICProgram + elem.sourceConfig.sourceName + elem.sourceConfig.ndsPVName));
				m_mapSrcDANCP.emplace(elem.ndsDANPVName, auxMap);
			}
			subRel_t subElem;
			subElem.pushFrom = elem.sourceConfig.ndsPVName;
			subElem.pushTo = elem.sourcePVAux.getFullName();
			m_subList.push_back(subElem);

			//Attributes
			for(auto&& attr: elem.attrsList){
				nds::PVBaseOut& attrPV = attr.ndsPVAux;
				attrData_t& attrData = attr.attrData;
				attrData.parentSource = elem.sourcePVAux.getFullExternalName();
				m_mapAttr.emplace(attrPV.getFullExternalName(), attrData);

				subElem.pushFrom = attrData.ndsPVName;
				subElem.pushTo = attrPV.getFullName();
				m_subList.push_back(subElem);
			}

			//Datablock metadata
			if(!elem.datablockConfig.datablockMetadata.ndsPVDim.empty()){
				std::string nameDANPV = elem.datablockConfig.ndsPVDimAux.getFullName();
				std::string nameDANPVExt = elem.datablockConfig.ndsPVDimAux.getFullExternalName();
				std::string nameOrigPV = elem.datablockConfig.datablockMetadata.ndsPVDim;
				m_mapDB.emplace(nameDANPVExt, elem.sourcePVAux.getFullExternalName());
				subElem.pushFrom = nameOrigPV;
				subElem.pushTo = nameDANPV;
				m_subList.push_back(subElem);
			}
			if (!elem.datablockConfig.datablockMetadata.ndsPVSampRate.empty()) {
				std::string nameDANPV = elem.datablockConfig.ndsPVSampRateAux.getFullName();
				std::string nameDANPVExt = elem.datablockConfig.ndsPVSampRateAux.getFullExternalName();
				std::string nameOrigPV = elem.datablockConfig.datablockMetadata.ndsPVSampRate;
				m_mapDB.emplace(nameDANPVExt, elem.sourcePVAux.getFullExternalName());
				subElem.pushFrom = nameOrigPV;
				subElem.pushTo = nameDANPV;
				m_subList.push_back(subElem);
			}
			if (!elem.datablockConfig.datablockMetadata.ndsPVOpMode.empty()) {
				std::string nameDANPV = elem.datablockConfig.ndsPVOpModeAux.getFullName();
				std::string nameDANPVExt = elem.datablockConfig.ndsPVOpModeAux.getFullExternalName();
				std::string nameOrigPV = elem.datablockConfig.datablockMetadata.ndsPVOpMode;
				m_mapDB.emplace(nameDANPVExt, elem.sourcePVAux.getFullExternalName());
				subElem.pushFrom = nameOrigPV;
				subElem.pushTo = nameDANPV;
				m_subList.push_back(subElem);
			}

			//Channels Enable Disable
			if(!elem.chEnDisConfig.channelEnableDisable.ndsPVName.empty()){
				std::string nameDANPV = elem.chEnDisConfig.ndsPVAux.getFullName();
				std::string nameDANPVExt = elem.chEnDisConfig.ndsPVAux.getFullExternalName();
				std::string nameOrigPV = elem.chEnDisConfig.channelEnableDisable.ndsPVName;
				m_mapCh.emplace(nameDANPVExt, elem.sourcePVAux.getFullExternalName());
				subElem.pushFrom = nameOrigPV;
				subElem.pushTo = nameDANPV;
				m_subList.push_back(subElem);
			}
		}

		configDANCPNodes(m_danXMLData);

#ifdef DEBUG
		rootNode.setLogLevel(nds::logLevel_t::debug);
#else
		rootNode.setLogLevel(nds::logLevel_t::warning);
#endif
	} catch (std::exception &e) {
		{
			std::lock_guard<std::mutex> lock(m_lockDevicesMap);
			// It is necessary to erase the device from the map. To free the name for further use
			m_DevicesMap.erase(m_deviceName);
		}

		throw std::runtime_error(e.what());
	}
}

PluginNDSDAN::~PluginNDSDAN() {
	const std::uint32_t MAX_TIMEOUT = m_mapSrcDANCP.size() * 30000; //30 seconds per each node
	nds::state_t curState = rootNode.getState();
	std::uint32_t timeout = 0;

	if(curState != nds::state_t::off && curState != nds::state_t::fault){
		std::cout << "Max time to wait until turn off: " << MAX_TIMEOUT/1000 << " seconds" << std::endl;
	}

	while(curState != nds::state_t::off && curState != nds::state_t::fault && timeout++ < MAX_TIMEOUT){
		switch(curState){
		case nds::state_t::on:
			rootNode.setState(nds::state_t::off);
			break;
		case nds::state_t::running:
			rootNode.setState(nds::state_t::on);
			break;
		default:
			//In the intermediate states keep waiting until one of the other states is reached
			break;
		}
		::usleep(1000);
		curState = rootNode.getState();
		//std::cout << "Current state: " << ndsState2String.at(curState) << std::endl;
	}

	std::lock_guard<std::mutex> lock(m_lockDevicesMap);
	// It is necessary to erase the device from the map. To free the name for further use
	m_DevicesMap.erase(m_deviceName);
	ndsDebugStream(rootNode) << __func__ << " (" + m_deviceName + ")" << std::endl;
}

/**********************************************************
 * STATE MACHINE
 **********************************************************/
void PluginNDSDAN::stm_SwitchOn() {
	ndsDebugStream(rootNode) << m_deviceName << ": SwitchOn" << std::endl;
	subscribePVs();
}

void PluginNDSDAN::stm_SwitchOff() {
	ndsDebugStream(rootNode) << m_deviceName << ": SwitchOff" << std::endl;
	unsubscribePVs();
}

void PluginNDSDAN::stm_Start() {
	ndsDebugStream(rootNode) << m_deviceName << ": Start" << std::endl;
}

void PluginNDSDAN::stm_Stop() {
	ndsDebugStream(rootNode) << m_deviceName << ": Stop" << std::endl;
}

void PluginNDSDAN::stm_recover() {
	throw nds::StateMachineRollBack("Cannot recover");
}

bool PluginNDSDAN::stm_allowChange(const nds::state_t currentState, const nds::state_t currentGlobalState,
		const nds::state_t desiredState) {
	return true;
}
////////////////////////////////////////////////////////////

/**********************************************************
 * PARSING
 **********************************************************/
void PluginNDSDAN::parseXMLDB(const boost::property_tree::ptree::value_type &ptreeNDSDAN, danXMLData_t &xmlData) {
	const auto &optDatablockXML = ptreeNDSDAN.second.get_child_optional("datablockHeader");
	//const auto &datablockXML = ptreeNDSDAN.second.equal_range("datablockHeader").first;

	if (optDatablockXML) {
		const auto &datablockXML = *optDatablockXML;

		xmlData.datablockConfig.datablockMetadata.ndsPVDim = datablockXML.get < std::string > ("dimensions.<xmlattr>.nds-pv", "");
		xmlData.datablockConfig.datablockMetadata.ndsPVSampRate = datablockXML.get < std::string
				> ("samplingRate.<xmlattr>.nds-pv", "");
		xmlData.datablockConfig.datablockMetadata.ndsPVOpMode = datablockXML.get < std::string > ("opMode.<xmlattr>.nds-pv", "");

		std::string strType;
		if (!xmlData.datablockConfig.datablockMetadata.ndsPVDim.empty()) {
			try {
				strType = datablockXML.get < std::string > ("dimensions.<xmlattr>.pv-type");
				xmlData.datablockConfig.datablockMetadata.ndsTypeDim = mapString2NDSType.at(strType);
			} catch (std::out_of_range&) {
				const std::string msg = "ERROR: " + strType + " is not a valid type for for dimensions parameter.";
				throw std::runtime_error(msg);
			}
		}
		if (!xmlData.datablockConfig.datablockMetadata.ndsPVSampRate.empty()) {
			try {
				strType = datablockXML.get < std::string > ("samplingRate.<xmlattr>.pv-type");
				xmlData.datablockConfig.datablockMetadata.ndsTypeSampRate = mapString2NDSType.at(strType);

			} catch (std::out_of_range&) {
				const std::string msg = "ERROR: " + strType + " is not a valid type for for samplingRate parameter.";
				throw std::runtime_error(msg);
			}
		}
		if (!xmlData.datablockConfig.datablockMetadata.ndsPVOpMode.empty()) {
			try {
				strType = datablockXML.get < std::string > ("opMode.<xmlattr>.pv-type");
				xmlData.datablockConfig.datablockMetadata.ndsTypeOpMode = mapString2NDSType.at(strType);
			} catch (std::out_of_range&) {
				const std::string msg = "ERROR: " + strType + " is not a valid type for for operationalMode parameter.";
				throw std::runtime_error(msg);
			}
		}
	}
}

void PluginNDSDAN::parseXMLAttr(const boost::property_tree::ptree::value_type& ptreeNDSDAN, danXMLData_t& xmlData){
	auto attributeRange = ptreeNDSDAN.second.equal_range("attribute");
	for (auto it = attributeRange.first; it != attributeRange.second; ++it) {
		attrData_t attrData;
		attrData.name = it->second.get<std::string>("<xmlattr>.name");
		attrData.units = it->second.get<std::string>("<xmlattr>.unit", "");
		attrData.description = it->second.get<std::string>("<xmlattr>.description", "");

		std::string strPVTypeAttr = it->second.get<std::string>("<xmlattr>.pv-type");
		try {
			auto ndsPVType = mapString2NDSType.at(strPVTypeAttr);
			attrData.ndsPVType = ndsPVType;
		} catch (std::out_of_range&) {
			const std::string msg = "ERROR: " + strPVTypeAttr + " is not a valid type for attribute " + attrData.name;
			throw std::runtime_error(msg);
		}
		if (attrData.ndsPVType == nds::dataType_t::dataTimespec
				|| attrData.ndsPVType == nds::dataType_t::dataTimespecArray
				|| attrData.ndsPVType == nds::dataType_t::dataTimestamp) {
			const std::string msg = strPVTypeAttr + " is not supported for attribute " + attrData.name
					+ " in " + xmlData.sourceConfig.ICProgram + "-" + xmlData.sourceConfig.sourceName;
			throw std::runtime_error(msg);
		}
		attrData.ndsPVName = it->second.get<std::string>("<xmlattr>.nds-pv");

		attrDataWPV_t auxAttr;
		auxAttr.attrData = attrData; //The NDS-PV will be set later
		xmlData.attrsList.push_back(auxAttr);
	}
}

void PluginNDSDAN::parseCHEnDis(const boost::property_tree::ptree::value_type &ptreeNDSDAN, danXMLData_t &xmlData) {
	xmlData.chEnDisConfig.channelEnableDisable.ndsPVName = ptreeNDSDAN.second.get < std::string > ("ch-pv.<xmlattr>.name", "");
	if(!xmlData.chEnDisConfig.channelEnableDisable.ndsPVName.empty()){
		std::string strPVType;
		try{
			strPVType = ptreeNDSDAN.second.get < std::string > ("ch-pv.<xmlattr>.type");
			xmlData.chEnDisConfig.channelEnableDisable.ndsType = mapString2NDSType.at(strPVType);
		}catch(std::out_of_range&){
			const std::string msg = "ERROR: " + strPVType + " is not a supported type in " + xmlData.sourceConfig.ICProgram + "-"
					+ xmlData.sourceConfig.sourceName;
			throw std::runtime_error(msg);
		}
	}
}

void PluginNDSDAN::parseXMLConfig(const boost::property_tree::ptree::value_type &ptreeNDSDAN, danXMLData_t &xmlData) {
	xmlData.sourceConfig.ICProgram = ptreeNDSDAN.second.get < std::string > ("ICProgram");
	xmlData.sourceConfig.sourceName = ptreeNDSDAN.second.get < std::string > ("sourceName");

	const std::string strPVType = ptreeNDSDAN.second.get < std::string > ("nds-pv.<xmlattr>.type");
	nds::dataType_t ndsPVType;
	try {
		ndsPVType = mapString2NDSType.at(strPVType);
	} catch (std::out_of_range&) {
		const std::string msg = "ERROR: " + strPVType + " is not a supported NDS PV type in " + xmlData.sourceConfig.ICProgram + "-"
				+ xmlData.sourceConfig.sourceName;
		throw std::runtime_error(msg);
	}
	if (ndsPVType == nds::dataType_t::dataString || ndsPVType == nds::dataType_t::dataTimespec
			|| ndsPVType == nds::dataType_t::dataTimespecArray || ndsPVType == nds::dataType_t::dataTimestamp) {
		const std::string msg = "ERROR: " + strPVType + " is not supported in " + xmlData.sourceConfig.ICProgram + "-"
				+ xmlData.sourceConfig.sourceName;
		throw std::runtime_error(msg);
	}

	const std::string strDANType = ptreeNDSDAN.second.get < std::string > ("danType");
	try {
		xmlData.sourceConfig.sampleDANSize = mapString2DANSize.at(strDANType);
	} catch (std::out_of_range&) {
		const std::string msg = "ERROR: " + strDANType + " is not a supported DAN type in " + xmlData.sourceConfig.ICProgram + "-"
				+ xmlData.sourceConfig.sourceName;
		throw std::runtime_error(msg);
	}

	//Config parse
	xmlData.sourceConfig.ndsPVType = ndsPVType;

	std::string strMethod = ptreeNDSDAN.second.get < std::string > ("<xmlattr>.method");
	if (boost::iequals(strMethod, "copy")) {
		xmlData.sourceConfig.method = methodDAN::CP;
	} else if (boost::iequals(strMethod, "da")) {
		xmlData.sourceConfig.method = methodDAN::DA;
	} else {
		const std::string msg = "ERROR: Method " + strMethod + " does not exists.";
		throw std::runtime_error(msg);
	}

	xmlData.sourceConfig.ndsPVName = ptreeNDSDAN.second.get < std::string > ("nds-pv.<xmlattr>.name");
	xmlData.sourceConfig.xmlPath = boost::filesystem::canonical(ptreeNDSDAN.second.get < std::string > ("xml")).string();
	xmlData.sourceConfig.blockNumberOfSamples = ptreeNDSDAN.second.get < std::int32_t > ("nds-pv.<xmlattr>.nelm", 1);
	xmlData.sourceConfig.sampleNDSSize = mapNDSType2SampleSize.at(xmlData.sourceConfig.ndsPVType);
	xmlData.sourceConfig.bufferSize = xmlData.sourceConfig.blockNumberOfSamples * xmlData.sourceConfig.sampleNDSSize
			* ptreeNDSDAN.second.get < size_t > ("nds-pv.<xmlattr>.nch", 1) * 10;
	xmlData.sourceConfig.samplingRate = ptreeNDSDAN.second.get < std::int32_t > ("samplingRate");
	//DANCP node configuration
	xmlData.sourceConfig.numBlocks = ptreeNDSDAN.second.get < std::int32_t > ("numBlocks", 1);

}

std::vector<PluginNDSDAN::danXMLData_t> PluginNDSDAN::parseXML(const std::string &path) {
	std::ifstream input(path);
	if (!input.good()) {
		std::string msg = "Error parsing XML. Could not open \"" + path + "\"";
		throw std::runtime_error(msg);
	}

	try {
		using boost::property_tree::ptree;
		ptree pt;
		read_xml(input, pt);

		std::vector<danXMLData_t> danXMLData;
		for (ptree::value_type const &xmlNDSDAN : pt.get_child("nds-dan-device")) {
			if (xmlNDSDAN.first == "nds-dan") {
				danXMLData_t xmlData;

				//NDSDAN Configuration
				parseXMLConfig(xmlNDSDAN, xmlData);

				//Attributes configuration
				parseXMLAttr(xmlNDSDAN, xmlData);

				//Datablock configuration
				parseXMLDB(xmlNDSDAN, xmlData);

				parseCHEnDis(xmlNDSDAN, xmlData);

				//Add to the rest of <nds-dan> elements
				danXMLData.push_back(xmlData);
			}
		}

		return danXMLData;
	}catch(std::exception& e){
		std::string msg = "Error parsing XML \""+ path + "\" Message: " + e.what();
		throw std::runtime_error(msg);
	}
}
////////////////////////////////////////////////////////////

/**********************************************************
 * DOUT FUNCTIONS
 **********************************************************/
template<typename T>
typename std::enable_if<!is_vector<T>::value>::type
PluginNDSDAN::func_dOutSourceCP(const timespec &timestamp, const T &value, const std::string &pvName) {
	//This only works for scalar types
	ndsDebugStream(rootNode) << "dOut called for " << pvName << std::endl;
	try{
		auto&& sourceCP = m_mapSrcDANCP.at(pvName);
		auto&& stateDANCP = sourceCP.node.getState();
		if(stateDANCP == nds::state_t::running){
			static_cast<T*>(sourceCP.ptr.get())[0] = value;
			std::uint64_t tBaseNs = timestamp.tv_sec * 1e9 + timestamp.tv_nsec;
			sourceCP.node.putDataBlock(tBaseNs);
			ndsDebugStream(rootNode) << pvName << ": Pushed Data Block" << std::endl;
		} else {
			ndsErrorStream(rootNode) << "Node needs to be running. Current value: " << ndsState2String.at(stateDANCP) << std::endl;
		}
	}catch(std::out_of_range&){
		ndsErrorStream(rootNode) << pvName << " not found in map CP" << std::endl;
	}
}
template<typename T>
typename std::enable_if<is_vector<T>::value>::type
PluginNDSDAN::func_dOutSourceCP(const timespec &timestamp, const T &value, const std::string &pvName) {
	ndsDebugStream(rootNode) << "dOut called for " << pvName << std::endl;

	try {
		auto &&sourceCP = m_mapSrcDANCP.at(pvName);
		auto &&stateDANCP = sourceCP.node.getState();
		if (stateDANCP == nds::state_t::running) {
			std::uint64_t tBaseNs = timestamp.tv_sec * 1e9 + timestamp.tv_nsec;
			auto ptr = static_cast<typename T::value_type*>(sourceCP.ptr.get());
			int bufferSizeSamples = (sourceCP.node.getBlockNumberOfSamples() * sourceCP.node.getNumberOfEnabledChannels());
			//std::copy(value.begin(), value.begin() + std::min((std::int32_t) value.size(), bufferSizeSamples), ptr);
			std::copy_n(value.begin(), std::min((std::int32_t) value.size(), bufferSizeSamples), ptr);
			sourceCP.node.putDataBlock(tBaseNs);
			ndsDebugStream(rootNode) << pvName << ": Pushed Data Block" << std::endl;
		} else {
			ndsErrorStream(rootNode) << "Node needs to be running. Current value: " << ndsState2String.at(stateDANCP) << std::endl;
		}
	} catch (std::out_of_range&) {
		ndsErrorStream(rootNode) << pvName << " not found in map CP" << std::endl;
	}
}

///string conversions
//std::string to_string(const nds::timestamp_t& t){
//	return std::string("id: ") + std::to_string(t.id) + " edge: " + std::to_string(t.edge) + " timestamp: "
//			+ std::to_string(t.timestamp.tv_sec) + "." + std::to_string(t.timestamp.tv_nsec);
//}
//
//std::string to_string(const timespec& t){
//	return t.tv_sec + "." + t.tv_nsec;
//}

std::string to_string(const std::string& t){
	return t;
}

template<typename T>
typename std::enable_if<is_vector<T>::value, std::string>::type to_string(const T& t){
	using std::to_string;
	using ::to_string;
	const std::string sep = "/";
	std::string ret = "";
	for(auto elem: t){
		ret += to_string(elem) + sep;
	}
	ret.pop_back(); //The last separator
	return ret;
}


template<typename T>
std::string danToString(T&& t){
	using std::to_string;
	using ::to_string;
	return to_string(std::forward<T>(t));
}
//////////////////////////

template<typename T>
typename std::enable_if<!is_vector<T>::value>::type
PluginNDSDAN::func_dOutAttrCP(const timespec &timestamp, const T &value, const std::string &pvName) {
	attrData_t attr;
	try {
		attr = m_mapAttr.at(pvName);
		ndsDebugStream(rootNode) << pvName << ": Changing attribute " << attr.name << std::endl;
	} catch (std::out_of_range&) {
		ndsErrorStream(rootNode) << pvName << " not found in attribute map" << std::endl;
	}

	try{
		sourceDataCP_t& sourceCP = m_mapSrcDANCP.at(attr.parentSource);
		auto success = sourceCP.node.setUserAttribute(attr.name, danToString(value), attr.units, attr.description);
		if(!success){
			ndsErrorStream(rootNode) << pvName << ": " << "Unable to update attribute " << attr.name << " in "
					<< sourceCP.node.getICProgramName() << " " << sourceCP.node.getSourceName() << std::endl;
		}
	} catch(std::out_of_range&){
		ndsErrorStream(rootNode) << pvName << ": " << attr.parentSource << " not found in CP map" << std::endl;
	}
}

template<typename T>
typename std::enable_if<is_vector<T>::value>::type
PluginNDSDAN::func_dOutAttrCP(const timespec &timestamp, const T &value, const std::string &pvName){
	attrData_t attr;
	try {
		attr = m_mapAttr.at(pvName);
		ndsDebugStream(rootNode) << pvName << ": Changing attribute " << attr.name << std::endl;
	} catch (std::out_of_range&) {
		ndsErrorStream(rootNode) << pvName << " not found in attribute map" << std::endl;
	}

	try{
		sourceDataCP_t& sourceCP = m_mapSrcDANCP.at(attr.parentSource);
		auto success = sourceCP.node.setUserAttribute(attr.name, danToString(value), attr.units, attr.description);
		if(!success){
			ndsErrorStream(rootNode) << pvName << ": " << "Unable to update attribute " << attr.name << " in "
					<< sourceCP.node.getICProgramName() << " " << sourceCP.node.getSourceName() << std::endl;
		}
	} catch(std::out_of_range&){
		ndsErrorStream(rootNode) << pvName << ": " << attr.parentSource << " not found in CP map" << std::endl;
	}
}

template<typename T>
void PluginNDSDAN::func_dOutDim(const timespec &timestamp, const std::vector<T> &value, const std::string &pvName){
	ndsDebugStream(rootNode) << "dOut dim called for " << pvName << std::endl;

	try {
		auto &&parentSource = m_mapDB.at(pvName);
		auto &&source = m_mapSrcDANCP.at(parentSource);

		auto &&stateDAN = source.node.getState();
		if (stateDAN == nds::state_t::running) {
			if (value.size() == 2) {
				source.node.setDataBlockHeaderDim(static_cast<std::int32_t>(value[0]), static_cast<std::int32_t>(value[1]));
				ndsDebugStream(rootNode) << "Updated dimensions for " << pvName << std::endl;
			} else {
				ndsErrorStream(rootNode) << "Size of received value is not two for dimensions" << std::endl;
			}
		} else {
			ndsErrorStream(rootNode) << "Node needs to be running to change dimensions. Current value: "
					<< ndsState2String.at(stateDAN) << " " << pvName << std::endl;
		}
	} catch (std::out_of_range&) {
		ndsErrorStream(rootNode) << pvName << " not found in map" << std::endl;
	}
}

template<typename T>
void PluginNDSDAN::func_dOutSampRate(const timespec &timestamp, const T &value, const std::string &pvName){
	ndsDebugStream(rootNode) << "dOut sampling rate called for " << pvName << std::endl;

	try {
		auto &&parentSource = m_mapDB.at(pvName);
		auto &&source = m_mapSrcDANCP.at(parentSource);

		auto &&stateDAN = source.node.getState();
		if (stateDAN == nds::state_t::running) {
			source.node.setDataBlockHeaderSampRate(static_cast<double>(value));
			ndsDebugStream(rootNode) << "Updated sampling rate for " << pvName << std::endl;
		} else {
			ndsErrorStream(rootNode) << "Node needs to be running to change sampling rate. Current value: "
					<< ndsState2String.at(stateDAN) << " " << pvName << std::endl;
		}
	} catch (std::out_of_range&) {
		ndsErrorStream(rootNode) << pvName << " not found in map" << std::endl;
	}
}

template<typename T>
void PluginNDSDAN::func_dOutOpMode(const timespec &timestamp, const T &value, const std::string &pvName){
	ndsDebugStream(rootNode) << "dOut operational mode called for " << pvName << std::endl;

	try {
		auto &&parentSource = m_mapDB.at(pvName);
		auto &&source = m_mapSrcDANCP.at(parentSource);

		auto &&stateDAN = source.node.getState();
		if (stateDAN == nds::state_t::running) {
			source.node.setDataBlockHeaderOpMode(static_cast<std::int64_t>(value));
			ndsDebugStream(rootNode) << "Updated operational mode for " << pvName << std::endl;
		} else {
			ndsErrorStream(rootNode) << "Node needs to be running to change operational mode. Current value: "
					<< ndsState2String.at(stateDAN) << " " << pvName << std::endl;
		}
	} catch (std::out_of_range&) {
		ndsErrorStream(rootNode) << pvName << " not found in map" << std::endl;
	}
}

template<typename T>
void PluginNDSDAN::func_dOutChEnableDisable(const timespec &timestamp, const std::vector<T> &value, const std::string &pvName){
	ndsDebugStream(rootNode) << "dOut channel enable disable called for " << pvName << std::endl;

	try {
		auto &&parentSource = m_mapCh.at(pvName);
		auto &&node = m_mapSrcDANCP.at(parentSource).node;

		auto &&stateDAN = node.getState();
		if (stateDAN == nds::state_t::on) {
			if (value.size() % 2 == 0 && value.size() != 0) {
				for(auto i = 0; i < value.size(); i+=2){
					//It can be done twice in a row because it has already been checked that the number of elements is multiple of 2
					auto &&numCh = static_cast<std::int32_t>(value.at(i));
					auto &&state = static_cast<bool>(value.at(i+1));
					bool ret;
					if (state) {
						ndsDebugStream(rootNode) << "Enabling channel " << numCh << " (" << pvName << ")" << std::endl;
						ret = node.setChannelEnable(numCh);
					} else {
						ndsDebugStream(rootNode) << "Disabling channel " << numCh << " (" << pvName << ")" << std::endl;
						ret = node.setChannelDisable(numCh);
					}

					if(!ret){
						ndsErrorStream(rootNode) << "ERROR: Unable to enable or disable channel " << numCh << " " << pvName << std::endl;
					}
				}
			} else {
				ndsErrorStream(rootNode) << "ERROR: Data received for channel enable or disable is not a multiple of 2 (Size:  "
						<< std::to_string(value.size()) << ") " << pvName << std::endl;
			}
		} else {
			ndsErrorStream(rootNode) << "ERROR: Node needs to be ON for enabling or disabling channels. Current value: "
					<< ndsState2String.at(stateDAN) << " " << pvName << std::endl;
		}

	} catch (std::out_of_range&) {
		ndsErrorStream(rootNode) << pvName << " not found in map" << std::endl;
	}
}

////////////////////////////////////////////////////////////


/**********************************************************
 * PV CREATION
 **********************************************************/

///DelegateOuts
template<typename T>
nds::PVBaseOut PluginNDSDAN::createDOutSource(const std::string& info){
	return rootNode.addChild(
			nds::PVDelegateOut<T>(info, NULL,
			std::bind(&PluginNDSDAN::func_dOutSourceCP<T>, this, std::placeholders::_1, // @suppress("Invalid arguments") Eclipse complains, but it works
					std::placeholders::_2, std::placeholders::_3)));
}
template<typename T>
nds::PVBaseOut PluginNDSDAN::createDOutAttr(const std::string& info){
	return rootNode.addChild(
			nds::PVDelegateOut<T>(info, NULL,
			std::bind(&PluginNDSDAN::func_dOutAttrCP<T>, this, std::placeholders::_1, // @suppress("Invalid arguments") Eclipse complains, but it works
					std::placeholders::_2, std::placeholders::_3)));
}

///Functors
template<typename T>
struct PluginNDSDAN::functorSourceCP{
	nds::PVBaseOut operator()(const std::string &info, PluginNDSDAN* ptrPlugin){
		return ptrPlugin->createDOutSource<T>(info);
	}
};
template<typename T>
struct PluginNDSDAN::functorAttribute{
	nds::PVBaseOut operator()(const std::string &info, PluginNDSDAN* ptrPlugin){
		return ptrPlugin->createDOutAttr<T>(info);
	}
};

template<template<typename> class TFunctor, class T>
nds::PVBaseOut PluginNDSDAN::createDOut(const std::string& name){
	return TFunctor<T>()(name, this);
}

template<template<typename> class TFunctor>
nds::PVBaseOut PluginNDSDAN::selectTypeAndCreate(const nds::dataType_t& type, const std::string& name){
	nds::PVBaseOut ret;

	switch (type) {
	case nds::dataType_t::dataBoolArray:
		ret = std::move(createDOut<TFunctor, std::vector<bool>>(name));
		break;
	case nds::dataType_t::dataFloat32:
		ret = std::move(createDOut<TFunctor, float>(name));
		break;
	case nds::dataType_t::dataFloat32Array:
		ret = std::move(createDOut<TFunctor, std::vector<float>>(name));
		break;
	case nds::dataType_t::dataFloat64:
		ret = std::move(createDOut<TFunctor, double>(name));
		break;
	case nds::dataType_t::dataFloat64Array:
		ret = std::move(createDOut<TFunctor, std::vector<double>>(name));
		break;
	case nds::dataType_t::dataInt16Array:
		ret = std::move(createDOut<TFunctor, std::vector<std::int16_t>>(name));
		break;
	case nds::dataType_t::dataInt32:
		ret = std::move(createDOut<TFunctor, std::int32_t>(name));
		break;
	case nds::dataType_t::dataInt32Array:
		ret = std::move(createDOut<TFunctor, std::vector<std::int32_t>>(name));
		break;
	case nds::dataType_t::dataInt64:
		ret = std::move(createDOut<TFunctor, std::int64_t>(name));
		break;
	case nds::dataType_t::dataInt64Array:
		ret = std::move(createDOut<TFunctor, std::vector<std::int64_t>>(name));
		break;
	case nds::dataType_t::dataInt8Array:
		ret = std::move(createDOut<TFunctor, std::vector<std::int8_t>>(name));
		break;
	case nds::dataType_t::dataString:
		ret = std::move(createDOut<TFunctor, std::string>(name));
		break;
//	case nds::dataType_t::dataTimespec:
//		ret = std::move(createDOut<TFunctor, timespec>(name));
//		break;
//	case nds::dataType_t::dataTimespecArray:
//		ret = std::move(createDOut<TFunctor, std::vector<timespec>>(name));
//		break;
//	case nds::dataType_t::dataTimestamp:
//		ret = std::move(createDOut<TFunctor, nds::timestamp_t>(name));
//		break;
	case nds::dataType_t::dataUint16Array:
		ret = std::move(createDOut<TFunctor, std::vector<std::uint16_t>>(name));
		break;
	case nds::dataType_t::dataUint32Array:
		ret = std::move(createDOut<TFunctor, std::vector<std::uint32_t>>(name));
		break;
	case nds::dataType_t::dataUint8Array:
		ret = std::move(createDOut<TFunctor, std::vector<std::uint8_t>>(name));
		break;
	default:
		break;
	}

	return ret;
}

nds::PVBaseOut PluginNDSDAN::createDOutDim(const nds::dataType_t& type, const std::string& name){
	switch(type){
	case nds::dataType_t::dataInt8Array:
		return rootNode.addChild(
				nds::PVDelegateOut<std::vector<std::int8_t>>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutDim<std::int8_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	case nds::dataType_t::dataInt16Array:
		return rootNode.addChild(
				nds::PVDelegateOut<std::vector<std::int16_t>>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutDim<std::int16_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	case nds::dataType_t::dataInt32Array:
		return rootNode.addChild(
				nds::PVDelegateOut<std::vector<std::int32_t>>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutDim<std::int32_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	default:
		const std::string msg = "ERROR: Not supported type for datablock dimensions " + name;
		throw std::runtime_error(msg);
	}
}
nds::PVBaseOut PluginNDSDAN::createDOutSampRate(const nds::dataType_t &type, const std::string &name) {
	switch (type) {
	case nds::dataType_t::dataInt32:
		return rootNode.addChild(
				nds::PVDelegateOut<std::int32_t>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutSampRate<std::int32_t>, this, std::placeholders::_1, std::placeholders::_2,
								std::placeholders::_3)));
		break;
	case nds::dataType_t::dataFloat32:
		return rootNode.addChild(
				nds::PVDelegateOut<float>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutSampRate<float>, this, std::placeholders::_1, std::placeholders::_2,
								std::placeholders::_3)));
		break;
	case nds::dataType_t::dataFloat64:
		return rootNode.addChild(
				nds::PVDelegateOut<double>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutSampRate<double>, this, std::placeholders::_1, std::placeholders::_2,
								std::placeholders::_3)));
		break;
	default:
		const std::string msg = "ERROR: Not supported type for datablock sampling rate " + name;
		throw std::runtime_error(msg);
	}
}
nds::PVBaseOut PluginNDSDAN::createDOutOpMode(const nds::dataType_t& type, const std::string& name){
	switch (type) {
	case nds::dataType_t::dataInt32:
		return rootNode.addChild(
				nds::PVDelegateOut<std::int32_t>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutOpMode<std::int32_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	case nds::dataType_t::dataInt64:
		return rootNode.addChild(
				nds::PVDelegateOut<std::int64_t>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutOpMode<std::int64_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	default:
		const std::string msg = "ERROR: Not supported type for datablock operational mode " + name;
		throw std::runtime_error(msg);
	}
}

nds::PVBaseOut PluginNDSDAN::createDOutChEnableDisable(const nds::dataType_t& type, const std::string& name){
	switch(type){
	case nds::dataType_t::dataInt8Array:
		return rootNode.addChild(
				nds::PVDelegateOut<std::vector<std::int8_t>>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutChEnableDisable<std::int8_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	case nds::dataType_t::dataInt16Array:
		return rootNode.addChild(
				nds::PVDelegateOut<std::vector<std::int16_t>>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutChEnableDisable<std::int16_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	case nds::dataType_t::dataInt32Array:
		return rootNode.addChild(
				nds::PVDelegateOut<std::vector<std::int32_t>>(name, NULL,
						std::bind(&PluginNDSDAN::func_dOutChEnableDisable<std::int32_t>, this, std::placeholders::_1,
								std::placeholders::_2, std::placeholders::_3)));
		break;
	default:
		const std::string msg = "ERROR: Not supported type for enabling or disabling channels " + name;
		throw std::runtime_error(msg);
	}
}

void PluginNDSDAN::createPVs(std::vector<danXMLData_t> &danXMLData) {
	for (auto &&elem : danXMLData) {
		std::string pvNameStrip = elem.sourceConfig.ndsPVName + "_DAN";
		std::replace(pvNameStrip.begin(), pvNameStrip.end(), '-', '_');

		/**
		 * While saving the reference to the PV is not needed,
		 * it is used later to retrieve the full name of the PV (including parent nodes)
		 * due to until the rootNode it is initialized
		 * the value returned by getFullExternalName() is incorrect
		 */
		switch (elem.sourceConfig.method) {
		case methodDAN::CP:
			elem.sourcePVAux = selectTypeAndCreate<functorSourceCP>(elem.sourceConfig.ndsPVType, pvNameStrip);
			break;
		case methodDAN::DA:
			throw(std::runtime_error("Method DA not implemented"));
			break;
		}

		//Attributes
		for (auto &&attr : elem.attrsList) {
			pvNameStrip = elem.sourceConfig.ICProgram + "_" + elem.sourceConfig.sourceName + "_" + attr.attrData.name + "_DAN";
			std::replace(pvNameStrip.begin(), pvNameStrip.end(), '-', '_');

			/**
			 * While saving the reference to the PV is not needed,
			 * it is used later to retrieve the full name of the PV (including parent nodes)
			 * due to until the rootNode it is initialized
			 * the value returned by getFullExternalName() is incorrect
			 */
			attr.ndsPVAux = selectTypeAndCreate<functorAttribute>(attr.attrData.ndsPVType, pvNameStrip);
		}

		//Datablock metadata
		if(!elem.datablockConfig.datablockMetadata.ndsPVDim.empty()){
			pvNameStrip = elem.sourceConfig.ICProgram + "_" + elem.sourceConfig.sourceName + "_DB_dim_DAN";
			std::replace(pvNameStrip.begin(), pvNameStrip.end(), '-', '_');
			elem.datablockConfig.ndsPVDimAux = createDOutDim(elem.datablockConfig.datablockMetadata.ndsTypeDim, pvNameStrip);
		}
		if (!elem.datablockConfig.datablockMetadata.ndsPVSampRate.empty()) {
			pvNameStrip = elem.sourceConfig.ICProgram + "_" + elem.sourceConfig.sourceName + "_DB_sampRate_DAN";
			std::replace(pvNameStrip.begin(), pvNameStrip.end(), '-', '_');
			elem.datablockConfig.ndsPVSampRateAux = createDOutSampRate(elem.datablockConfig.datablockMetadata.ndsTypeSampRate, pvNameStrip);
		}
		if (!elem.datablockConfig.datablockMetadata.ndsPVOpMode.empty()) {
			pvNameStrip = elem.sourceConfig.ICProgram + "_" + elem.sourceConfig.sourceName + "_DB_opMode_DAN";
			std::replace(pvNameStrip.begin(), pvNameStrip.end(), '-', '_');
			elem.datablockConfig.ndsPVOpModeAux = createDOutOpMode(elem.datablockConfig.datablockMetadata.ndsTypeOpMode, pvNameStrip);
		}

		//Enable/Disable channels
		if(!elem.chEnDisConfig.channelEnableDisable.ndsPVName.empty()){
			pvNameStrip = elem.sourceConfig.ICProgram + "_" + elem.sourceConfig.sourceName + "_chEnableDisable_DAN";
			std::replace(pvNameStrip.begin(), pvNameStrip.end(), '-', '_');
			elem.chEnDisConfig.ndsPVAux = createDOutChEnableDisable(elem.chEnDisConfig.channelEnableDisable.ndsType, pvNameStrip);
		}
	}
}
////////////////////////////////////////////////////////////


/**********************************************************
 * NDS-DAN NODE CREATION AND CONFIGURATION
 **********************************************************/
void PluginNDSDAN::createDANNodes(std::vector<danXMLData_t> &danXMLData, std::map<std::string, ndsdan::DANCP>& mapNodeDANCP){

	for (auto &&elem : danXMLData) {
		std::string ndsPVNameStrip = elem.sourceConfig.ndsPVName;
		std::replace(ndsPVNameStrip.begin(), ndsPVNameStrip.end(), '-', '_');

		switch (elem.sourceConfig.method) {
		case methodDAN::CP:
		{
			const std::string key = elem.sourceConfig.ICProgram + elem.sourceConfig.sourceName + elem.sourceConfig.ndsPVName;
			const std::string nameNode = "DANCP_" + elem.sourceConfig.ICProgram + elem.sourceConfig.sourceName;
			ndsdan::DANCP node = rootNode.addChild(
				ndsdan::DANCP(nameNode, [this, &elem](ndsdan::dataDAN_t *dataDAN) {
					void *ptr;
					try {
						this->m_mapSrcDANCP.at(elem.ndsDANPVName).ptr.reset(new std::int8_t[dataDAN->bufferSize]);
						ptr = this->m_mapSrcDANCP.at(elem.ndsDANPVName).ptr.get();
					} catch (std::out_of_range &e) {
						ptr = nullptr;
					}
					return ptr;
				},
				[this, &elem](void *dataStructure) {
					//delete[] static_cast<std::int8_t*>(this->m_mapSrcDANCP[elem.ndsDANPVName].ptr);
					return nullptr;
				},
				[](const ndsdan::dataDAN_t &dataDAN) {
					return nullptr;
				},
				nds::autoEnable_t::running));
				mapNodeDANCP.emplace(key, node);
			}
			break;
		case methodDAN::DA:
		{
			throw(std::runtime_error("Method DA not implemented"));
		}
			break;
		}
	}
}

void PluginNDSDAN::configDANCPNodes(std::vector<danXMLData_t> &danXMLData) {
	timespec timestamp = rootNode.getTimestamp();

	for(auto&& elem : danXMLData) {
		auto danCP = m_mapSrcDANCP.at(elem.ndsDANPVName);
		danCP.node.setSourceName(timestamp, elem.sourceConfig.sourceName);
		danCP.node.setICProgramName(timestamp, elem.sourceConfig.ICProgram);
		danCP.node.setSamplingRate(timestamp, elem.sourceConfig.samplingRate);
		danCP.node.setSampleSize(timestamp, elem.sourceConfig.sampleDANSize);
		danCP.node.setBlockNumberOfSamples(timestamp, elem.sourceConfig.blockNumberOfSamples);
		danCP.node.setBufferSize(timestamp, elem.sourceConfig.bufferSize);
		danCP.node.setPathXML(timestamp, elem.sourceConfig.xmlPath);
		danCP.node.setNumberOfBlocks(timestamp, elem.sourceConfig.numBlocks);
	}
}
////////////////////////////////////////////////////////////

/**********************************************************
 * SUB/UNSUB
 **********************************************************/
void PluginNDSDAN::subscribePVs() {
	ndsDebugStream(rootNode) << "Subscribing PVs..." << std::endl;
	for (auto&& pv : m_subList) {
		ndsDebugStream(rootNode) << "\tSubscribing " << pv.pushFrom << " to " << pv.pushTo << std::endl;
		m_factory.subscribe(pv.pushFrom, pv.pushTo);
	}
}

void PluginNDSDAN::unsubscribePVs() {
	ndsDebugStream(rootNode) << "Unsubscribing PVs..." << std::endl;
	for (auto pv : m_subList) {
		ndsDebugStream(rootNode) << "\tUnsubscribing " << pv.pushTo << std::endl;
		try{
			m_factory.unsubscribe(pv.pushTo);
		}catch(std::exception& e){
			ndsErrorStream(rootNode) << "\tError unsubscribing " << pv.pushTo << ": " << e.what() << std::endl;
		}
	}
}
////////////////////////////////////////////////////////////

//providing a name for the device driver. Here is the connection between the device driver name and the class supporting it
NDS_DEFINE_DRIVER(PluginNDSDAN, PluginNDSDAN)
