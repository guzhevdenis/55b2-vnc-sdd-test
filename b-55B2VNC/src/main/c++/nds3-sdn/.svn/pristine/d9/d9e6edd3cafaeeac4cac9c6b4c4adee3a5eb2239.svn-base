/**
 * @file DeviceTesting.cpp
 */

//#include <math.h>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "DeviceTesting.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */
#define S2NS 1000000000 //Seconds to nanoSeconds

DeviceTesting::DeviceTesting (nds::Factory& factory,
                                const std::string& deviceName,
                                const nds::namedParameters_t& namedParameters) :
      m_deviceName(deviceName),
	  m_stopPUB(true),
      rootNode(deviceName)
{

  m_stateMachine = rootNode.addChild(nds::StateMachine( false,
                  std::bind(&DeviceTesting::stm_SwitchOn, this),
                  std::bind(&DeviceTesting::stm_SwitchOff, this),
                  std::bind(&DeviceTesting::stm_Start, this),
                  std::bind(&DeviceTesting::stm_Stop, this),
                  std::bind(&DeviceTesting::stm_recover, this),
                  std::bind(&DeviceTesting::stm_allowChange, this,
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

//  nds::namedParameters_t::const_iterator findParam =  namedParameters.find("INIT");
//
//  if (findParam != namedParameters.end() && findParam->second=="YES") {
//   // initializePVs();
//  }

  nds::namedParameters_t::const_iterator findParam =  namedParameters.find("PV_CONF");
  if (findParam != namedParameters.end())  {
	  m_xmlPVConfig = parseXMLFile(findParam->second);
  }

  if(createPVs(m_xmlPVConfig)==true){
	  ndsErrorStream(m_stateMachine) <<"Error creating PVs in DeviceTesting device."<<std::endl;
  }else{
	  ndsDebugStream(m_stateMachine) <<"PVs created successfully..."<<std::endl;
  }

  //////////////////////////////////////////////////////////////////////////
  /// Auxiliary PVs
  //////////////////////////////////////////////////////////////////////////
  m_publishingRate_PV = rootNode.addChild(nds::PVVariableOut<std::int32_t>("PublishingRate"));
  m_publishingRate_PV.write(m_publishingRate_PV.getTimestamp(), 1);
  m_errStatus_PV = rootNode.addChild(nds::PVVariableIn<std::int32_t>("ErrorStatus"));
  m_errMsg_PV = rootNode.addChild(nds::PVVariableIn<std::string>("ErrorMessage"));

  //////////////////////////////////////////////////////////////////////////
  /// Initializing rootNode
  //////////////////////////////////////////////////////////////////////////
  rootNode.initialize(this, factory);
  rootNode.setLogLevel(nds::logLevel_t::debug);

}

DeviceTesting::~DeviceTesting ()
{


}

  void*
  DeviceTesting::allocateDevice (nds::Factory& factory,
                                  const std::string& deviceName,
                                  const nds::namedParameters_t& namedParameters)
  {
    return new DeviceTesting(factory, deviceName, namedParameters);
  }


  void
  DeviceTesting::deallocateDevice (void* deviceName)
  {
    delete (DeviceTesting*)deviceName;
  }

// #endif

void
DeviceTesting::stm_SwitchOn (void)
{
  ndsDebugStream(rootNode) << (m_deviceName + "-STM: Switching On" ) << std::endl;
}

void
DeviceTesting::stm_SwitchOff (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Switching Off" ) << std::endl;
}

void
DeviceTesting::stm_Start (void)
{
  const std::string threadName = m_deviceName + "-ACQ";
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Starting" ) << std::endl;
  m_stopPUB = false;
  m_threadPUB = rootNode.runInThread(threadName, std::bind(&DeviceTesting::publishingThread, this));
}

void
DeviceTesting::stm_Stop (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Stopping" ) << std::endl;
  m_stopPUB = true;
  m_threadPUB.join();
  ndsDebugStream(m_stateMachine) << "SDN-DAQ successfully stopped." << std::endl;
}

void
DeviceTesting::stm_recover (void)
{
  ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Recovering" ) << std::endl;
}

bool
DeviceTesting::stm_allowChange(const nds::state_t currentState, const nds::state_t, const nds::state_t desiredState)
{
  if (currentState == nds::state_t::off && desiredState == nds::state_t::running) {
      ndsErrorStream(m_stateMachine) << (m_deviceName + "-STM: State change not allowed until switching on the Device state machine" ) << std::endl;
      return false;
  } else {
      ndsDebugStream(m_stateMachine) << (m_deviceName + "-STM: Allowing change" ) << std::endl;
      return true;
  }
}


void DeviceTesting::publishingThread(void){

	std::int32_t rate = m_publishingRate_PV.getValue();
	ndsDebugStream(m_stateMachine) <<"Pushing rate: " <<rate<<std::endl;

	std::vector<bool> boolArray(10,false);
	std::vector<std::uint8_t> uint8Array(10,0);
	std::vector<std::uint16_t>uint16Array(10,0);
	std::vector<std::uint32_t>uint32Array(10,0);
	std::vector<std::int8_t> int8Array(10,0);
	std::vector<std::int16_t> int16Array(10,0);
	std::vector<std::int32_t> int32Array(10,0);
	std::vector<std::int64_t> int64Array(10,0);
	std::vector<float> floatArray(10,0.0);
	std::vector<double> doubleArray(10,0.0);
	timespec initTimespec;
	initTimespec.tv_nsec=0;
	initTimespec.tv_sec=0;
	std::vector<timespec> timespecArray(10,initTimespec);

	while (!m_stopPUB) {
		//INT32
        for (int32_PVs::iterator it = m_int32_PVs.begin(); it != m_int32_PVs.end();++it){
			it->second.push(rootNode.getTimestamp(), (std::int32_t)std::distance(m_int32_PVs.begin(), it));
		}
		//INT64
        for (int64_PVs::iterator it = m_int64_PVs.begin(); it != m_int64_PVs.end();++it){
			it->second.push(rootNode.getTimestamp(), (std::int64_t)std::distance(m_int64_PVs.begin(), it));
		}
		//Float
        for(float_PVs::iterator it = m_float_PVs.begin(); it != m_float_PVs.end();++it){
			it->second.push(rootNode.getTimestamp(), (float)std::distance(m_float_PVs.begin(), it));
		}
		//Double
        for(double_PVs::iterator it = m_double_PVs.begin(); it != m_double_PVs.end();++it){
			it->second.push(rootNode.getTimestamp(), (double)std::distance(m_double_PVs.begin(), it));
		}
		//Vector Bool
        for(boolArray_PVs::iterator it = m_boolArray_PVs.begin(); it != m_boolArray_PVs.end();++it){
			for(unsigned int pos =0; pos<std::distance(m_boolArray_PVs.begin(), it) && pos<boolArray.size();pos++){
				boolArray[pos] = !boolArray[pos];
			}
			it->second.push(rootNode.getTimestamp(), boolArray);
		}

		//Vector UINT8
        for(uint8Array_PVs::iterator it = m_uint8Array_PVs.begin(); it != m_uint8Array_PVs.end();++it){
			for(auto& v:uint8Array){
				v= std::distance(m_uint8Array_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), uint8Array);
		}
		//Vector UINT16
        for(uint16Array_PVs::iterator it = m_uint16Array_PVs.begin(); it != m_uint16Array_PVs.end();++it){
			for(auto& v:uint16Array){
				v= std::distance(m_uint16Array_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), uint16Array);
		}
		//Vector UINT32
        for(uint32Array_PVs::iterator it = m_uint32Array_PVs.begin(); it != m_uint32Array_PVs.end();++it){
			for(auto& v:uint32Array){
				v= std::distance(m_uint32Array_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), uint32Array);
		}

		//Vector INT8
        for(int8Array_PVs::iterator it = m_int8Array_PVs.begin(); it != m_int8Array_PVs.end();++it){
			for(auto& v:int8Array){
				v= std::distance(m_int8Array_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), int8Array);
		}
		//Vector INT16
        for(int16Array_PVs::iterator it = m_int16Array_PVs.begin(); it != m_int16Array_PVs.end();++it){
			for(auto& v:int16Array){
				v= std::distance(m_int16Array_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), int16Array);
		}
		//Vector INT32
        for(int32Array_PVs::iterator it = m_int32Array_PVs.begin(); it != m_int32Array_PVs.end();++it){
			for(auto& v:int32Array){
				v= std::distance(m_int32Array_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), int32Array);
		}
		//Vector INT64
        for(int64Array_PVs::iterator it = m_int64Array_PVs.begin(); it != m_int64Array_PVs.end();++it){
			for(auto& v:int64Array){
				v= std::distance(m_int64Array_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), int64Array);
		}

		//Vector Float
        for(floatArray_PVs::iterator it = m_floatArray_PVs.begin(); it != m_floatArray_PVs.end();++it){
			for(auto& v:floatArray){
				v= std::distance(m_floatArray_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), floatArray);
		}

		//Vector Double
        for(doubleArray_PVs::iterator it = m_doubleArray_PVs.begin(); it != m_doubleArray_PVs.end();++it){
			for(auto& v:doubleArray){
				v= std::distance(m_doubleArray_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), doubleArray);
		}

		//String
        for(string_PVs::iterator it = m_string_PVs.begin(); it != m_string_PVs.end();++it){
			it->second.push(rootNode.getTimestamp(), std::string("Testing-"+std::to_string(std::distance(m_string_PVs.begin(),it))));
		}

		//timespec
        for(timespec_PVs::iterator it = m_timespec_PVs.begin(); it != m_timespec_PVs.end();++it){
			timespec var;
			var.tv_nsec = var.tv_sec = std::distance(m_timespec_PVs.begin(),it);
			it->second.push(rootNode.getTimestamp(), var);
		}

		//timespecArray
        for(timespecArray_PVs::iterator it = m_timespecArray_PVs.begin(); it != m_timespecArray_PVs.end();++it){
			for(auto& v:timespecArray){
				v.tv_nsec=v.tv_sec=std::distance(m_timespecArray_PVs.begin(), it);
			}
			it->second.push(rootNode.getTimestamp(), timespecArray);
		}

		//nds::timestamp
        for(timestamp_PVs::iterator it = m_timestamp_PVs.begin(); it != m_timestamp_PVs.end();++it){
        	nds::timestamp_t var;
        	var.timestamp.tv_nsec = var.timestamp.tv_sec = var.id = std::distance(m_timestamp_PVs.begin(),it);
        	var.edge = true;
			it->second.push(rootNode.getTimestamp(), var);
		}

		sleep(rate);
  }

}

void
DeviceTesting::initializePVs(void)
{
  m_errStatus_PV.setValue(0);
  m_errMsg_PV.setValue("No error");
}

bool DeviceTesting::createPVs(  xmlPVConfig& inAtt )
{
	bool error=false;
	ndsDebugStream(rootNode)<<"======================================"<<std::endl;
	ndsDebugStream(rootNode)<<"Creating PVs to publish testing data..."<<std::endl;
	ndsDebugStream(rootNode)<<"======================================"<<std::endl;
	nds::dataType_t ndsDataType;

	for(xmlPVConfig::iterator pvFindParam = inAtt.begin();pvFindParam!=inAtt.end();++pvFindParam){

		if(!typeTodataType(pvFindParam->second.pvType,ndsDataType)){//Not error
			ndsDebugStream(rootNode)<<"PV found ["<<pvFindParam->second.pvName<<"] type: " <<pvFindParam->second.pvType<<" ("<<(int)ndsDataType<<")\n";
			switch(ndsDataType){
			case(nds::dataType_t::dataInt8Array):
				m_int8Array_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<int8_t>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<int8_t>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataInt16Array):
				m_int16Array_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<int16_t>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<int16_t>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataInt32):
				m_int32_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::int32_t>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::int32_t>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataInt64):
				m_int64_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::int64_t>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::int64_t>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataInt32Array):
				m_int32Array_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<std::int32_t>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<std::int32_t>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataInt64Array):
				m_int64Array_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<std::int64_t>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<std::int64_t>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataFloat32):
					m_float_PVs.insert(std::pair<std::string,nds::PVVariableIn<float>>(pvFindParam->second.pvName,
							rootNode.addChild(nds::PVVariableIn<float>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataFloat32Array):
				m_floatArray_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<float>>>(pvFindParam->second.pvName,
							rootNode.addChild(nds::PVVariableIn<std::vector<float>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataFloat64):
					m_double_PVs.insert(std::pair<std::string,nds::PVVariableIn<double>>(pvFindParam->second.pvName,
							rootNode.addChild(nds::PVVariableIn<double>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataFloat64Array):
					m_doubleArray_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<double>>>(pvFindParam->second.pvName,
							rootNode.addChild(nds::PVVariableIn<std::vector<double>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataBoolArray):
				m_boolArray_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<bool>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<bool>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataUint8Array):
				m_uint8Array_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<uint8_t>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<uint8_t>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataUint16Array):
				m_uint16Array_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<uint16_t>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<uint16_t>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataUint32Array):
				m_uint32Array_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<uint32_t>>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::vector<uint32_t>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataString):
				m_string_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::string>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<std::string>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataTimespec):
					m_timespec_PVs.insert(std::pair<std::string,nds::PVVariableIn<timespec>>(pvFindParam->second.pvName,
							rootNode.addChild(nds::PVVariableIn<timespec>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataTimespecArray):
					m_timespecArray_PVs.insert(std::pair<std::string,nds::PVVariableIn<std::vector<timespec>>>(pvFindParam->second.pvName,
							rootNode.addChild(nds::PVVariableIn<std::vector<timespec>>(pvFindParam->second.pvName))));
				break;
			case(nds::dataType_t::dataTimestamp):
				m_timestamp_PVs.insert(std::pair<std::string,nds::PVVariableIn<nds::timestamp_t>>(pvFindParam->second.pvName,
						rootNode.addChild(nds::PVVariableIn<nds::timestamp_t>(pvFindParam->second.pvName))));
				break;
			default:
				//Not supported datatype TODO nds-error
				break;
			}//End switch-case
		}
		else{//dataType error
			//Not supported datatype TODO nds-error
		}

	}//End For

	ndsDebugStream(rootNode)<<"======================================"<<std::endl<<std::endl;
	return error;
}



