/**
 * @file
 * @brief Header file to define the nisyncDevice class
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#ifndef NISYNCDEVICE_H_
#define NISYNCDEVICE_H_

#include <memory>

#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <TimeTools.h>

#include <nds3/nds.h>

#include "nds3/nisyncDefinitions.h"

//#include "../nisyncDriver/nisyncFirmware.h"


class nisyncFirmware;
class nisyncRouting;
class nisyncTiming;
class nisyncTerminal;

/**
 * @brief Class used to handle a PXI6683H board using Nisync ToolsMain.
 *
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 * It contains the port implementation and all the nodes pointers to control the PXI6683H board.
 *
 * @ingroup Device
 */
class nisyncDevice
{

public:

	/**
	 * @brief Constructor of the class.
	 *
	 * It will create all the NDS nodes objects necessaries for the device and will save the pointers
	 * to the nodes. It also will add some PVs for error management.
	 *
	 * @param factory    the control system factory that requested the creation of the device
	 * @param deviceName    the name given to the device
	 * @param parameters optional parameters passed to the device
	 */
	nisyncDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
	/**
	 * @brief Destructor of the class.
	 */
	~nisyncDevice();

	/**
	 * @brief Return pointer to device data member to use nisync API from nodes.
	 * @return m_devdata
	 */
	devData_t getDevData();

	/**
	 * @brief Setter of Error PVs. It sets and pushes the different ERROR/WARNING states and
	 * messages received from nodes.
	 */
	void setError(std::string source, const std::string& grade, std::string message);

	/**
	 * @brief Delegate function called in the transition from OFF and ON state.
	 */
	void switchOn_Device();
	/**
	 * @brief Delegate function called in the transition from ON and OFF state.
	 */
	void switchOff_Device();
	/**
	 * @brief Delegate function called in the transition from ON and RUNNING state.
	 */
	void start_Device();
	/**
	 * @brief Delegate function called in the transition from RUNNING and ON state.
	 */
	void stop_Device();
	/**
	 * @brief Delegate function called in the transition from FAULT to OFF state.
	 */
	void recover_Device();

	/**
	 * @brief function called to set the Terminals name.
	 * @param autoEnable
	 */
	void nameTerm(const nds::autoEnable_t& autoEnable);
	/**
	 * @brief Function to check if the required transition between states is allowed.
	 * @param localState Current state of the state machine.
	 * @param globalState Current global of the state machine.
	 * @param newState Desired state where the state machine should switch.
	 * @return true if the transition of states is allowed.
	 */
	bool allow_Device_Change(const nds::state_t localState, const nds::state_t globalState, const nds::state_t newState);

