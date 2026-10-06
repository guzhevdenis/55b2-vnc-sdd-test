/**
 * @file nisyncFirmware.cpp
 *
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#include <functional>
#include <iostream>
#include <sys/time.h>
#include <string.h>

#include <nds3/nds.h>
#include <nisync-lib.h>

#include "nds3/nisyncDevice.h"
#include "nisyncFirmware.h"


nisyncFirmware::nisyncFirmware(const std::string& name, nds::Node& parent, nisyncDevice* device, nds::autoEnable_t autoEnable):
p_Device(device)
{
	/**
	 * This class will be a new child node of the given parent node, usually
	 * a class that holds the Root Node.
	 */
	nds::FirmwareArgs_t handlerFIRM(
			std::bind(&nisyncFirmware::switchOn_Firmware, this),
			std::bind(&nisyncFirmware::switchOff_Firmware, this),
			std::bind(&nisyncFirmware::start_Firmware, this),
			std::bind(&nisyncFirmware::stop_Firmware, this),
			std::bind(&nisyncFirmware::recover_Firmware, this),
			std::bind(&nisyncFirmware::allow_Firmware_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&nisyncFirmware::PV_Firmware_Path_Writer, this, std::placeholders::_1, std::placeholders::_2),
			autoEnable);

	m_Firmware = parent.addChild(nds::Firmware(name, 256, handlerFIRM));

}

nisyncFirmware::~nisyncFirmware() {
	/**
	 * The driver has to manage if the user stop incorrectly the NDS device driver. If we destroy the device driver
	 * without stopping the state machine some error are going to arise.
	 */
	ndsDebugStream(m_Firmware)<<"DESTROY"<<__func__<<std::endl;

	nds::state_t currentState;
	currentState=((nds::Node)m_Firmware).getState(); // getting the current state
	switch (currentState){
	case nds::state_t::running: // if we are in runnign we have to change to ON and later to  OFF
			m_Firmware.setState(nds::state_t::on);
			while (((currentState=((nds::Node)m_Firmware).getState()) != nds::state_t::on)){ // waiting until the STM has changed
				::usleep(1000);
			}

			m_Firmware.setState(nds::state_t::off);
			while (((currentState=((nds::Node)m_Firmware).getState()) != nds::state_t::off)){ // waiting until the STM has changed
				::usleep(1000);
			}
		break;
	case nds::state_t::on:
		m_Firmware.setState(nds::state_t::off);
		while (((currentState=((nds::Node)m_Firmware).getState()) != nds::state_t::off)){
			::usleep(1000);
		}
		break;
	default: //nothing to do in other important states
		break;
	}

}

void nisyncFirmware::switchOn_Firmware(){
	/*Call to HW to get a firmware description. */
	std::string errorStr="";
	if(p_Device==NULL){
		errorStr="RootNode pointer NULL.";
		p_Device->setError("nisyncFirmware","ERROR",errorStr);
		ndsErrorStream(m_Firmware)<<errorStr<<std::endl;
		throw nds::StateMachineError(errorStr);

	}else{
		nds::state_t parentState = p_Device->m_Device_stateMachine.getLocalState();
		if(parentState!=nds::state_t::initializing && parentState!=nds::state_t::on && parentState!=nds::state_t::starting && parentState!=nds::state_t::running){
			errorStr = "Parent node state should be ON. Returning State Machine to OFF State.";
			p_Device->setError("nisyncFirmware","WARNING",errorStr);
			throw nds::StateMachineRollBack(errorStr);
		}
		m_devData=p_Device->getDevData();
		if (nisync_get_device_info(m_devData.devfd, &device_info)) {
			errorStr = "Failed to obtain device info: "+ std::string(strerror(errno));
			p_Device->setError("nisyncFirmware","ERROR",errorStr);
			ndsErrorStream(m_Firmware)<<errorStr<<std::endl;
			throw nds::StateMachineRollBack(errorStr);
		}

		m_Firmware.setHardwareRevision(m_Firmware.getTimestamp(), std::to_string((long long int) device_info.revision));
		m_Firmware.setSerialNumber(m_Firmware.getTimestamp(), std::to_string((long long int) device_info.serial_number));
		m_Firmware.setDeviceModel(m_Firmware.getTimestamp(), device_info.model);
		m_Firmware.setDeviceType(m_Firmware.getTimestamp(), std::to_string((long long int) m_devData.devtype));
		m_Firmware.setDriverVersion(m_Firmware.getTimestamp(), device_info.driver_version);
		m_actualState = nds::state_t::on; //From OFF to ON state
	}
}

// Called when the Firmware node has to be switched off.
void nisyncFirmware::switchOff_Firmware(){
	m_actualState = nds::state_t::off; //From RUNNING to ON state
}

// Called when the Firmware node has to start working. We start the Firmware thread.
void nisyncFirmware::start_Firmware(){

	m_bStop_Firmware = false;
	/*
	 *  This thread only gets the status board. It could also provide the
	 *  installation process of a new firmware.
	 */
	m_Firmware_Thread =	m_Firmware.runInThread("Firmware",std::bind(&nisyncFirmware::Firmware_thread_body, this));
	m_actualState = nds::state_t::running; //From ON to RUNNING state

}

// Stop the Firmware node thread
void nisyncFirmware::stop_Firmware(){
	m_bStop_Firmware = true;
	m_Firmware_Thread.join();
	m_actualState = nds::state_t::on; //From RUNNING to ON state
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void nisyncFirmware::recover_Firmware(){
	throw nds::StateMachineRollBack("Cannot recover");
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool nisyncFirmware::allow_Firmware_Change(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState){
	return true;
}

void nisyncFirmware::PV_Firmware_Path_Writer(const timespec& /*timestamp*/, const std::string& /*value*/){

}

void nisyncFirmware::Firmware_thread_body(){

	nisync_status_t status;
	std::string errorStr="";
	while(!m_bStop_Firmware) {
		if (nisync_get_status(m_devData.devfd,&status)) {
			errorStr = "Impossible to obtain the board status "+ std::string(strerror(errno));
			p_Device->setError("nisyncFirmware","ERROR",errorStr);
			ndsErrorStream(m_Firmware)<<errorStr<<std::endl;
			//throw nds::NdsError(errorStr);
		}
		m_Firmware.setFirmwareStatus(m_Firmware.getTimestamp(),(std::int32_t)status);
		::usleep(10000);
	}
}

