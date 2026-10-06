/**
 * @file nisyncFirmware.h
 * @brief Header file to define the nisyncFirmware class
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#pragma once
#include <nisync-lib.h>
#include <nds3/nds.h>

class nisyncDevice;

/**
 * @brief Implementation of nisync_tools for Firmware and board information details.
 * This is a class that retrieve all the board information and checks continuously the
 * status of the PXI6683H board.
 *
 * In the transition from OFF to ON state, will get all basic information about the PXI6683H board.
 *
 * <i>This class is used as a container to separate the nodes into files without interfering
 * into the nds structure.This helps with code maintainability. This should be added to
 * a generic include file.</i>
 *
 * @ingroup Firmware
 */
class nisyncFirmware {

public:

	/**
	 * @brief Constructor of the class.
	 *
	 * It will add the  NDS nodes as part of the control system. These nodes will be automatically
	 * added as children of any parent class of terminal.
	 *
	 * @param name The name given to the node
	 * @param parent Parent node which the Firmware node belongs
	 * @param device Pointer to nisyncDevice
	 * @param autoEnable Enables the firmware
	 */
	nisyncFirmware(const std::string& name, nds::Node& parent,nisyncDevice* device, nds::autoEnable_t autoEnable);

	/**
	 * @brief Destructor of the class
	 *
	 */
	~nisyncFirmware();

	/**
	 * @brief Firmware node to handle the firmware and parameters of the device.
	 *
	 * This node is linked to the next functions: @n
	 * <ul>
	 * 		<li>@ref switchOn_Firmware </li>
	 * 		<li>@ref switchOff_Firmware </li>
	 * 		<li>@ref start_Firmware </li>
	 * 		<li>@ref stop_Firmware </li>
	 * 		<li>@ref recover_Firmware </li>
	 * 		<li>@ref allow_Firmware_Change </li>
	 * 		<li>@ref PV_Firmware_Path_Writer </li>
	 * </ul>
	 */
	nds::Firmware m_Firmware;

	/**
	 * @brief Delegate function called in the transition from OFF and ON state.
	 */
	void switchOn_Firmware();

	/**
	 * @brief Delegate function called in the transition from ON and OFF state.
	 */
	void switchOff_Firmware();

	/**
	 * @brief Delegate function called in the transition from ON and RUNNING state.
	 */
	void start_Firmware();

	/**
	 * @brief Delegate function called in the transition from RUNNING and ON state.
	 */
	void stop_Firmware();

	/**
	 * @brief Delegate function called in the transition from FAULT to OFF state.
	 */
	void recover_Firmware();

	/**
	 * @brief Function to check if the required transition between states is allowed.
	 * @param currentState Current state of the state machine.
	 * @param currentGlobalState Current global of the state machine.
	 * @param desiredState Desired state where the state machine should switch.
	 * @return true if the transition of states is allowed.
	 */
	bool allow_Firmware_Change(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState);

	/**
	 * @brief Firmware setters
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup FirmwareDelegateFunctions
	 */
	void PV_Firmware_Path_Writer(const timespec& timestamp, const std::string& value);

	/**
	 * @brief Function that will be checking the status of the board and will publish it
	 * to the control system.
	 */
	void Firmware_thread_body();

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// --------------------------------- MEMBER VARIABLES ---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * @brief A thread that runs Firmware_thread_body().
	 * @ingroup FirmwareMemberVariables
	 */
	nds::Thread m_Firmware_Thread;

	/**
	 * @brief A boolean flag that stop the Firmware loop in Firmware_thread_body()
	 *        when true.
	 * @ingroup FirmwareMemberVariables
	 */
	volatile bool m_bStop_Firmware;



private:
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// --------------------------------- MEMBER VARIABLES ---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * @brief Pointer to nisyncDevice. It allows the communication with the main class of the Device and
	 * all it public resources (RootNode stateMachine states, error PVs at device level)
	 * @ingroup FirmwareMemberVariables
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
	 * @ingroup FirmwareMemberVariables
	 */
	devData_t m_devData;

	/**
	 * @brief Struct nisync_device_info contains all the data related with the board information
	 * @ingroup FirmwareMemberVariables
	 */
	nds::state_t m_actualState;

	/**
	 * @brief Struct nisync_device_info contains all the data related with the board information
	 * @ingroup FirmwareMemberVariables
	 */
	nisync_device_info device_info;
};