bool DeviceTesting::typeTodataType(std::string& typeStr,nds::dataType_t& ans){

	bool error= false;
	if(typeStr=="int32"){
		ans = nds::dataType_t::dataInt32;
	}else if(typeStr=="int32Array"){
		ans = nds::dataType_t::dataInt32Array;
	}else if(typeStr=="int64"){
		ans = nds::dataType_t::dataInt64;
	}else if(typeStr=="int64Array"){
		ans = nds::dataType_t::dataInt64Array;
	}else if(typeStr=="float"){
		ans = nds::dataType_t::dataFloat32;
	}else if(typeStr=="floatArray"){
		ans = nds::dataType_t::dataFloat32Array;
	}else if(typeStr=="double"){
		ans = nds::dataType_t::dataFloat64;
	}else if(typeStr=="doubleArray"){
		ans = nds::dataType_t::dataFloat64Array;
	}else if(typeStr=="boolArray"){
		ans = nds::dataType_t::dataBoolArray;
	}else if(typeStr=="uint8Array"){
		ans = nds::dataType_t::dataUint8Array;
	}else if(typeStr=="uint16Array"){
		ans = nds::dataType_t::dataUint16Array;
	}else if(typeStr=="uint32Array"){
		ans = nds::dataType_t::dataUint32Array;
	}else if(typeStr=="int8Array"){
		ans = nds::dataType_t::dataInt8Array;
	}else if(typeStr=="int16Array"){
		ans = nds::dataType_t::dataInt16Array;
	}else if(typeStr=="string"){
		ans = nds::dataType_t::dataString;
	}else if(typeStr=="timespec"){
		ans = nds::dataType_t::dataTimespec;
	}else if(typeStr=="timespecArray"){
		ans = nds::dataType_t::dataTimespecArray;
	}else if(typeStr=="timestamp"){
		ans = nds::dataType_t::dataTimestamp;
	}else {
		error = true;
	}

	return error;
}

