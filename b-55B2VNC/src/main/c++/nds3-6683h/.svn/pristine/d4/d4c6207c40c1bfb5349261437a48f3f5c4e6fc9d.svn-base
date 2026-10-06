/**
 * @file
 *
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#include <iostream>
#include <sstream>
#include <mutex>
#include <unistd.h>
#include <functional>
#include <vector>
#include <map>
#include <cstddef>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <nds3/nds.h>
#include <nisync-lib.h>

#include "nds3/nisyncDevice.h"
#include "nisyncFirmware.h"
#include "nisyncRouting.h"
#include "nisyncTiming.h"
#include "nisyncTerminal.h"



using namespace std::placeholders;
static std::map<std::string, nisyncDevice*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

nisyncDevice::nisyncDevice(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t& parameters)
    : rootNode(DeviceName)
    , m_name(DeviceName)
	, m_resetOn(false)
    , m_fd(-1)
    , m_devnum(0)
    , errDetect(false)
	, m_clearErrorMsg("ClearErrorMsg",std::bind(&nisyncDevice::PV_Device_clearErrorMessage, this, std::placeholders::_1, std::placeholders::_2))
//	, m_clearDeviceCLK("ClearDeviceCLK", std::bind(&nisyncDevice::PV_Device_clearDeviceCLK, this, std::placeholders::_1, std::placeholders::_2))
	, m_clearDeviceFTE("ClearDeviceFTE", std::bind(&nisyncDevice::PV_Device_clearDeviceFTE, this, std::placeholders::_1, std::placeholders::_2))
{
	{
		std::lock_guard<std::mutex> lock(m_lockDevicesMap);
		if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end())
		{
			throw std::logic_error("Device with the same name already allocated. This should not happen");
		}
		m_DevicesMap[DeviceName] = this;
	}

	m_findParam = parameters.find("BOARD_NUM");
	if(m_findParam!=parameters.end()){
		m_devnum = std::stoi(m_findParam->second);
	}

	nds::autoEnable_t autoEnable=nds::autoEnable_t::none;
	m_findParam = parameters.find("AUTOENABLE");
	if(m_findParam!=parameters.end()){
		if(boost::iequals(m_findParam->second, "off")){
			autoEnable=nds::autoEnable_t::off;
		}else if(boost::iequals(m_findParam->second, "on")){
			autoEnable=nds::autoEnable_t::on;
		}else if(boost::iequals(m_findParam->second, "running")){
			autoEnable=nds::autoEnable_t::running;
		}else{
//			ndsWarningStream(rootNode)<<"Incorrect autoEnable parameter set. The driver will run with autoEnable==off by default."<<std::endl;
		}
	}

	//By default the NISYNC device is not reset unless you pass the parameter RESET=YES
	m_devData.softReset = false;
	m_findParam = parameters.find("RESET");
	if(m_findParam!=parameters.end()){
		if(boost::iequals(m_findParam->second, "yes")){
			m_resetOn = true;
		}else if(boost::iequals(m_findParam->second, "soft")){
			m_devData.softReset = true;
		}
	}

	m_findParam = parameters.find("TERMINALS");
	//std::ostringstream trName;

	if(m_findParam!=parameters.end()){
		nameTerm(autoEnable);
	}else{
		//			setError(m_name, "ERROR", "Not defined the TERMINALS parameter);
	}

	m_Device_stateMachine=rootNode.addChild(nds::StateMachine(false,
			std::bind(&nisyncDevice::switchOn_Device, this),
			std::bind(&nisyncDevice::switchOff_Device,this),
			std::bind(&nisyncDevice::start_Device,this),
			std::bind(&nisyncDevice::stop_Device,this),
			std::bind(&nisyncDevice::recover_Device,this),
			std::bind(&nisyncDevice::allow_Device_Change,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)
	));

	m_Firmware_ptr.reset(new nisyncFirmware("Firm", rootNode,this,autoEnable));
	m_Routing_ptr.reset(new nisyncRouting("Routing", rootNode,this,autoEnable));
	m_Timing_ptr.reset(new nisyncTiming("Timing", rootNode,this,autoEnable));

	/*
	 * Initializing a vector of nisync Terminals. Note that a name is
	 * each TR(x) node has a unique name and FTE and TS nodes are its children.
	 * Also, the rootNode is passed so each terminal Node can be added as a child
	 * of the main driver class.
	 */


	m_errorSource = rootNode.addChild(nds::PVVariableIn<std::string>("ErrorSrc"));
	m_errorSeverity =rootNode.addChild(nds::PVVariableIn<std::int32_t>("ErrorSev"));
	m_errorMessage =rootNode.addChild(nds::PVVariableIn<std::string>("ErrorMsg"));
	rootNode.addChild(m_clearErrorMsg);
	rootNode.addChild(m_clearDeviceFTE);

