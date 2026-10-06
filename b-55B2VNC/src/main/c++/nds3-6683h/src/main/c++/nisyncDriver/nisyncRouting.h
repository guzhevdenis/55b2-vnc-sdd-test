/**
 * @file nisyncRouting.h
 * @brief Header file to define the nisyncRouting class
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#include <nisync-lib.h>
#include <nds3/nds.h>

#include "nds3/nisyncDevice.h"

/**
 * @brief Implementation of nisync_tools for Routing configuration.
 *
 * This is a class that implements routing connection tools using nisync tools for the
 * PXI6683H board, it allows select several connections between terminals and/or clocks.
 *
 * <i>This class is used as a container to separate the nodes into files without interfering
 * into the nds structure.This helps with code maintainability. This should be added to
 * a generic include file.</i>
 *
 * @ingroup Routing
 */
class nisyncRouting {

public:

	/**
	 * @brief Constructor of the class.
	 *
	 * It will add the  NDS nodes as part of the control system. These nodes will be automatically
	 * added as children of any parent class of terminal.
	 *
	 * @param name The name given to the node
	 * @param parent Parent node which the FTE node belongs
	 * @param device Pointer to nisyncDevice
	 * @param autoEnable Enables the Routing
	 *
	 */
	nisyncRouting(const std::string& name, nds::Node& parent,nisyncDevice* device, nds::autoEnable_t autoEnable);
	/**
	 * @brief Destructor of the class
	 */
	~nisyncRouting();

	/**
	 * @brief Routing node to handle the routing configuration between clocks and terminals.
	 *
	 * This node is linked to the next functions: @n
	 * <ul>
	 * 		<li>@ref switchOn_Routing </li>
	 * 		<li>@ref switchOff_Routing </li>
	 * 		<li>@ref start_Routing </li>
	 * 		<li>@ref stop_Routing </li>
	 * 		<li>@ref recover_Routing </li>
	 * 		<li>@ref allow_Routing_Change </li>
	 * 		<li>@ref PV_Routing_ClkSet_Writer </li>
	 * 		<li>@ref PV_Routing_ClkDstRead_Writer </li>
	 * 		<li>@ref PV_Routing_TermSet_Writer </li>
	 * 		<li>@ref PV_Routing_TermDstRead_Writer </li>
	 * </ul>
	 *
	 * @ingroup RoutingMemberVariables
	 */
	nds::Routing<std::string> m_Routing;

	/**
	 * @brief Delegate function called in the transition from OFF and ON state.
	 */
	void switchOn_Routing();

	/**
	 * @brief Delegate function called in the transition from ON and OFF state.
	 */
	void switchOff_Routing();

	/**
	 * @brief Delegate function called in the transition from ON and RUNNING state.
	 */
	void start_Routing();

	/**
	 * @brief Delegate function called in the transition from RUNNING and ON state.
	 */
	void stop_Routing();

	/**
	 * @brief Delegate function called in the transition from FAULT to OFF state.
	 */
	void recover_Routing();

	/**
	 * @brief Function to check if the required transition between states is allowed.
	 * @param currentState Current state of the state machine.
	 * @param currentGlobalState Current global of the state machine.
	 * @param desiredState Desired state where the state machine should switch.
	 * @return true if the transition of states is allowed.
	 */
	bool allow_Routing_Change(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState);

	/**
	 * @brief Routing clock setter delegate function. This function allows a specific
	 * routing configuration between two clock terminals
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup RoutingDelegateFunctions
	 */
	void PV_Routing_ClkSet_Writer(const timespec& timestamp, const std::int32_t& value);
	/**
	 * @brief Routing clock destination read delegate function. This function reads the clock
	 * source for a clock terminal.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup RoutingDelegateFunctions
	 */
	void PV_Routing_ClkDstRead_Writer(const timespec& timestamp, const std::int32_t& value);
	/**
	 * @brief Routing terminal setter delegate function. This function allows a specific
	 * routing configuration between two terminals in the board.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup RoutingDelegateFunctions
	 */
	void PV_Routing_TermSet_Writer(const timespec& timestamp, const std::int32_t& value);
	/**
	 * @brief Routing terminal destination read delegate function. This function reads the terminal
	 * source for a destination terminal.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup RoutingDelegateFunctions
	 */
	void PV_Routing_TermDstRead_Writer(const timespec& timestamp, const std::int32_t& value);


	/**
	 * Delegate PVs initializers
	 */
	void PV_Routing_ClkSet_Initializer(timespec* timestamp, std::int32_t* value);
	void PV_Routing_ClkDstRead_Initializer(timespec* timestamp, std::int32_t* value);
	void PV_Routing_TermSet_Initializer(timespec* timestamp, std::int32_t* value);
	void PV_Routing_TermDstRead_Initializer(timespec* timestamp, std::int32_t* value);

private:

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// --------------------------------- MEMBER VARIABLES ---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * @brief Pointer to nisyncDevice. It allows the communication with the main class of the Device and
	 * all it public resources (RootNode stateMachine states, error PVs at device level)
	 *
	 * @ingroup RoutingMemberVariables
	 */
	nisyncDevice* p_Device;

	/**
	 * @brief Copy of file descriptor, number of device and type of board.
	 *
	 * The next table shows the fields and data types of the structure that can be assigned:
	 *
	 * <table>
	 * 	<tr> <td><b><i>Data type</i></b></td> <td><b>Name</b></td> </tr>
	 * 	<tr> <td><i>nisync_device_type</i></td> <td>devtype</td> </tr>
	 * 	<tr> <td><i>int</i></td> <td>devfd</td> </tr>
	 * 	<tr> <td><i>int</i></td> <td>devnum</td> </tr>
	 * </table>
	 *
	 * @ingroup RoutingMemberVariables
	 */
	devData_t m_devData;

	/**
	 * @brief Struct nisync_device_info contains all the data related with the board information
	 *
	 * @ingroup RoutingMemberVariables
	 */
	nds::state_t m_actualState;


};