xmlPVConfig DeviceTesting::parseXMLFile( const std::string &filename)
{

	ndsDebugStream(rootNode)<<"======================================"<<std::endl;
	ndsDebugStream(rootNode)<<"Parsing XML file"<<std::endl;
	ndsDebugStream(rootNode)<<"======================================"<<std::endl;

	std::ifstream input(filename);
	//m_SDNPUB.setPathXML(m_SDNPUB.getTimestamp(), filename);

	// populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(input, pt);

	// traverse pt
	xmlPVConfig ans;
	xmlPV_t pv;

	BOOST_FOREACH(ptree::value_type const&tree, pt.get_child("pv-config")) {
		if( tree.first == "pv" ) {
					try{
						pv.pvName = tree.second.get<std::string>("<xmlattr>.name");
						pv.pvType = tree.second.get<std::string>("<xmlattr>.datatype");
						ans.insert(std::pair<std::string,xmlPV_t>(pv.pvName,pv));
					}catch (std::exception &e)
					{
						ndsErrorStream(rootNode) << "ERROR: " << e.what() << "\n"; //TODO use nds-error
					}
					ndsDebugStream(rootNode)<<"PV name: "<<pv.pvName<<" dataType: "<< pv.pvType<<std::endl;
				}
	}

	ndsDebugStream(rootNode)<<"======================================"<<std::endl<<std::endl;
	return ans;
}



NDS_DEFINE_DRIVER(DeviceTesting, DeviceTesting)


