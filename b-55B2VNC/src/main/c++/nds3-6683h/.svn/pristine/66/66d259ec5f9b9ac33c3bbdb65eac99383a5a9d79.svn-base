/**
 * @file nisyncRouting.cpp
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

//#include "nds3/nisyncDevice.h"
#include "nisyncRouting.h"


nisyncRouting::nisyncRouting(const std::string& name, nds::Node& parent,nisyncDevice* device, nds::autoEnable_t autoEnable):
p_Device(device){


	nds::RoutingArgs_t handlerRTN(std::bind(&nisyncRouting::switchOn_Routing, this),
						std::bind(&nisyncRouting::switchOff_Routing, this),
						std::bind(&nisyncRouting::start_Routing, this),
						std::bind(&nisyncRouting::stop_Routing, this),
						std::bind(&nisyncRouting::recover_Routing, this),
						std::bind(&nisyncRouting::allow_Routing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
						std::bind(&nisyncRouting::PV_Routing_ClkSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
						std::bind(&nisyncRouting::PV_Routing_ClkDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2),
						std::bind(&nisyncRouting::PV_Routing_TermSet_Writer,this, std::placeholders::_1, std::placeholders::_2),
						std::bind(&nisyncRouting::PV_Routing_TermDstRead_Writer,this, std::placeholders::_1, std::placeholders::_2),
			autoEnable);
	handlerRTN.PV_ClkSet_Initializer = std::bind(&nisyncRouting::PV_Routing_ClkSet_Initializer,this, std::placeholders::_1, std::placeholders::_2);
	handlerRTN.PV_ClkDstRead_Initializer = std::bind(&nisyncRouting::PV_Routing_ClkDstRead_Initializer,this, std::placeholders::_1, std::placeholders::_2);
	handlerRTN.PV_TermSet_Initializer = std::bind(&nisyncRouting::PV_Routing_TermSet_Initializer,this, std::placeholders::_1, std::placeholders::_2);
	handlerRTN.PV_TermDstRead_Initializer = std::bind(&nisyncRouting::PV_Routing_TermDstRead_Initializer,this, std::placeholders::_1, std::placeholders::_2);

	/*
	 * This class will be a new child node of the given parent node, usually
	 * a class that holds the Root Node.
	 */
	m_Routing = parent.addChild(nds::Routing<std::string>(name, handlerRTN));

}

nisyncRouting::~nisyncRouting() {
	/**
	 * The driver has to manage if the user stop incorrectly the NDS device driver. This NDS node does not have a thread running.
	 * Nothing to do
	 */
	ndsDebugStream(m_Routing)<<"DESTROY"<<__func__<<std::endl;

}

//Methods to control Routing state machine
// Called when the Routing node has to be switched on.
void nisyncRouting::switchOn_Routing(){
	std::string errorStr = "";
	if(p_Device==NULL){
		errorStr = "RootNode pointer NULL.";
		p_Device->setError("nisyncRouting","ERROR",errorStr);
		ndsErrorStream(m_Routing)<<errorStr<<std::endl;
		throw nds::StateMachineError(errorStr);
	}else{
		nds::state_t parentState = p_Device->m_Device_stateMachine.getLocalState();
		if(parentState!=nds::state_t::initializing && parentState!=nds::state_t::on && parentState!=nds::state_t::starting && parentState!=nds::state_t::running){
			errorStr ="Parent node state should be ON. Returning State Machine to OFF State in Routing Node.";
			p_Device->setError("nisyncRouting","WARNING",errorStr);
			throw nds::StateMachineRollBack(errorStr);
		}
		//Get basic device information (devfd, devnum, devtype)
		m_devData=p_Device->getDevData();
		m_actualState = nds::state_t::on;
	}

}

// Called when the Routing node has to be switched off.
void nisyncRouting::switchOff_Routing(){
	m_actualState = nds::state_t::off;

}

// Called when the Routing node has to start working. We start the FTE thread.
void nisyncRouting::start_Routing(){
	m_actualState = nds::state_t::running;

}

// Stop the Routing node thread
void nisyncRouting::stop_Routing(){
	m_actualState = nds::state_t::on;

}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.