#ifdef DEBUG
	rootNode.setLogLevel(nds::logLevel_t::debug);
#else
	rootNode.setLogLevel(nds::logLevel_t::error);
#endif
	rootNode.initialize(this,factory);

}

nisyncDevice::~nisyncDevice(){
	std::lock_guard<std::mutex> lock(m_lockDevicesMap);

	m_DevicesMap.erase(m_name);

	ndsDebugStream(rootNode)<<"DESTROY"<<__func__<<std::endl;
}

void nisyncDevice::switchOn_Device() {

}

void nisyncDevice::switchOff_Device() {

		if (close(m_fd) < 0) {
			std::string errorStr = "Cannot close device file descriptor: "+ std::string(strerror(errno));
			setError("rootNode","ERROR",errorStr );
			ndsErrorStream(rootNode)<<errorStr <<std::endl;
			throw nds::StateMachineError(errorStr );
		}
		m_fd = -1; //Setting device file descriptor to -1

//TODO Check getGlobalState() in nds-core. This function doesn't work as expected.
//	if(m_Device_stateMachine.getGlobalState()==nds::state_t::switchingOff){
//	}else{
//		setError("rootNode","WARNING","Must turn OFF all the Device childs before turn OFF the Device.");
//		throw nds::StateMachineRollBack("Must turn OFF all the Device childs before turn OFF the Device.");
//	}
}

void nisyncDevice::start_Device() {

	nisync_device_info m_device_info;
	memset(&m_device_info, 0x00, sizeof(m_device_info));
	int rc = nisync_get_device_info(m_fd, &(m_device_info));
	if (rc && errno != ENOMSG) {
		close(m_fd);
		std::string errorStr = "Cannot get device info: "+ std::string(strerror(errno));
		setError("rootNode","ERROR",errorStr);
		ndsErrorStream(rootNode)<<errorStr<<std::endl;
		throw nds::StateMachineRollBack(errorStr);
	}
}

void nisyncDevice::stop_Device() {
	if(m_Device_stateMachine.getGlobalState()!=nds::state_t::stopping){
		setError("rootNode","WARNING","Must STOP all the Device childs before stop the Device.");
		throw nds::StateMachineRollBack("Must STOP all the Device childs before stop the Device.");
	}
}

void nisyncDevice::recover_Device() {
}

