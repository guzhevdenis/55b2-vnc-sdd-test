/**
 * @file
 *
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#include <functional>
#include <iostream>
#include <sys/time.h>

#include <nds3/nds.h>
#include "nds3/nisyncDevice.h"
#include "nisyncTerminal.h"
#include <poll.h>


nisyncTerminal::nisyncTerminal(const std::string& name, nds::Node& parent, nisync_terminal_t termNumber, nds::autoEnable_t autoEnable, nisyncDevice* device)
    : p_Device(device)
    , m_termNumber(termNumber)
    , m_name(name)
//    , m_tsEnabled(false) /* Initial number of timestamps is zero. */
	, m_enableFTE("Enable",std::bind(&nisyncTerminal::PV_FTE_Enable_Writer,this, std::placeholders::_1, std::placeholders::_2)
										  ,std::bind(&nisyncTerminal::PV_FTE_Enable_Initializer,this, std::placeholders::_1, std::placeholders::_2))
	, m_FTEEnabled(false)
	, m_checkFTELevels("checkFTELevels",std::bind(&nisyncTerminal::PV_FTE_checkFteLevels,this, std::placeholders::_1, std::placeholders::_2))
{

	m_nisyncTerminal = parent.addChild(nds::Node(name));

	m_terminal_stateMachine=m_nisyncTerminal.addChild(nds::StateMachine(false,
				std::bind(&nisyncTerminal::switchOn_terminal, this),
				std::bind(&nisyncTerminal::switchOff_terminal,this),
				std::bind(&nisyncTerminal::start_terminal,this),
				std::bind(&nisyncTerminal::stop_terminal,this),
				std::bind(&nisyncTerminal::recover_terminal,this),
				std::bind(&nisyncTerminal::allow_terminal_Change,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
				autoEnable
		));


	/**
	 * @brief Calling the Timestamping node constructor.
	 *
	 *   Adding Timestamping node to the control system. Note that in this case is
	 * added as a child of the parent node. See the exampleDrv cpp file for more
	 * information.
	 *
	 *   All the delegate methods and the state machine functions must be passed
	 * to the constructor of the node.
	 *
	 */
	nds::TimestampingArgs_t handlerTMS(
		std::bind(&nisyncTerminal::disable_terminal_state, this),
		std::bind(&nisyncTerminal::disable_terminal_state, this),
		std::bind(&nisyncTerminal::disable_terminal_state, this),
		std::bind(&nisyncTerminal::disable_terminal_state, this),
		std::bind(&nisyncTerminal::disable_terminal_state, this),
		std::bind(&nisyncTerminal::disable_terminal_Change, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),
		std::bind(&nisyncTerminal::pv_enable_writer, this,std::placeholders::_1, std::placeholders::_2),
		std::bind(&nisyncTerminal::pv_edge_writer, this,std::placeholders::_1, std::placeholders::_2),
		std::bind(&nisyncTerminal::pv_clearoverflow_writer, this,std::placeholders::_1, std::placeholders::_2)
	);
	handlerTMS.PV_Edge_Initializer = std::bind(&nisyncTerminal::pv_edge_initializer, this, std::placeholders::_1, std::placeholders::_2);

	m_timestamping = m_nisyncTerminal.addChild(nds::Timestamping<nds::timestamp_t>("TS", handlerTMS));
	m_decimationType = m_timestamping.addChild(nds::PVVariableOut<std::int32_t>("DecimationType"));

	//Debugging PV
	m_numberOfPushedTimestamps = m_timestamping.addChild(nds::PVVariableIn<std::int32_t>("NumberOfPushedTimestamps"));
	m_numberOfPushedTimestamps.setValue((std::int32_t)0);

	/*
	 * This class will be a new child node of the given parent node, usually
	 * a class that holds the Root Node.
	 */
	nds::FTEArgs_t handlerFTE(
			std::bind(&nisyncTerminal::disable_terminal_state, this),
			std::bind(&nisyncTerminal::disable_terminal_state, this),
			std::bind(&nisyncTerminal::disable_terminal_state, this),
			std::bind(&nisyncTerminal::disable_terminal_state, this),
			std::bind(&nisyncTerminal::disable_terminal_state, this),
			std::bind(&nisyncTerminal::disable_terminal_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&nisyncTerminal::PV_FTE_Set_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&nisyncTerminal::PV_FTE_Suppress_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&nisyncTerminal::PV_FTE_ChgPeriod_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&nisyncTerminal::PV_FTE_PendingValue_Writer,this, std::placeholders::_1, std::placeholders::_2)
	);
	handlerFTE.PV_ChgPeriod_Initializer= std::bind(&nisyncTerminal::PV_FTE_ChgPeriod_Initializer,this, std::placeholders::_1, std::placeholders::_2);
	handlerFTE.PV_PendingValue_Initializer = std::bind(&nisyncTerminal::PV_FTE_PendingValue_Initializer,this, std::placeholders::_1, std::placeholders::_2);
	handlerFTE.PV_Set_Initializer = std::bind(&nisyncTerminal::PV_FTE_Set_Initializer,this, std::placeholders::_1, std::placeholders::_2);
	handlerFTE.PV_Suppress_Initializer = std::bind(&nisyncTerminal::PV_FTE_Suppress_Initializer,this, std::placeholders::_1, std::placeholders::_2);

	m_FTE = m_nisyncTerminal.addChild(nds::FTE<std::string>("FTE",handlerFTE));


	m_FTE.addChild(m_checkFTELevels);
	m_FTE.addChild(m_enableFTE);
	m_enableFTE_RBV = m_FTE.addChild(nds::PVVariableIn<std::int32_t>("Enable_RBV"));
	m_enableFTE_RBV.setValue((std::int32_t)false);
	m_checkFTENumErrors = m_FTE.addChild(nds::PVVariableIn<std::int32_t>("checkFTENumErrors"));
	m_checkFTELevelsStatus = m_FTE.addChild(nds::PVVariableIn<std::string>("checkFTELevelsStatus"));
	m_checkFTELevelsCode = m_FTE.addChild(nds::PVVariableIn<std::int32_t>("checkFTELevelsCode"));

	//By default the decimation type will be by Software and no decimation
	m_decimationType.write(m_nisyncTerminal.getTimestamp(), 0);
	m_timestampConf.decCount = 1;
}



nisyncTerminal::~nisyncTerminal() {
	/**
	 * The driver has to manage if the user stop incorrectly the NDS device driver. If we destroy the device driver
	 * without stopping the state machine some error are going to arise.
	 */

	ndsDebugStream(m_nisyncTerminal)<<"DESTROY"<<__func__<<std::endl;

	nds::state_t currentState;
	currentState=m_terminal_stateMachine.getState();
	switch (currentState){
	case nds::state_t::on:
		m_terminal_stateMachine.setState(nds::state_t::off);
		while (((currentState=((nds::Node)m_terminal_stateMachine).getState()) != nds::state_t::off)){ // waiting until the STM has changed
				::usleep(1000);
		}
		break;
	case nds::state_t::running:
		m_terminal_stateMachine.setState(nds::state_t::on);
		while (((currentState=((nds::Node)m_terminal_stateMachine).getState()) != nds::state_t::on)){ // waiting until the STM has changed
				::usleep(1000);
		}
		m_terminal_stateMachine.setState(nds::state_t::off);
		while (((currentState=((nds::Node)m_terminal_stateMachine).getState()) != nds::state_t::off)){ // waiting until the STM has changed
				::usleep(1000);
		}
		break;
	default:
		//other states do nothing
		break;
	}
}
/**
 * Used to block implicit FTE and TS state machines as the terminal state machine controls the terminal.
 */
void nisyncTerminal::disable_terminal_state() {
	/*Do nothing. Disabled.*/
}

bool nisyncTerminal::disable_terminal_Change(const nds::state_t, const nds::state_t, const nds::state_t) {
	return false;
}

/**
 *   Terminal State machine methods.
 */

void nisyncTerminal::switchOn_terminal() {
	std::string errorStr;
	if(p_Device==NULL){
		errorStr= "RootNode pointer NULL.";
		p_Device->setError(m_name,"ERROR",errorStr);
		ndsErrorStream(m_nisyncTerminal)<<errorStr<<std::endl;
		throw nds::StateMachineRollBack(errorStr);
	}else{
		nds::state_t parentState = p_Device->m_Device_stateMachine.getLocalState();
		if(parentState != nds::state_t::on && parentState != nds::state_t::initializing){
			errorStr = "Parent node state should be ON or INITIALIZING to allow this transition.";
			p_Device->setError("nisync"+m_name,"WARNING",errorStr);
			throw nds::StateMachineRollBack(errorStr);
		}

		m_devData=p_Device->getDevData();
		m_termfd = nisync_open_terminal(m_devData.devtype,m_devData.devnum,m_termNumber,NISYNC_READ_BLOCKING);
		if (m_termfd == -1){
			errorStr= "Cannot open terminal "+ m_name +" :"+ std::string(strerror(errno));
			ndsErrorStream(m_nisyncTerminal)<<errorStr<<std::endl;
			throw nds::StateMachineRollBack(errorStr);
		}else if(m_devData.softReset){
			/**
			 * If SOFTRESET is set in the parameters, we abort clock, FTEs and disable FTE in the terminal
			 * to set the device in a known state
			 */
			if (nisync_abort_clock(m_termfd)) {
				if(errno!=EINVAL && errno!=EPERM){
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to suppress clock: " + std::string(strerror(errno)));
				}
			}
			if(nisync_abort_all_ftes(m_termfd)){
				if(errno!=EINVAL){
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to abort ftes in terminal: "+ std::string(strerror(errno)));
				}
			}
			if (nisync_disable_future_time_events(m_termfd)){
				if(errno!=EINVAL){
					m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to disable future time events: " + std::string(strerror(errno)));
				}
			}
		}
		m_timestamping.setMaxTimestamps(m_timestamping.getTimestamp(),m_tsLen); //Setting Maximum TS like NDS2
		m_FTE.setMaximum(m_FTE.getTimestamp(), m_tsLen-1);
	}
}

void nisyncTerminal::switchOff_terminal() {
	if (close(m_termfd) < 0) {
			std::string errorStr = "Cannot close terminal "+ m_name + " file descriptor: " +std::string(strerror(errno));
			p_Device->setError("nisync"+m_name,"ERROR",errorStr);
			ndsErrorStream(m_nisyncTerminal)<< errorStr<<std::endl;
			throw nds::StateMachineRollBack(errorStr);
		}
	m_termfd=-1;
}

void nisyncTerminal::start_terminal() {

	std::string errorStr;
	if (m_termfd < 0) {
		errorStr = "Device must be in ON state to perform this action!";
		ndsWarningStream(m_nisyncTerminal)<< errorStr<<std::endl;
		p_Device->setError(m_name,"WARNING",errorStr);
		throw nds::StateMachineRollBack(errorStr);
	}

	std::int32_t decimation=1;
	std::int32_t decimationType = m_decimationType.getValue();

	if(decimationType>0){
		decimation = m_timestamping.getDecimation();
		m_timestamping.setDecimation(m_timestamping.getTimestamp(), 1); //Clear PV Decimation if HW decimation is enabled
	}else{
		m_timestamping.setDecimation(m_timestamping.getTimestamp(), m_timestamping.getDecimation());
	}

	if (nisync_enable_timestamp_trigger(m_termfd, m_timestampConf.edge, decimation)) {
		p_Device->setError(m_name,"ERROR","Failed to enable timestamp triggering: " + std::string(strerror(errno)));
		ndsErrorStream(m_nisyncTerminal)<<"Failed to enable timestamp triggering: " + std::string(strerror(errno))<<std::endl;
		throw nds::StateMachineRollBack("Failed to enable timestamp triggering: " + std::string(strerror(errno)));
	}

	m_stop_timestamping= false; //< We will set to true to stop the Timing thread
	/**
	 *   Start the Timestamping thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_timestamping_thread = m_timestamping.runInThread(m_name,std::bind(&nisyncTerminal::timestamping_thread_body, this));

}

void nisyncTerminal::stop_terminal() {

	m_stop_timestamping = true;
	m_timestamping_thread.join();

	if (nisync_disable_timestamp_trigger(m_termfd)){
		p_Device->setError(m_name,"ERROR","Failed to disable timestamp triggering: " + std::string(strerror(errno)));
		ndsErrorStream(m_nisyncTerminal)<<"Failed to disable timestamp triggering: " + std::string(strerror(errno))<<std::endl;
		throw nds::StateMachineError("Failed to disable timestamp triggering: " + std::string(strerror(errno)));
	}
}

void nisyncTerminal::recover_terminal() {
	//throw nds::StateMachineRollBack("Cannot recover");
}

bool nisyncTerminal::allow_terminal_Change(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState) {
	return true;
}

/**
 * Timestamping methods.
 */

void nisyncTerminal::pv_enable_writer(const timespec& /*timestamp*/, const std::int32_t& /*value*/) {
	//Not need in this Timestamping implementation
}


void nisyncTerminal::pv_edge_writer(const timespec& /*timestamp*/, const std::int32_t& value) {

	if(m_terminal_stateMachine.getLocalState() == nds::state_t::on){

		if(value>=NISYNC_EDGE_INVALID || value<0){
			m_timestampConf.edge = NISYNC_EDGE_INVALID;
			m_timestamping.setEdge(m_timestamping.getTimestamp(),(std::int32_t)NISYNC_EDGE_INVALID); //RBV PV

		}else{
			m_timestampConf.edge = static_cast<nisync_edge_t>(value);
			/// Decimation Count is set to 1 because the decimation is been done by software
			/// in the ndsCore for the Timestamp PV instead of been done by hardware using
			/// the nisync driver
			m_timestampConf.decCount = 1;
			m_timestamping.setEdge(m_timestamping.getTimestamp(),value); //RBV PV
		}

	}else{
		terminalWarning("Should turn ON the terminal node.");
	}
}

void nisyncTerminal::pv_clearoverflow_writer(const timespec&  /*timestamp*/, const std::int32_t& /*value*/) {

	/// int nisync_get_status (int fd, nisync_status_t * status){}
	/// After device is initialized, it may encounter hardware or software runtime errors. Use this function to retrieve possible
	/// error condition. Clearing the error condition depends on the error itself. As a last resort a nisync_reset() function
	/// should clear all errors unless HW problem.

}


void nisyncTerminal::timestamping_thread_body() {


		nds::timestamp_t timestampToPush;
		nisync_timestamp_nanos_t array[m_tsLen];


		struct pollfd terminalfd;
		terminalfd.fd = m_termfd;
		terminalfd.events = POLLIN;

		int NumberOfPushedDataBlocks=0;
		while(!m_stop_timestamping) {

			int retval = poll(&terminalfd, 1, 10);
			if(retval<=0){
				//If retval is equal to 0 == timeout. Ignore this case and continue, just relevant error in Poll
				if(retval<0){
					terminalError("timestamping_thread_body: failed to poll terminals: " + std::string(strerror(errno)));
				}

			}else{
				if (terminalfd.revents & POLLIN) { ///Check if timestamps are available
					retval = nisync_read_timestamps_ns(m_termfd, array, m_tsLen);
					if (retval < 0 && errno == ENOENT) {
						terminalError("Failed to read "+ std::to_string((long long int)m_tsLen) + std::string(" samples from terminal: " + std::string(strerror(errno))));
					}else
					{
						// Call push for all the available data
						////////////////////////////////////////////////////////
						for (int scanData = 0; scanData < retval; scanData++) {
							timestampToPush.id = NumberOfPushedDataBlocks + scanData;
							timestampToPush.edge =array[scanData].edge;
							/**
							 * Implementing the conversion from TAI to UTC with the CCS helpers available in CCS 6.3
							 */
							ccs::types::AbsoluteTime utcTime = ccs::HelperTools::ConvertTAI2UTC(array[scanData].nanos);
							timestampToPush.timestamp.tv_sec= utcTime/NSEC_PER_SEC;
							timestampToPush.timestamp.tv_nsec= utcTime%NSEC_PER_SEC;

							m_timestamping.push(m_timestamping.getTimestamp(),(const nds::timestamp_t&)timestampToPush);
						}
						NumberOfPushedDataBlocks+=retval;

					}
					::usleep(1); //Same sleep as epics-nisync
				}
			}

		}
		m_numberOfPushedTimestamps.setValue(NumberOfPushedDataBlocks);
		m_numberOfPushedTimestamps.push(m_timestamping.getTimestamp(),NumberOfPushedDataBlocks);

}

/**
 *  Timestamping initalizers
 */
void nisyncTerminal::pv_edge_initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_nisyncTerminal.getTimestamp();
	*value = 2; //Any edge by default
	m_timestampConf.edge = static_cast<nisync_edge_t>(*value); //initial value

}

/* End Timestamping methods. */

/**
 * FTE methods.
 */

void nisyncTerminal::PV_FTE_Enable_Writer(const timespec& /*timestamp*/, const std::int32_t& value){

	if(m_terminal_stateMachine.getLocalState() == nds::state_t::on){
		if(value>=1){
			uint8_t enabled;
			if (nisync_future_time_events_configuration(m_termfd, &enabled)) {
				m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
				m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
				terminalError("Failed to obtain future time events status: " + std::string(strerror(errno)));
			}
			if (!enabled && nisync_enable_future_time_events(m_termfd)){
				m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
				m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
				terminalError("Failed enabling future time events: " + std::string(strerror(errno)));
			}else{
				m_FTEEnabled=true;
				m_enableFTE_RBV.setValue((std::int32_t)m_FTEEnabled);
				m_enableFTE_RBV.push(m_FTE.getTimestamp(),(std::int32_t)m_FTEEnabled);
			}
		}else{
			if (nisync_abort_clock(m_termfd)) {
				if(errno!=EINVAL && errno!=EPERM){
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to suppress clock: " + std::string(strerror(errno)));
				}
			}
			if(nisync_abort_all_ftes(m_termfd)){
				if(errno!=EINVAL){
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to abort ftes in terminal: "+ std::string(strerror(errno)));
				}
			}
			if (nisync_disable_future_time_events(m_termfd)){
				if(errno!=EINVAL){
					m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to disable future time events: " + std::string(strerror(errno)));
				}
			}
			m_FTEEnabled=false;
			m_enableFTE_RBV.setValue((std::int32_t)m_FTEEnabled);
			m_enableFTE_RBV.push(m_FTE.getTimestamp(),(std::int32_t)m_FTEEnabled);
		}
	}else{//Node state==OFF
		m_FTE.setSetStatus(m_FTE.getTimestamp(),"WARNING");
		m_FTE.setSetCode(m_FTE.getTimestamp(),1);
		terminalWarning("Should turn ON the terminal node.");
	}


}

void nisyncTerminal::PV_FTE_Set_Writer(const timespec& /*timestamp*/, const std::int32_t& value){

	nds::state_t actualState = m_terminal_stateMachine.getLocalState() ;
	if(actualState == nds::state_t::on && m_FTEEnabled==true){
		if(value == 1){//PV Set==1
			setMode_t modeSet = (setMode_t)m_FTE.getModeSet();
			nisync_level_t fteLevel;


			timespec startTimeSet;
			timespec endTimeSet;


			clk_t tempClk;
			ccs::types::AbsoluteTime utcTime;
			ccs::types::AbsoluteTime taiTime;

			switch(modeSet){
			case setMode_t::single:
				fteLevel = static_cast<nisync_level_t>(m_FTE.getLevelSet());
				startTimeSet = m_FTE.getStartTimeSet();
				/**
				 * Implementing the conversion from UTC to TAI with the CCS helpers available in CCS 6.3
				 */
				utcTime = ccs::HelperTools::FromFractionalToAbsoluteTime(startTimeSet.tv_sec, startTimeSet.tv_nsec);
				taiTime = ccs::HelperTools::ConvertUTC2TAI(utcTime);

				if (nisync_generate_fte_ns(m_termfd, (uint64_t)taiTime,fteLevel)) {
					m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to Set FTE: " + std::string(strerror(errno)));
				}
				m_FTE.setSetStatus(m_FTE.getTimestamp(),"OK");
				m_FTE.setSetCode(m_FTE.getTimestamp(),0);

				break;

			case setMode_t::pulse:
					uint64_t pulseWidth;
					uint64_t pulseStartTime;
					fteLevel = static_cast<nisync_level_t>(m_FTE.getLevelSet());
					startTimeSet = m_FTE.getStartTimeSet();
					endTimeSet = m_FTE.getStopTimeSet();
					if(((startTimeSet.tv_sec+startTimeSet.tv_nsec)!=0) && ((endTimeSet.tv_sec*1e9+endTimeSet.tv_nsec)>(startTimeSet.tv_sec*1e9+startTimeSet.tv_nsec))){
						/**
						 * Implementing the conversion from UTC to TAI with the CCS helpers available in CCS 6.3
						 */
						uint64_t pulseStopTime;
						utcTime = ccs::HelperTools::FromFractionalToAbsoluteTime(startTimeSet.tv_sec, startTimeSet.tv_nsec);
						pulseStartTime = ccs::HelperTools::ConvertUTC2TAI(utcTime);
						utcTime = ccs::HelperTools::FromFractionalToAbsoluteTime(endTimeSet.tv_sec, endTimeSet.tv_nsec);
						pulseStopTime = ccs::HelperTools::ConvertUTC2TAI(utcTime);
						pulseWidth = pulseStopTime - pulseStartTime;
					}else{
						pulseStartTime = 0;
						/**
						 * Implementing the conversion from UTC to TAI with the CCS helpers available in CCS 6.3
						 */

						utcTime = ccs::HelperTools::FromFractionalToAbsoluteTime(endTimeSet.tv_sec, endTimeSet.tv_nsec);
						pulseWidth = ccs::HelperTools::ConvertUTC2TAI(utcTime); //pulseDuration in ns
					}

					if (nisync_generate_pulse_ns(m_termfd,pulseStartTime,pulseWidth,fteLevel)) {
						m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
						m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
						terminalError("Failed to Set Pulse in terminal: " + std::string(strerror(errno)));
					}

					m_FTE.setSetStatus(m_FTE.getTimestamp(),"OK");
					m_FTE.setSetCode(m_FTE.getTimestamp(),0);
				break;

			case setMode_t::clk:
					startTimeSet = m_FTE.getStartTimeSet();
					/**
					 * Implementing the conversion from UTC to TAI with the CCS helpers available in CCS 6.3
					 */
					utcTime = ccs::HelperTools::FromFractionalToAbsoluteTime(startTimeSet.tv_sec, startTimeSet.tv_nsec);
					tempClk.startTime= ccs::HelperTools::ConvertUTC2TAI(utcTime); //startTime in ns
					endTimeSet = m_FTE.getStopTimeSet();
					utcTime = ccs::HelperTools::FromFractionalToAbsoluteTime(endTimeSet.tv_sec, endTimeSet.tv_nsec);
					tempClk.endTime= ccs::HelperTools::ConvertUTC2TAI(utcTime); //endTime in ns


					tempClk.period =m_FTE.getPeriodNsecSet();
					tempClk.dutyCycle =m_FTE.getDutyCycleSet();
					scheduledClock = tempClk;
					if(tempClk.endTime==0){
						tempClk.endTime = NISYNC_TIME_INDEFINITE_NANOS;
					}
					if (nisync_generate_clock_ns(m_termfd,tempClk.startTime,tempClk.endTime ,tempClk.period,tempClk.dutyCycle)) {
						m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
						m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
						terminalError("Failed to Set Clock in terminal: " + std::string(strerror(errno)));
					}

					m_FTE.setSetStatus(m_FTE.getTimestamp(),"OK");
					m_FTE.setSetCode(m_FTE.getTimestamp(),0);

				break;

			case setMode_t::inmLVL:
					fteLevel = static_cast<nisync_level_t>(m_FTE.getLevelSet());
					if (nisync_set_terminal_level(m_termfd,fteLevel)) {
						m_FTE.setSetStatus(m_FTE.getTimestamp(),"ERROR");
						m_FTE.setSetCode(m_FTE.getTimestamp(),-1);
						terminalError("Failed to set Immediate Terminal Level: " + std::string(strerror(errno)));
					}
					m_FTE.setSetStatus(m_FTE.getTimestamp(),"OK");
					m_FTE.setSetCode(m_FTE.getTimestamp(),0);
				break;

			default:
				m_FTE.setSetStatus(m_FTE.getTimestamp(),"WARNING");
				m_FTE.setSetCode(m_FTE.getTimestamp(),1);
				terminalWarning("No mode available for Set mode");
				break;
			}
		}else{ //PV Set==0
			m_FTE.setSetStatus(m_FTE.getTimestamp(),"WARNING");
			m_FTE.setSetCode(m_FTE.getTimestamp(),1);
			terminalWarning("SetPV value not equal to 1.");
		}

	}else{//Node state==OFF
		m_FTE.setSetStatus(m_FTE.getTimestamp(),"WARNING");
		m_FTE.setSetCode(m_FTE.getTimestamp(),1);
		terminalWarning("Should turn ON the terminal node and enable FTEs for the terminal.");
	}
}

void nisyncTerminal::PV_FTE_Suppress_Writer(const timespec& /*timestamp*/, const std::int32_t& value){

	if(m_terminal_stateMachine.getLocalState() == nds::state_t::on && m_FTEEnabled==true){
		if(value==1){
			int32_t suppressAll = m_FTE.getAllSuppress();
			if(suppressAll==1){
				if(nisync_abort_all_ftes(m_devData.devfd)){
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to abort all ftes in device: "+ std::string(strerror(errno)));
				}
				m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"OK");
				m_FTE.setSuppressCode(m_FTE.getTimestamp(),0);
			}else{

				suppressMode_t modeSuppress = (suppressMode_t)m_FTE.getModeSuppress();
				timespec startTimeSuppress = m_FTE.getStartTimeSuppress();

				/**
				 * Implementing the conversion from UTC to TAI with the CCS helpers available in CCS 6.3
				 */

				ccs::types::AbsoluteTime utcTime = ccs::HelperTools::FromFractionalToAbsoluteTime(startTimeSuppress.tv_sec, startTimeSuppress.tv_nsec);
				uint64_t fteTime = ccs::HelperTools::ConvertUTC2TAI(utcTime); //startTime in ns

				switch(modeSuppress){

				case suppressMode_t::single:
					if (nisync_abort_fte_ns(m_termfd,fteTime)) {
						m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"ERROR");
						m_FTE.setSuppressCode(m_FTE.getTimestamp(),-1);
						terminalError("Failed to abort fte: "+ std::string(strerror(errno)));
					}
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"OK");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),0);
					break;

				case suppressMode_t::clk:
					if (nisync_abort_clock(m_termfd)) {
						m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"ERROR");
						m_FTE.setSuppressCode(m_FTE.getTimestamp(),-1);
						terminalError("Failed to suppress clock: " + std::string(strerror(errno)));
					}
					scheduledClock=clk_t{0,0,0,0};
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"OK");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),0);
					break;

				default:
					m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"WARNING");
					m_FTE.setSuppressCode(m_FTE.getTimestamp(),1);
					terminalWarning("No mode available.");
					break;
				}
			}
		}else{
			m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"WARNING");
			m_FTE.setSuppressCode(m_FTE.getTimestamp(),1);
			terminalWarning("SuppressPV value not equal to 1.");
		}
	}else{
		m_FTE.setSuppressStatus(m_FTE.getTimestamp(),"WARNING");
		m_FTE.setSuppressCode(m_FTE.getTimestamp(),1);
		terminalWarning("Should turn ON the terminal node and enable FTEs for the terminal.");
	}
}

