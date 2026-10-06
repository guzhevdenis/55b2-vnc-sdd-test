/**
 * @file
 * @brief Header file to define the nisyncFirmware class
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#include <nds3/nds.h>

/**
 * @brief This is a class that implements Timestamping reading using nisync_tools for the
 * PXI6683H board. It allows to configure the edge of the timestamping in an specific terminal.
 *
 * <br>In the transition from the state OFF to ON open and block a terminal int he board.
 *
 * <i>This class is used as a container to separate the nodes into files without interfering
 * into the nds structure.This helps with code maintainability. This should be added to
 * a generic include file.</i>
 *
 * @ingroup TerminalTS
 */
class nisyncTerminal {

public:

	/**
	 * @brief Constructor of the class.
	 *
	 *   It will add the Timestamping NDS nodes as part of the control system.
	 *   These nodes will be automatically added as children of any parent class of terminal.
	 *
	 * @param name The name given to the node
	 * @param termNumber Number of the terminal
	 * @param parent Parent node which the timestamping node belongs
	 * @param device Pointer to nisyncDevice
	 * @param autoEnable Enables the terminal
	 *
	 */
	nisyncTerminal(const std::string& name, nds::Node& parent,nisync_terminal_t termNumber,nds::autoEnable_t autoEnable,nisyncDevice* device);
	/**
	 * @brief Destructor of the class
	 *
	 */
	~nisyncTerminal();

	/**
	 * This class will be a new child node of the given parent node, in this case
	 * the driver class that holds the rootNode.
	 */
	nds::Node m_nisyncTerminal;

	/**
	 * @brief Timestamping node to handle Timestamping in the device terminals.
	 *
	 * nds::Timestamping node provides PVs to set-up the Timestamping in different terminals.
	 *
	 */
	nds::Timestamping<nds::timestamp_t> m_timestamping;

	/**
	 * @brief FTE node to handle FTE in the device terminals.
	 *
	 * nds::FTE node provides PVs to set-up the FTE in different terminals.
	 *
	 */
	nds::FTE<std::string> m_FTE;

	/**
	 * @brief State machine node to handle the device.
	 *
	 * This node is linked to the next functions: @n
	 * <ul>
	 * 		<li>@ref switchOn_terminal </li>
	 * 		<li>@ref switchOff_terminal </li>
	 * 		<li>@ref start_terminal </li>
	 * 		<li>@ref stop_terminal </li>
	 * 		<li>@ref recover_terminal </li>
	 * 		<li>@ref allow_terminal_Change </li>
	 * </ul>
	 */
	nds::StateMachine m_terminal_stateMachine;

	/**
	 * @brief Delegate function called in the transition from OFF and ON state.
	 */
	void switchOn_terminal();

	/**
	 * @brief Delegate function called in the transition from ON and OFF state.
	 */
	void switchOff_terminal();

	/**
	 * @brief Delegate function called in the transition from ON and RUNNING state.
	 */
	void start_terminal();

	/**
	 * @brief Delegate function called in the transition from RUNNING and ON state.
	 */
	void stop_terminal();

	/**
	 * @brief Delegate function called in the transition from FAULT to OFF state.
	 */
	void recover_terminal();

	/**
	 * @brief Function to check if the required transition between states is allowed.
	 * @param currentState Current state of the state machine.
	 * @param currentGlobalState Current global of the state machine.
	 * @param desiredState Desired state where the state machine should switch.
	 * @return true if the transition of states is allowed.
	 */
	bool allow_terminal_Change(const nds::state_t currentState, const nds::state_t currentGlobalState, const nds::state_t desiredState);

	/**
	 * Used to block implicit FTE and TS state machines as the terminal state machine controls the terminal.
	 */
	void disable_terminal_state();
	bool disable_terminal_Change(const nds::state_t, const nds::state_t, const nds::state_t);