bool nisyncDevice::allow_Device_Change(const nds::state_t localState, const nds::state_t, const nds::state_t newState) {

	/*TODO This is done due to hierarchical STM functionality. In fact, this is a temporary solution that has to be discussed.
	 * The following code must be placed in SwitchOn.
	 */
	if(localState==nds::state_t::off && newState==nds::state_t::on){

		//Putting the default values for Error PVs after reseting the board.
		std::string err="rootNode";
		m_errorSource.setValue(rootNode.getTimestamp(),err);
		m_errorSource.push(rootNode.getTimestamp(),err);
		err = "No error";
		m_errorMessage.setValue(rootNode.getTimestamp(),err);
		m_errorMessage.push(rootNode.getTimestamp(),err);

		m_errorSeverity.setValue(rootNode.getTimestamp(),0);
		m_errorSeverity.push(rootNode.getTimestamp(),0);

		std::string errorStr="";
		/* Open device */
		m_fd = nisync_open_device(NISYNC_DEVICE_TYPE_PXI6683H, m_devnum);
		if (m_fd < 0) {
			errorStr = "Cannot open device: "+ std::string(strerror(errno));
			setError("rootNode","ERROR",errorStr);
			ndsErrorStream(rootNode)<<errorStr<<std::endl;
			m_devData.devfd=m_fd;//error propagate to child nodes
			throw nds::StateMachineRollBack(errorStr);
		}else if (m_resetOn){
			//TODO consider reset behavior. https://jira.iter.org/browse/NDS-7
			if(nisync_reset(m_fd)){
				errorStr = "Cannot reset device: "+ std::string(strerror(errno));
				setError("rootNode","ERROR",errorStr);
				ndsErrorStream(rootNode)<<errorStr<<std::endl;
				throw nds::StateMachineRollBack(errorStr);
			}
		}else if(m_devData.softReset){
			/**
			 * If no reset is done, the device may still have configuration from previous executions,
			 * which may produce unwanted behavior or crashes. To avoid this, if the parameter SOFTRESET
			 * is set, the driver will check if any of the terminals is connected to another one and if so,
			 * disconnect them.
			 *
			 * This is needed because a full reset will also reset time and mess with PTP
			 */
			for(int termNum=0; termNum<=NISYNC_PXI_TRIG7; termNum++) {
				if ((termNum>= 0 &&  termNum < NISYNC_RTSI0 ) || (termNum > NISYNC_RTSI7  && termNum <= NISYNC_PXI_TRIG7)){
					nisync_terminal_t srcTerm;
					nisync_synchronous_t Sync;
					nisync_invert_t Inv;
					nisync_terminal_t dstTerm = (nisync_terminal_t)termNum;
					if(nisync_get_trigger_terminal_connection(m_fd, &srcTerm, &Sync, dstTerm, &Inv)==0){
						if(srcTerm != NISYNC_TERMINAL_INVALID){
							if(nisync_disconnect_trigger_terminals(m_fd, srcTerm, dstTerm)!=0){
								errorStr = "Failed to disconnect terminal "+std::to_string(termNum)+": "+ std::string(strerror(errno));
								setError("rootNode","ERROR",errorStr);
								ndsErrorStream(rootNode)<<errorStr<<std::endl;
							}
						}
					}
				}
			}
		}
		m_devData.devnum=m_devnum;
		m_devData.devfd=m_fd;
		m_devData.devtype=NISYNC_DEVICE_TYPE_PXI6683H;
	}
	return true;
}

devData_t nisyncDevice::getDevData(){
	return m_devData;
}

void nisyncDevice::PV_Device_clearErrorMessage(const timespec& /*timestamp*/, const std::int32_t& /*value*/){

	//Putting the default values for Error PVs after reseting the board.
	std::string err="rootNode";
	m_errorSource.setValue(rootNode.getTimestamp(),err);
	m_errorSource.push(rootNode.getTimestamp(),err);
	err = "No error";
	m_errorMessage.setValue(rootNode.getTimestamp(),err);
	m_errorMessage.push(rootNode.getTimestamp(),err);

	m_errorSeverity.setValue(rootNode.getTimestamp(),0);
	m_errorSeverity.push(rootNode.getTimestamp(),0);
}

void nisyncDevice::PV_Device_clearDeviceFTE(const timespec& /*timestamp*/, const std::int32_t& /*value*/){
	if(nisync_abort_all_ftes(m_fd)==-1){
		std::string msg = "Unable to abort all FTEs\n";
		setError("ERROR", "ERROR", msg);
	}
}