void nisyncTerminal::PV_FTE_ChgPeriod_Writer(const timespec& /*timestamp*/, const std::int32_t& value){

	if(m_terminal_stateMachine.getLocalState() == nds::state_t::on && m_FTEEnabled==true){
		if(value==1){
			clk_t tempClk;
			try{
				tempClk = scheduledClock;
				tempClk.period = m_FTE.getPeriodChgPeriod();

				if(nisync_abort_clock(m_termfd)){
					m_FTE.setChgPeriodStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setChgPeriodCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to change period Clock in terminal: " + std::string(strerror(errno)));
				}

				if (nisync_generate_clock_ns(m_termfd,NISYNC_TIME_IMMEDIATE_NANOS,tempClk.endTime ,tempClk.period,tempClk.dutyCycle)) {
					m_FTE.setChgPeriodStatus(m_FTE.getTimestamp(),"ERROR");
					m_FTE.setChgPeriodCode(m_FTE.getTimestamp(),-1);
					terminalError("Failed to change period Clock in terminal: " + std::string(strerror(errno)));
				}

				scheduledClock = tempClk;
				m_FTE.setChgPeriodStatus(m_FTE.getTimestamp(),"OK");
				m_FTE.setChgPeriodCode(m_FTE.getTimestamp(),0);
			}catch(const std::out_of_range &e){
				m_FTE.setChgPeriodStatus(m_FTE.getTimestamp(),"WARNING");
				m_FTE.setChgPeriodCode(m_FTE.getTimestamp(),1);
				terminalWarning("Not previously settled clock in the terminal.");
			}

		}else{
			m_FTE.setChgPeriodStatus(m_FTE.getTimestamp(),"WARNING");
			m_FTE.setChgPeriodCode(m_FTE.getTimestamp(),1);
			terminalWarning("ChangePV value not equal to 1.");
		}
	}else{
		m_FTE.setChgPeriodStatus(m_FTE.getTimestamp(),"WARNING");
		m_FTE.setChgPeriodCode(m_FTE.getTimestamp(),1);
		terminalWarning("Should turn ON the terminal node.");
	}

}