	/**
	 * @brief This function will be called when the timestamping functionalities
	 * need to be enabled.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup TimestampingDelegateFunctions
	 */
	void pv_enable_writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Function that specifies in which edge of the clock signal shall the
	 * samples be acquired.
	 *
	 * The next table shows the values that can be assigned to edge:
	 *
	 * <table>
	 * 	<tr> <td><b><i>value</i></b></td> <td><b>Meaning</b></td>  </tr>
	 * 	<tr> <td><i>0</i></td> <td>Rising</td> </tr>
	 * 	<tr> <td><i>1</i></td> <td>Falling</td> </tr>
	 * 	<tr> <td><i>2</i></td> <td>Any</td> </tr>
	 * 	<tr> <td><i>Other</i></td> <td>Invalid</td> </tr>
	 * </table>
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup TimestampingDelegateFunctions
	 */
	void pv_edge_writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Functions that clears the stack of timestamps pushed by the hardware
	 * in case there is an overflow.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup TimestampingDelegateFunctions
	 */
	void pv_clearoverflow_writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Thread that will be reading all the timestamps captured by the board for the
	 * configured terminal.
	 */
	void timestamping_thread_body();

	/**
	 * @brief A thread that runs timestamping_thread_body().
	 * @ingroup FirmwareMemberVariables
	 */
	nds::Thread m_timestamping_thread;

	/**
	 * @brief A boolean flag that stop the Firmware loop in Firmware_thread_body()
	 *        when true.
	 * @ingroup TimestampingMemberVariables
	 */
	volatile bool m_stop_timestamping;

	/**
	 * Initializers
	 */
	void pv_edge_initializer(timespec* timestamp, std::int32_t* value);

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// ----------------------------------------FTE-----------------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	/**
	 * @brief FTE delegate function to enable the FTEs for a terminal.
	 *
	 *  It enables or disables the use of FTEs for the terminal.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system. value>=1, enables FTEs. value<=0 disables FTEs.
	 * @ingroup FTEDelegateFunctions
	 */
	void PV_FTE_Enable_Writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief FTE delegate function for set a FTE.
	 *
	 * It allows to schedule a single FTE,
	 * the generation of a single pulse in a terminal of the board, the generation of
	 * a clock in a terminal of the board or change the level of a terminal immediately.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup FTEDelegateFunctions
	 */
	void PV_FTE_Set_Writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief FTE delegate function for suppress a FTE.
	 *
	 * It allows to suppress a single FTE in a particular terminal of the board or
	 * suppress all the FTEs scheduled in all the terminals in the board.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup FTEDelegateFunctions
	 */
	void PV_FTE_Suppress_Writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief FTE delegate function for change the period in a previously FTE scheduled.
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup FTEDelegateFunctions
	 */
	void PV_FTE_ChgPeriod_Writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief FTE delegate function that retrieve how many FTEs have been scheduled in one specific terminal
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup FTEDelegateFunctions
	 */
	void PV_FTE_PendingValue_Writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief FTE delegate function that retrieve how many FTEs have been scheduled in one specific terminal
	 *
	 * @param timestamp Timestamp when the PV was called by the control system.
	 * @param value Actual PV value set by the control system
	 * @ingroup FTEDelegateFunctions
	 */
	void PV_FTE_checkFteLevels(const timespec& timestamp, const std::int32_t& value);


	/**
	 * Initializers
	 */
	void PV_FTE_Enable_Initializer(timespec* timestamp, std::int32_t* value);
	void PV_FTE_Set_Initializer(timespec* timestamp, std::int32_t* value);
	void PV_FTE_Suppress_Initializer(timespec* timestamp, std::int32_t* value);
	void PV_FTE_ChgPeriod_Initializer(timespec* timestamp, std::int32_t* value);
	void PV_FTE_PendingValue_Initializer(timespec* timestamp, std::int32_t* value);

private:

