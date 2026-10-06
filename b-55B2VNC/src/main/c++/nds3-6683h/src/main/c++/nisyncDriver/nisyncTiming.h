/**
 * @file nisyncTiming.h
 * @brief Header file to define the nisyncTiming class
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#include <iomanip>
#include <sstream>

#include <nisync-lib.h>
#include <nds3/nds.h>

#include "nds3/nisyncDevice.h"

/**
 * @brief Implementation of nisync_tools for get the Time of the PXI6683H board.
 *
 * This is a class that implements
 *
 * <i>This class is used as a container to separate the nodes into files without interfering
 * into the nds structure.This helps with code maintainability. This should be added to
 * a generic include file.</i>
 *
 */

class nisyncTiming {

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
	 * @param autoEnable Enables the timing
	 *
	 */
	nisyncTiming(const std::string& name, nds::Node& parent,nisyncDevice* device, nds::autoEnable_t autoEnable);
	/**
	 * @brief Destructor of the class
	 *
	 */
	~nisyncTiming();

	/**
	 * @brief Timing node to handle the timing configuration between clocks and terminals.
	 *
	 * This node is linked to the next functions: @n
	 * <ul>
	 * 		<li>@ref switchOn_timing </li>
	 * 		<li>@ref switchOff_timing </li>
	 * 		<li>@ref start_timing </li>
	 * 		<li>@ref stop_timing </li>
	 * 		<li>@ref recover_timing </li>
	 * 		<li>@ref allow_timing_change </li>
	 * 		<li>@ref pv_timing_reader </li>
	 * </ul>
	 *
	 * @ingroup TimingMemberVariables
	 */
	nds::Timing m_Timing;

	/**
	 * @brief Delegate function called in the transition from OFF and ON state.
	 */
	void switchOn_timing();

	/**
	 * @brief Delegate function called in the transition from ON and OFF state.
	 */
	void switchOff_timing();

	/**
	 * @brief Delegate function called in the transition from ON and RUNNING state.
	 */
	void start_timing();

	/**
	 * @brief Delegate function called in the transition from RUNNING and ON state.
	 */
	void stop_timing();

	/**
	 * @brief Delegate function called in the transition from FAULT to OFF state.
	 */
	void recover_timing();

	/**
	 * @brief Function to check if the required transition between states is allowed.
	 * @param currentState Current state of the state machine.
	 * @param currentGlobalState Current global of the state machine.
	 * @param desiredState Desired state where the state machine should switch.
	 * @return true if the transition of states is allowed.
	 */
	bool allow_timing_change(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState);

	/**
	 *  @brief Delegate function that reads and pushes the Time in the board to the control system.
	 *
	 *	<i>Data time in text format (%Y-%m-%d T%T.Nanoseconds).It is not published to EPICS because
	 *	the Timing node does not provide a setter for this PV. Instead of that, the own node publishes
	 *	the Time with the next format:"Day Mon dd hh:mm:ss yyyy"</i>
	 *
	 *	@param timestamp Timestamp when the PV was called by the control system.
	 *	@param value Actual PV value set by the control system
	 *  @ingroup TimingDelegateFunctions
	 */
	void pv_timing_reader(timespec * timestamp, timespec * value);

	/**
	 * @brief Function that will be acquiring in a thread the seconds since the board is synchronized
	 * with the ITER time.
	 */
	void timing_thread_body();

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// --------------------------------- MEMBER VARIABLES ---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * @brief Thread that runs Timing_thread_body() to get the Seconds since the board is synchronized.
	 *
	 * @ingroup TimingMemberVariables
	 */
	nds::Thread m_timing_thread;

	/**
	 * @brief Boolean flag that stop the Timing loop in Timing_thread_body() when true.
	 *
	 * @ingroup TimingMemberVariables
	 */
	volatile bool m_bStop_timing;


private:

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// --------------------------------- MEMBER VARIABLES ---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * @brief Pointer to nisyncDevice. It allows the communication with the main class of the Device and
	 * all it public resources (RootNode stateMachine states, error PVs at device level)
	 * @ingroup TimingMemberVariables
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
	 * @ingroup TimingMemberVariables
	 */
	devData_t m_devData;

	/**
	 * @brief Struct nisync_device_info contains all the data related with the board information
	 * @ingroup TimingMemberVariables
	 */
	nds::state_t m_actualState;

	/**
	 * @brief Struct nisync_device_info contains all the data related with the board information
	 * @ingroup TimingMemberVariables
	 */
	nisync_device_info device_info;


};