void nisyncDevice::setError(std::string source, const std::string& grade, std::string message){

	mtx.lock();
	if(grade=="WARNING" && !errDetect){
		m_errorMessage.setValue(m_errorMessage.getTimestamp(),message);
		m_errorMessage.push(m_errorMessage.getTimestamp(),message);
		m_errorSource.setValue(m_errorSource.getTimestamp(),source);
		m_errorSource.push(m_errorSource.getTimestamp(),source);
		m_error.source = source;
		m_error.grade = grade;
		m_error.message = message;
	}
	if(grade=="ERROR" && !errDetect){
		m_errorMessage.setValue(m_errorMessage.getTimestamp(),message);
		m_errorMessage.push(m_errorMessage.getTimestamp(),message);
		m_errorSource.setValue(m_errorSource.getTimestamp(),source);
		m_errorSource.push(m_errorSource.getTimestamp(),source);
		m_error.source = source;
		m_error.grade = grade;
		m_error.message = message;
		//errDetect=true;
	}

	mtx.unlock();
}

void nisyncDevice::nameTerm(const nds::autoEnable_t& autoEnable){
	int firstCh, lastCh;
	std::string trName;

	boost::char_separator<char> sep(".");
	boost::tokenizer<boost::char_separator<char> > chGr(m_findParam->second,sep);

	for(boost::tokenizer<boost::char_separator<char> >::iterator iter=chGr.begin(); iter!=chGr.end();++iter){
		boost::char_separator<char> sepChannel("-");
		boost::tokenizer<boost::char_separator<char> > tokens(*iter,sepChannel);

		for (boost::tokenizer<boost::char_separator<char>>::iterator iterChannel=tokens.begin(); iterChannel!=tokens.end(); ++iterChannel) {
			firstCh = std::stoi(*iterChannel);
			++iterChannel;
			if (iterChannel!=tokens.end()) {
				lastCh = std::stoi(*iterChannel);
				for(int chNum=firstCh; chNum<=lastCh; chNum++) {
					if ((chNum>= 0 &&  chNum < NISYNC_RTSI0 ) || (chNum > NISYNC_RTSI7  && chNum <= NISYNC_PXI_TRIG7)){
						trName =  "TR" + std::to_string(chNum);
						m_Terminal_vctr.push_back(std::unique_ptr<nisyncTerminal>(new nisyncTerminal(trName, rootNode,(nisync_terminal_t)chNum,autoEnable,this)));
					}else{
						//throw nds::NdsError("Incorrect number of channels MIN=0 MAX="+std::to_string(NISYNC_PXI_TRIG7)+". Got "+std::to_string(chNum));
//							ndsErrorStream(rootNode)<<"Incorrect number of channels MIN=0 MAX="<<NISYNC_PXI_TRIG7<<". Got "<<chNum<<std::endl;
					}
				}
			}
			else {
				if ((firstCh>= 0 &&  firstCh < NISYNC_RTSI0 ) || (firstCh > NISYNC_RTSI7  && firstCh <= NISYNC_PXI_TRIG7)){
					trName =  "TR" + std::to_string(firstCh);
					m_Terminal_vctr.push_back(std::unique_ptr<nisyncTerminal>(new nisyncTerminal(trName, rootNode,(nisync_terminal_t)firstCh,autoEnable,this)));
				}else{
//						ndsErrorStream(rootNode)<<"Incorrect number of channels MIN=0 MAX="<<NISYNC_PXI_TRIG7<<". Got "<<firstCh<<std::endl;
					//throw nds::NdsError("Incorrect number of channels MIN=0 MAX="+std::to_string(NISYNC_PXI_TRIG7)+". Got "+std::to_string(firstCh));
				}
				break;
			}
		}
	}
}

NDS_DEFINE_DRIVER(nisyncDevice, nisyncDevice)