void nisyncRouting::recover_Routing(){
	throw nds::StateMachineRollBack("Cannot recover");
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.

bool nisyncRouting::allow_Routing_Change(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState){
	return true;
}


void nisyncRouting::PV_Routing_ClkSet_Writer(const timespec& timestamp, const std::int32_t& value){

	std::string errorStr;
	if(m_actualState==nds::state_t::on || m_actualState==nds::state_t::running ){

		if(value==1){

			//Retrieve the data from the PVs that have been previously configured
			nisync_clock_t clockSrc = static_cast<nisync_clock_t>(m_Routing.getClkSrc());
			nisync_clock_t clockDst = static_cast<nisync_clock_t>(m_Routing.getClkDst());
			std::string clkSrcStr = std::to_string((long long int)clockSrc);
			std::string clkDstStr = std::to_string((long long int)clockDst);


			if(clockSrc>=NISYNC_CLK_INVALID || clockDst>=NISYNC_CLK_INVALID){
				//m_Routing.setClkSetStatus(timestamp,"NISYNC_CLK_INVALID selected. ClkSrc: "+clkSrcStr+" ClkDst "+ clkDstStr);
				m_Routing.setClkSetStatus(timestamp,"WARNING");
				m_Routing.setClkSetCode(timestamp,1);
				errorStr="NISYNC_CLK_INVALID selected. ClkSrc: "+clkSrcStr+" ClkDst "+ clkDstStr;
				p_Device->setError("nisyncRouting","WARNING",errorStr);
				ndsWarningStream(m_Routing)<<errorStr<<std::endl;
			}
			else{
				if (nisync_connect_clk_terminals(m_devData.devfd,clockSrc, clockDst)) {
					//m_Routing.setClkSetStatus(timestamp,"Failed to connect clock terminals "+clkSrcStr+" -> "+ clkDstStr +" :"  + std::string(strerror(errno)));
					m_Routing.setClkSetStatus(timestamp,"ERROR");
					m_Routing.setClkSetCode(timestamp,-1);
					errorStr = "Failed to connect clock terminals "+clkSrcStr+" -> "+ clkDstStr +" :"  + std::string(strerror(errno));
					p_Device->setError("nisyncRouting","ERROR",errorStr);
					ndsErrorStream(m_Routing)<<errorStr<<std::endl;;
					//throw nds::NdsError(errorStr);
				}
				m_Routing.setClkSetStatus(timestamp,"OK");
				m_Routing.setClkSetCode(timestamp,0);
			}
		}else{
			//m_Routing.setClkSetStatus(timestamp,"ClkSet not equal to 1");
			m_Routing.setClkSetStatus(timestamp,"WARNING");
			m_Routing.setClkSetCode(timestamp,1);
			errorStr = "ClkSet not equal to 1";
			p_Device->setError("nisyncRouting","WARNING",errorStr);
			ndsWarningStream(m_Routing)<<errorStr<<std::endl;
		}
	}
	else{
		m_Routing.setClkSetStatus(timestamp,"WARNING");
		m_Routing.setClkSetCode(timestamp,1);
		errorStr = "Should turn ON the Routing node.";
		p_Device->setError("nisyncRouting","WARNING",errorStr);
		ndsWarningStream(m_Routing)<<errorStr<<std::endl;
	}

}

void nisyncRouting::PV_Routing_ClkDstRead_Writer(const timespec& timestamp, const std::int32_t& value){

	std::string errorStr;
	if(m_actualState==nds::state_t::on || m_actualState==nds::state_t::running ){
		nisync_clock_t clockSrc;
		nisync_clock_t clockDst = static_cast<nisync_clock_t>(value);
		std::string clkDstStr = std::to_string((long long int)clockDst);

		if(clockDst!=NISYNC_CLK_INVALID){
			if (nisync_get_clk_source_terminal(m_devData.devfd,&clockSrc, clockDst)) {
				errorStr="Failed to obtain source for clock "+clkDstStr+": " + std::string(strerror(errno));
				p_Device->setError("nisyncRouting","ERROR",errorStr);
				ndsErrorStream(m_Routing)<<errorStr<<std::endl;;
				//throw nds::NdsError(errorStr);
			}
			m_Routing.setClkSrcRead(timestamp, clockSrc);

		}else{
			errorStr = "Invalid destination clock selected: "+clkDstStr;
			p_Device->setError("nisyncRouting","WARNING",errorStr);
			ndsWarningStream(m_Routing)<<errorStr<<std::endl;
		}

	}else{
		errorStr = "Should turn ON the Routing node.";
		p_Device->setError("nisyncRouting","WARNING",errorStr);
		ndsWarningStream(m_Routing)<<errorStr<<std::endl;
	}
}

void nisyncRouting::PV_Routing_TermSet_Writer(const timespec& timestamp, const std::int32_t& value){

	std::string errorStr;
	if(m_actualState==nds::state_t::on || m_actualState==nds::state_t::running ){

		if (value==1){
			connection_t conn;
			connection_t connection;

			connection.srcTerminal = static_cast<nisync_terminal_t>(m_Routing.getTermSrc());
			connection.dstTerminal = static_cast<nisync_terminal_t>(m_Routing.getTermDst());
			connection.syncStatus = static_cast<nisync_synchronous_t>(m_Routing.getTermSyncSet());
			connection.invertStatus = static_cast<nisync_invert_t>(m_Routing.getTermInvertSet());
			std::string srcTerStr = std::to_string((long long int)connection.srcTerminal);
			std::string dstTerStr = std::to_string((long long int)connection.dstTerminal);
			conn.dstTerminal = connection.dstTerminal;

			if(connection.srcTerminal>=NISYNC_TERMINAL_INVALID || connection.dstTerminal>=NISYNC_TERMINAL_INVALID){
				m_Routing.setTermSetStatus(timestamp,"WARNING");
				m_Routing.setTermSetCode(timestamp,1);
				errorStr = "Invalid Source or Destination terminal selected: Source: "+srcTerStr+" Dest: "+ dstTerStr;
				p_Device->setError("nisyncRouting","WARNING",errorStr);
				ndsWarningStream(m_Routing)<<errorStr<<std::endl;
			}else{
				if(nisync_get_trigger_terminal_connection(m_devData.devfd, &conn.srcTerminal, &conn.syncStatus, conn.dstTerminal, &conn.invertStatus)) {
					m_Routing.setTermSetStatus(timestamp,"ERROR");
					m_Routing.setTermSetCode(timestamp,-1);
					errorStr= "Failed to obtain connection status for terminal "+dstTerStr+": "+ std::string(strerror(errno));
					p_Device->setError("nisyncRouting","ERROR",errorStr);
					ndsErrorStream(m_Routing)<<errorStr<<std::endl;;
					//throw nds::NdsError(errorStr);
				}

				if(conn.srcTerminal != NISYNC_TERMINAL_INVALID) {
					if (nisync_disconnect_trigger_terminals(m_devData.devfd, conn.srcTerminal, conn.dstTerminal)) {
						//m_Routing.setTermSetStatus(timestamp,"Failed to disconnect terminals "+ std::to_string((long long int)conn.srcTerminal) +" -> "+ dstTerStr +": "+ std::string(strerror(errno)));
						m_Routing.setTermSetStatus(timestamp,"ERROR");
						m_Routing.setTermSetCode(timestamp,-1);
						errorStr = "Failed to disconnect terminal "+dstTerStr+": "+ std::string(strerror(errno));
						p_Device->setError("nisyncRouting","ERROR",errorStr);
						ndsErrorStream(m_Routing)<<errorStr<<std::endl;
						//throw nds::NdsError(errorStr);
					}
				}

				if (nisync_connect_trigger_terminals(m_devData.devfd, connection.srcTerminal, connection.dstTerminal, connection.syncStatus, connection.invertStatus)) {
					m_Routing.setTermSetStatus(timestamp,"ERROR");
					m_Routing.setTermSetCode(timestamp,-1);
					errorStr = "Failed to connect terminals "+ srcTerStr +" -> "+ dstTerStr +": "+ std::string(strerror(errno));
					p_Device->setError("nisyncRouting","ERROR",errorStr);
					ndsErrorStream(m_Routing)<<errorStr<<std::endl;
					//throw nds::NdsError(errorStr);

				}
				m_Routing.setTermSetStatus(timestamp,"OK");
				m_Routing.setTermSetCode(timestamp,0);
			}
		}else{//TermSetPV==0
			m_Routing.setTermSetStatus(timestamp,"WARNING");
			m_Routing.setTermSetCode(timestamp,1);
			errorStr = "TermSet not equal to 1";
			p_Device->setError("nisyncRouting","WARNING",errorStr);
			ndsWarningStream(m_Routing)<<errorStr<<std::endl;
		}

	}else{
		m_Routing.setTermSetStatus(timestamp,"WARNING");
		m_Routing.setTermSetCode(timestamp,1);
		errorStr = "Should turn ON the Routing node.";
		p_Device->setError("nisyncRouting","WARNING",errorStr);
		ndsWarningStream(m_Routing)<<errorStr<<std::endl;
	}

}

void nisyncRouting::PV_Routing_TermDstRead_Writer(const timespec& timestamp, const std::int32_t& value){

	std::string errorStr;
	if(m_actualState==nds::state_t::on || m_actualState==nds::state_t::running ){

		connection_t conn;

		conn.dstTerminal = static_cast<nisync_terminal_t>(value);
		std::string dstTerStr = std::to_string((long long int)value);

		if(nisync_get_trigger_terminal_connection(m_devData.devfd, &conn.srcTerminal, &conn.syncStatus, conn.dstTerminal, &conn.invertStatus)) {
			errorStr = "Failed to obtain connection status for terminal "+dstTerStr+": "+ std::string(strerror(errno));
			p_Device->setError("nisyncRouting","ERROR",errorStr);
			ndsErrorStream(m_Routing)<<errorStr<<std::endl;
			//throw nds::NdsError(errorStr);
		}else{
			m_Routing.setTermSrcRead(timestamp, conn.srcTerminal);
			m_Routing.setTermSyncRead(timestamp,conn.syncStatus);
			m_Routing.setTermInvertRead(timestamp,conn.invertStatus);
		}

	}else{
		errorStr = "Should turn ON the Routing node.";
		p_Device->setError("nisyncRouting","WARNING",errorStr);
		ndsWarningStream(m_Routing)<<errorStr<<std::endl;
	}

}

void nisyncRouting::PV_Routing_ClkSet_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_Routing.getTimestamp();
	*value = 0;
}
void nisyncRouting::PV_Routing_ClkDstRead_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_Routing.getTimestamp();
	*value = 0;
}
void nisyncRouting::PV_Routing_TermSet_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_Routing.getTimestamp();
	*value = 0;
}
void nisyncRouting::PV_Routing_TermDstRead_Initializer(timespec* timestamp, std::int32_t* value){
	*timestamp = m_Routing.getTimestamp();
	*value = 0;
}