	/**
	 * @brief Pointer to nisyncDevice. It allows the communication with the main class of the Device and
	 * all it public resources (RootNode stateMachine states, error PVs at device level)
	 * @ingroup TimestampingMemberVariables
	 */
	 nisyncDevice* const p_Device;

	/**
	 * @brief Copy of file descriptor, number of device and type of board.
	 * @ingroup TimestampingMemberVariables
	 */
	devData_t m_devData;
	/**
	 * @brief Number of terminal that will be opened.
	 * @ingroup TimestampingMemberVariables
	 */
	const nisync_terminal_t m_termNumber;

	/**
	 * @brief File descriptor of a terminal in the board.
	 * @ingroup TimestampingMemberVariables
	 */
	int m_termfd;

	/**
	 * @brief Name of the node in the control system
	 * @ingroup TimestampingMemberVariables
	 */
	std::string m_name;

	/**
	 * @brief Maximum number of timestamps that can be read at once.
	 * @ingroup TimestampingMemberVariables
	 */
	static const size_t m_tsLen = 1024;

	/**
	 * @brief Name of the node in the control system
	 * @ingroup TimestampingMemberVariables
	 */
	nisyncTimestamp_t m_timestampConf;

	/**
	 * @brief Input PV with test purposes.
	 *
	 * @ingroup TimestampingMemberVariables
	 */
	nds::PVVariableIn<std::int32_t> m_numberOfPushedTimestamps;
	/**
	 * @brief Output PV to set the type of Decimation during timestamping.
	 *
	 * This PV will be updated by the first Node with an Error status, it will
	 * contain the cause of the error.
	 *
	 * @ingroup TimestampingMemberVariables
	 */
	nds::PVVariableOut<std::int32_t> m_decimationType;
	/**
	 * @brief Output PV to set the Hardware Decimation.
	 *
	 * This PV will configure the decimation by hardware using the nisync driver
	 *
	 * @ingroup TimestampingMemberVariables
	 */
	nds::PVVariableOut<std::int32_t> m_decimationHW;

	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// ----------------------------------------FTE-----------------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	/**
	 * @brief Variable that stores the maximum number of FTEs that can be scheduled in one terminal.
	 * @ingroup FTEMemberVariables
	 */
	uint32_t m_maxFtes;

	/**
	 * @brief Map that stores the clocks that have been scheduled in each terminal.
	 * @ingroup FTEMemberVariables
	 */
	clk_t scheduledClock;

	/**
	 * @brief Enable FTE for the terminal
	 *
	 * @ingroup FTEMemberVariables
	 */
	nds::PVDelegateOut<std::int32_t > m_enableFTE;
	/**
	 * @brief Enable FTE for the terminal Readback value.
	 *
	 * @ingroup FTEMemberVariables
	 */
	nds::PVVariableIn<std::int32_t> m_enableFTE_RBV;
	/**
	 * @brief Flag that indicates if the FTEs has been enabled.
	 *
	 * @ingroup FTEMemberVariables
	 */
	bool m_FTEEnabled;

	/**
	 * @brief Checks FTE levels
	 *
	 * @ingroup FTEMemberVariables
	 */
	nds::PVDelegateOut<std::int32_t > m_checkFTELevels;

	/**
	 * @brief Checks FTE levels
	 *
	 * @ingroup FTEMemberVariables
	 */
	nds::PVVariableIn<std::int32_t > m_checkFTENumErrors;

	/**
	 * @brief Checks FTE levels
	 *
	 * @ingroup FTEMemberVariables
	 */
	nds::PVVariableIn<std::string> m_checkFTELevelsStatus;

	/**
	 * @brief Checks FTE levels
	 *
	 * @ingroup FTEMemberVariables
	 */
	nds::PVVariableIn<std::int32_t > m_checkFTELevelsCode;


	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------
	// ----------------------------------------HELPERS---------------------------------------
	// ------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------

	void terminalError(std::string errorStr);
	void terminalWarning(std::string errorStr);

};
