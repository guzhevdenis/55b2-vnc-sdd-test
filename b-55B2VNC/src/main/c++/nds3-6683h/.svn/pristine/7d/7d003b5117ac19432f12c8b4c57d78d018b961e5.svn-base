/**
 * @file nisyncTiming.cpp
 *
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#include <functional>
#include <iostream>
#include <sys/time.h>

#include <nds3/nds.h>
#include <nisync-lib.h>

#include "nds3/nisyncDevice.h"
#include "nisyncTiming.h"


nisyncTiming::nisyncTiming(const std::string& name, nds::Node& parent,nisyncDevice* device, nds::autoEnable_t autoEnable):
p_Device(device)
/* member declarations */{


	/**
	 * This class will be a new child node of the given parent node, usually
	 * a class that holds the Root Node.
	 */
	m_Timing = parent.addChild(nds::Timing(name,
			std::bind(&nisyncTiming::switchOn_timing, this),
			std::bind(&nisyncTiming::switchOff_timing, this),
			std::bind(&nisyncTiming::start_timing, this),
			std::bind(&nisyncTiming::stop_timing, this),
			std::bind(&nisyncTiming::recover_timing, this),
			std::bind(&nisyncTiming::allow_timing_change, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
			std::bind(&nisyncTiming::pv_timing_reader, this,std::placeholders::_1,std::placeholders::_2),
			autoEnable));


	/**
	 * @brief Calling the  node constructor.
	 *
	 *   Adding  node to the control system. Note that in this case is
	 * added as a child of the parent node.
	 *
	 *   All the delegate methods and the state machine functions must be passed
	 * to the constructor of the node.
	 */
}



nisyncTiming::~nisyncTiming() {
	nds::state_t currentState;
	/**
	 * The driver has to manage if the user stop incorrectly the NDS device driver. If we destroy the device driver
	 * without stopping the state machine some error are going to arise.
	 */
	ndsDebugStream(m_Timing)<<"DESTROY"<<__func__<<std::endl;

    currentState=m_Timing.getState();
    switch(currentState){
    case nds::state_t::running:
    	m_Timing.setState(nds::state_t::on);
    	while ((currentState=m_Timing.getState()) != nds::state_t::on){ // waiting until the STM has changed
    		::usleep(1000);
    	}
    	m_Timing.setState(nds::state_t::off);
    	while ((currentState=m_Timing.getState()) != nds::state_t::off){ // waiting until the STM has changed
    	   	::usleep(1000);
    	}
    	break;
    case nds::state_t::on:
    	m_Timing.setState(nds::state_t::off);
    	while ((currentState=m_Timing.getState()) != nds::state_t::off){ // waiting until the STM has changed
    	   	::usleep(1000);
    	}
		break;
    default:
    	/**
    	 * Not necessary to do anything
    	 */
    	break;
    }

}


/*
 * Methods to control Timing state machine
 */

/**
 * Called when the Timing node has to be switched on
 * The driver is acquiring periodically the board time with an NDS thread
 */
void nisyncTiming::switchOn_timing(){
	std::string errorStr = "";
	if(p_Device==NULL){
		errorStr = "RootNode pointer NULL.";
		p_Device->setError("nisyncTiming","ERROR",errorStr);
		ndsErrorStream(m_Timing)<<errorStr<<std::endl;
		throw nds::StateMachineRollBack(errorStr);
	}else{

		nds::state_t parentState = p_Device->m_Device_stateMachine.getLocalState();
		if(parentState!=nds::state_t::initializing && parentState!=nds::state_t::on && parentState!=nds::state_t::starting && parentState!=nds::state_t::running){
			errorStr = "Parent node state should be ON. Returning State Machine to OFF State.";
			p_Device->setError("nisyncTiming","WARNING",errorStr);
			throw nds::StateMachineRollBack(errorStr);
		}

		//Get basic device information (devfd, devnum, devtype)
		m_devData=p_Device->getDevData();

		m_bStop_timing = false; //< We will set to true to stop the Timing thread
		/**
		 *   Start the Timing thread.
		 *   We don't need to check if the thread was already started because the state
		 *   machine guarantees that the start handler is called only while the state
		 *   is ON.
		 */
		m_timing_thread = m_Timing.runInThread("TM",std::bind(&nisyncTiming::timing_thread_body, this));
		m_actualState = nds::state_t::on;

	}
}

// Called when the Timing node has to be switched off.
void nisyncTiming::switchOff_timing(){
	m_bStop_timing = true;
	m_timing_thread.join();
	m_actualState = nds::state_t::off; //From stop to off state
}

// Called when the Timing node has to start working. We start the Timing thread.
void nisyncTiming::start_timing(){
	m_actualState = nds::state_t::running; //From on to running state
}

// Stop the Timing node thread
void nisyncTiming::stop_timing(){
	m_actualState = nds::state_t::on; //From running to on state
}

// A failure during a state transition will cause the state machine to switch to
// the failure state. For now we don't plan for this and every time the
// state machine wants to recover we throw StateMachineRollBack to force the
// state machine to stay on the failure state.
void nisyncTiming::recover_timing(){

	throw nds::StateMachineRollBack("Cannot recover");
}

// We always allow the state machine to switch state. Before calling this
// function the state machine has already verified that the requested state
// transition is legal.
bool nisyncTiming::allow_timing_change(const nds::state_t currentState,
		const nds::state_t currentGlobalState,
		const nds::state_t desiredState){
	return true;
}

void nisyncTiming::timing_thread_body() {

	timespec dummy_timestamp{0,0};

	uint64_t nanos;
	timespec time;

	while(!m_bStop_timing) {
		long int secsLastSync;
		if (nisync_get_device_info(m_devData.devfd, &device_info)) {
			p_Device->setError("nisyncTiming","ERROR","Error retrieving device info data."+ std::string(strerror(errno)));
			//throw nds::NdsError("Error retrieving device info data."+ std::string(strerror(errno)));
		}
		if (nisync_get_time_ns(m_devData.devfd,&nanos)) {
			/**
			 * Getting the PTP (TAI) time in ns
			 */
			p_Device->setError("nisyncTiming","ERROR","Failed to obtain device Time: "+ std::string(strerror(errno)));
			//throw nds::NdsError("Failed to obtain device Time: " + std::string(strerror(errno)));
		}
		//Caution device_info.time_since_sync is uint64_t vs SecsLastSync PV is int32_t (it comes from NDS2)
		secsLastSync = device_info.time_since_sync;
		/**
		 * Converting the TAI time to UTC using the helper library available in CODAC CORE SYSTEM 6.3.
		 * In CCS 6.1.2, this funtion is not available yet and the header file is included in this unit.
		 */
		ccs::types::AbsoluteTime utcTime = ccs::HelperTools::ConvertTAI2UTC(nanos);
		time.tv_sec=utcTime/NSEC_PER_SEC;
		time.tv_nsec=utcTime%NSEC_PER_SEC;

        dummy_timestamp=m_Timing.getTimestamp();
		m_Timing.setSecsLastSync(dummy_timestamp,secsLastSync);
		m_Timing.setSyncStatus(dummy_timestamp,device_info.sync_state);
		m_Timing.setTime(m_Timing.getTimestamp(),time); //Push the Time PV value to the control system

		usleep(100000); //10Hz
	}
}


void nisyncTiming::pv_timing_reader(timespec* timestamp, timespec* value){

	if(m_actualState==nds::state_t::on || m_actualState==nds::state_t::running ){

		uint64_t nanos;

		if (nisync_get_time_ns(m_devData.devfd,&nanos)) {
			p_Device->setError("nisyncTiming","ERROR","Failed to obtain device Time: "+ std::string(strerror(errno)));
			//throw nds::NdsError("Failed to obtain device Time: " + std::string(strerror(errno)));
		}

		timespec time;
		ccs::types::AbsoluteTime utcTime = ccs::HelperTools::ConvertTAI2UTC(nanos);
		time.tv_sec=utcTime/NSEC_PER_SEC;
		time.tv_nsec=utcTime%NSEC_PER_SEC;

		const char *format = "%Y-%m-%d T%T.%N";
		std::string textTime;
		const unsigned long int maxchars = 80;
		char data[maxchars];

		size_t fChars = strftime(data, maxchars, format, localtime(&time.tv_sec));

		if (fChars == 0) {
			p_Device->setError("nisyncTiming","ERROR","Insufficient space to write the whole time!");
			//throw nds::NdsError("Insufficient space to write the whole time!");
		}
		textTime.assign(data);
		std::stringstream stream;
		textTime = textTime.substr(0,textTime.find_last_of("%")); //Removing %N at the end of the string
		stream << textTime <<std::setfill('0')<<std::setw(9)<<time.tv_nsec; //Adding nanoseconds

		/// Data time in text format (%Y-%m-%d T%T.Nanoseconds).
		/// It is not published to EPICS because the timing node does not provide a
		/// setter for this PV. Instead of that, the own node publishes the Time
		/// with the next format:"Day Mon dd hh:mm:ss yyyy\n"
		//textTime = stream.str();

		//m_Timing.setTime(m_Timing.getTimestamp(),time); //Push the value to the control system
		*value=time; //Set the PVDelegateIn value pointer
		*timestamp=m_Timing.getTimestamp();
	}
	else{
		p_Device->setError("nisyncTiming","WARNING","Should turn ON the Timing node.");
		//throw nds::NdsError("Should turn ON the Timing node.");
	}
}