void nisyncTerminal::PV_FTE_PendingValue_Writer(const timespec& /*timestamp*/, const std::int32_t& value){

	if(m_terminal_stateMachine.getLocalState() >= nds::state_t::on && m_FTEEnabled==true){
		if(value==1){
			uint32_t numFtes;
			if (nisync_get_num_pending_ftes(m_termfd, &numFtes)) {
				m_FTE.setPendingStatus(m_FTE.getTimestamp(),"ERROR");
				m_FTE.setPendingCode(m_FTE.getTimestamp(),-1);
				terminalError("Failed to obtain number of pending FTEs: " + std::string(strerror(errno)));
			}
			m_FTE.setPendingValue(m_FTE.getTimestamp(),(int32_t)numFtes);
			m_FTE.setPendingStatus(m_FTE.getTimestamp(),"OK");
			m_FTE.setPendingCode(m_FTE.getTimestamp(),0);
		}else{
			m_FTE.setPendingStatus(m_FTE.getTimestamp(),"WARNING");
			m_FTE.setPendingCode(m_FTE.getTimestamp(),1);
			terminalWarning("PendingValuePV value not equal to 1.");

		}
	}else{
		m_FTE.setPendingStatus(m_FTE.getTimestamp(),"WARNING");
		m_FTE.setPendingCode(m_FTE.getTimestamp(),1);
		terminalWarning("Should turn ON the terminal node and enable FTEs for the terminal.");
	}
}