	/**
	 * @brief Device delegate function that clears the last occurred error.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup DeviceDelegateFunctions
	 */
	void PV_Device_clearErrorMessage(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Device delegate function that clears all configured FTEs in all terminals
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup DeviceDelegateFunctions
	 */
	void PV_Device_clearDeviceFTE(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Device delegate function that clears all configured CLKs in all terminals
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup DeviceDelegateFunctions
	 */
	void PV_Device_clearDeviceCLK(const timespec& timestamp, const std::int32_t& value);
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// --------------------------------- MEMBER VARIABLES ---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * @brief State machine node to handle the device.
	 *
	 * This node is linked to the next functions: @n
	 * <ul>
	 * 		<li>@ref switchOn_Device </li>
	 * 		<li>@ref switchOff_Device </li>
	 * 		<li>@ref start_Device </li>
	 * 		<li>@ref stop_Device </li>
	 * 		<li>@ref recover_Device </li>
	 * 		<li>@ref allow_Device_Change </li>
	 * </ul>
	 * @ingroup DeviceMemberVariables
	 */
	nds::StateMachine m_Device_stateMachine;

private:
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// --------------------------------- MEMBER VARIABLES ---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	/**
	 * Iterator for the parameter list.
	 */
	nds::namedParameters_t::const_iterator m_findParam;
	/**
	 * @brief Root node of the NDS driver.
	 *
	 * All children nodes and PVs are contained within this node. This will be the
	 * main class of the driver. To enable interface with Asyn the Root class is
	 * declared as Port.
	 * @ingroup DeviceMemberVariables
	 */
	nds::Port rootNode;

	/**
	 * @brief Name of the device in the control system
	 * @ingroup DeviceMemberVariables
	 */
	std::string m_name;
	/**
	 * @brief NDS-7 reset on switchon.
	 */
	bool m_resetOn;

	/**
	 * @brief Pointer to Firmware node for handle the firmware and parameters of the device.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	std::unique_ptr<nisyncFirmware> m_Firmware_ptr;

	/**
	 * @brief Pointer to FTE node for handle FTE in the device terminals.
	 *
	 * nds::FTE node provides PVs to set-up the FTE in different terminals.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	std::unique_ptr<nisyncRouting> m_Routing_ptr;

	/**
	 * @brief Pointer to Timing node to handle the time parameters of the device.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	std::unique_ptr<nisyncTiming> m_Timing_ptr;

	/**
	 * @brief Pointer to Vector of Timestamping terminal nodes for handle Timestamping in the device terminals.
	 *
	 * nds::FTE node provides PVs to set-up the FTE in different terminals.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	std::vector<std::unique_ptr<nisyncTerminal>> m_Terminal_vctr;

	/**
	 * @brief Device descriptor for PXI6683H board.
	 *
	 * Descriptor should be initialized with a negative value.
	 * @ingroup DeviceMemberVariables
	 */
	int m_fd;

	/**
	 * @brief Device descriptor for PXI6683H board.
	 *
	 * Descriptor should be initialized with NULL or 0.
	 * @ingroup DeviceMemberVariables
	 */
	int m_devnum;

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
	 * @ingroup DeviceMemberVariables
	 */
	devData_t m_devData;

	/**
	 * @brief Mutex for safe error setting.
	 * @ingroup DeviceMemberVariables
	 */
	std::mutex mtx;

	/**
	 * @brief Variable to indicate the first error that have occurred.
	 * @ingroup DeviceMemberVariables
	 */
	ndsError_t m_error;

	/**
	 * @brief Flag to indicate if an error have occurred.
	 *
	 * Flag must be initialized to false
	 * @ingroup DeviceMemberVariables
	 */
	bool errDetect;

	/**
	 * @brief Input PV to show error information through different messages.
	 *private
	 * This PV will be updated by the first Node with an Error status, it will
	 * contain which node has caused the error.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	nds::PVVariableIn<std::string> m_errorSource;

	/**
	 * @brief Input PV to show error information through different messages.
	 *
	 * This PV will be updated by the first Node with an Error status, it will
	 * contain the cause of the error.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	nds::PVVariableIn<std::string> m_errorMessage;

	/**
	 * @brief Input PV to show error information through different messages.
	 *
	 * This PV will be updated by the first Node with an Error status, it will
	 * contain the severity level of the cause of the error.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	nds::PVVariableIn<std::int32_t> m_errorSeverity;

	/**
	 * @brief Output PV to clear the last occurred error message.
	 *
	 * This PV will be set by the user.
	 *
	 * @ingroup DeviceMemberVariables
	 */
	nds::PVDelegateOut<std::int32_t> m_clearErrorMsg;

	/**
	 * @brief Output PV to clear all configured FTEs in the device's terminals
	 *
	 * @ingroup DeviceMemberVariables
	 */
	nds::PVDelegateOut<std::int32_t> m_clearDeviceFTE;

//	/**
//	 * @brief Output PV to clear all configured CLKs in the device's terminals
//	 *
//	 * @ingroup DeviceMemberVariables
//	 */
//	nds::PVDelegateOut<std::int32_t> m_clearDeviceCLK;

};
#endif