void nisyncTerminal::PV_FTE_checkFteLevels(const timespec& /*timestamp*/, const std::int32_t& value) {

	if(m_terminal_stateMachine.getLocalState() >= nds::state_t::on && m_FTEEnabled==true){
		if(value==1){
			std::uint32_t numErrs;
			if (nisync_check_fte_levels(m_termfd, &numErrs)) {
				m_checkFTELevelsStatus.setValue(m_FTE.getTimestamp(),"ERROR");
				m_checkFTELevelsStatus.push(m_FTE.getTimestamp(),std::string("ERROR"));
				m_checkFTELevelsCode.setValue(m_FTE.getTimestamp(),-1);
				m_checkFTELevelsCode.push(m_FTE.getTimestamp(),-1);
				terminalError("Failed to check FTE levels: " + std::string(strerror(errno)));
			}

			m_checkFTENumErrors.setValue(m_FTE.getTimestamp(),numErrs);
			m_checkFTENumErrors.push(m_FTE.getTimestamp(),(std::int32_t)numErrs);
			m_checkFTELevelsStatus.setValue(m_FTE.getTimestamp(),"OK");
			m_checkFTELevelsStatus.push(m_FTE.getTimestamp(),std::string("OK"));
			m_checkFTELevelsCode.setValue(m_FTE.getTimestamp(),0);
			m_checkFTELevelsCode.push(m_FTE.getTimestamp(),0);

		}else{
			m_checkFTELevelsStatus.setValue(m_FTE.getTimestamp(),std::string("WARNING"));
			m_checkFTELevelsStatus.push(m_FTE.getTimestamp(),std::string("WARNING"));
			m_checkFTELevelsCode.setValue(m_FTE.getTimestamp(),1);
			m_checkFTELevelsCode.push(m_FTE.getTimestamp(),1);
			terminalWarning("CheckFTEPV value not equal to 1.");
		}
	}else{
		m_checkFTELevelsStatus.setValue(m_FTE.getTimestamp(),std::string("WARNING"));
		m_checkFTELevelsStatus.push(m_FTE.getTimestamp(),std::string("WARNING"));
		m_checkFTELevelsCode.setValue(m_FTE.getTimestamp(),1);
		m_checkFTELevelsCode.push(m_FTE.getTimestamp(),1);
		terminalWarning("Should turn ON the terminal node and enable FTEs for the terminal.");
	}
}

void nisyncTerminal::PV_FTE_Enable_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_nisyncTerminal.getTimestamp();
	*value = 0;
}
void nisyncTerminal::PV_FTE_Set_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_nisyncTerminal.getTimestamp();
	*value = 0;
}
void nisyncTerminal::PV_FTE_Suppress_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_nisyncTerminal.getTimestamp();
	*value = 0;
}
void nisyncTerminal::PV_FTE_ChgPeriod_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_nisyncTerminal.getTimestamp();
	*value = 0;
}
void nisyncTerminal::PV_FTE_PendingValue_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_nisyncTerminal.getTimestamp();
	*value = 0;
}


/* End FTE methods. */


/*
 *  Helper Methods for cleaner errors and warnings handling.
 */
void nisyncTerminal::terminalError(std::string errorStr){
	p_Device->setError(m_name,"ERROR",errorStr);
	ndsErrorStream(m_nisyncTerminal)<<errorStr<<std::endl;
	//throw nds::NdsError(errorStr);
}

void nisyncTerminal::terminalWarning(std::string errorStr){
	p_Device->setError(m_name,"WARNING",errorStr);
	ndsWarningStream(m_nisyncTerminal)<<errorStr<<std::endl;
}
