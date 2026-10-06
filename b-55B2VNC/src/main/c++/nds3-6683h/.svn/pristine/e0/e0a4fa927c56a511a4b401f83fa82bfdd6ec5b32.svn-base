#include <iostream>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"


TEST(TP_NPD,0008)
{

	std::string statusRBV;
	std::int32_t codeRBV;

	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0};

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test: TP_NPD_0008");
	std::cout<<RST;

	TestUtils::displayTitle("Checking ERROR states");

	std::cout<<FCYN;
	TestUtils::waitEnterKey("Test ready. Press enter to Start");
	std::cout<<RST;

	//Create factory
	nds::Factory factory("test");
	try{
		const timespec* pStateMachineSwitchTime;
		nds::namedParameters_t parameters;
		parameters.emplace("TERMINALS","0"); //PFI0 terminal

		// Create test device of type Device and named rootNode
		factory.createDevice("nisyncDevice", "rootNode", parameters);

		//Get instance of the Test Control System
		nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		// ----------------------------------- Firmware Node ----------------------------------------
		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		std::cout<<FBLU;
		std::cout<<"Checking Firmware Error Management...\n";
		std::cout<<RST;
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		// ----------------------------------- Routing Node -----------------------------------------
		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		std::cout<<FBLU;
		std::cout<<"\nChecking Routing Error Management...\n";
		std::cout<<RST;
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		std::cout<<"\tPutting/getting some PVs with the node state machine in state OFF...\n";
		pInterface->writeCSValue("/rootNode-Routing.ClkSet", timestamp, (std::int32_t)0);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-Routing.ClkDstRead", timestamp, (std::int32_t)NISYNC_CLKOUT);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)0);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-Routing.TermDstRead", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		///--------------------------------------
		///--------- STATE MACHINES -------------
		///--------------------------------------
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> RUNNING
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (initializing) -> RUNNING
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		std::cout<<"\tSetting some incorrect values in configuration PVs...\n";
		//Invalid set value
		pInterface->writeCSValue("/rootNode-Routing.ClkSet", timestamp, (std::int32_t)0);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		//Invalid clk source
		pInterface->writeCSValue("/rootNode-Routing.ClkSrc", timestamp, (std::int32_t)NISYNC_CLK_INVALID);
		pInterface->writeCSValue("/rootNode-Routing.ClkSet", timestamp, (std::int32_t)1);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		//Invalid clk dst read
		pInterface->writeCSValue("/rootNode-Routing.ClkDstRead", timestamp, (std::int32_t)NISYNC_CLK_INVALID);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);


		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)0);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_TERMINAL_INVALID);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_TERMINAL_INVALID);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		//Change state:  RUNNING -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  RUNNING -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);



		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		// ----------------------------------- Timing Node -----------------------------------------
		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		std::cout<<FBLU;
		std::cout<<"\nChecking Timing Error Management...\n";
		std::cout<<RST;
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		timespec timeReadCS{0,0};
		pInterface->readCSValue("/rootNode-Timing.Time", &timestamp, &timeReadCS); //needed to push the data


		///--------------------------------------
		///--------- STATE MACHINES -------------
		///--------------------------------------
		std::cout<<"\tSetting some incorrect values in configuration PVs with the node state machine in state ON...\n";
		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (initializing) -> RUNNING
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		//Change state:  RUNNING -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		// --------------------------------- Timestamping Node --------------------------------------
		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		std::cout<<FBLU;
		std::cout<<"\nChecking TR0.TS Error Management...\n";
		std::cout<<RST;
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		pInterface->writeCSValue("/rootNode-TR0.TS.Edge", timestamp, (std::int32_t)NISYNC_EDGE_INVALID);

		///--------------------------------------
		///--------- STATE MACHINES -------------
		///--------------------------------------
		std::cout<<"\tSetting some incorrect values in configuration PVs with the node state machine in state ON...\n";
		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		pInterface->writeCSValue("/rootNode-TR0.TS.Enable", timestamp, (std::int32_t)0);
		pInterface->writeCSValue("/rootNode-TR0.TS.Edge", timestamp, (std::int32_t)NISYNC_EDGE_INVALID);

		//Change state:  ON -> (initializing) -> RUNNING
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);


		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		// ----------------------------------- TR0 Node -----------------------------------------
		// ------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------
		std::cout<<FBLU;
		std::cout<<"\nChecking TR0.FTE Error Management...\n";
		std::cout<<RST;

		//Trying to switch ON the Node before switching on the device.
		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		std::cout<<"\tPutting/getting some PVs with the node state machine in state OFF...\n";
		pInterface->writeCSValue("/rootNode-TR0.FTE.Set",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.SetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.SetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-TR0.FTE.Suppress",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-TR0.FTE.ChgPeriod",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodCode", &timestamp, &codeRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalPending",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.PendingStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.PendingCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-TR0.FTE.checkFTELevels",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.checkFTELevelsStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.checkFTELevelsCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);


		///-----------------------------------------
		///--------- STATE MACHINES ON -------------
		///-----------------------------------------
		std::cout<<"\tSetting some incorrect values in configuration PVs with the node state machine in state ON...\n";
		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		pInterface->writeCSValue("/rootNode-TR0.FTE.Set",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.SetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.SetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		//pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalSet",timestamp,(std::int32_t)NISYNC_TERMINAL_INVALID); // PVVariables are thread safe
//		pInterface->writeCSValue("/rootNode-TR0.FTE.Set",timestamp,(std::int32_t)2); // PVVariables are thread safe
//		pInterface->readCSValue("/rootNode-TR0.FTE.SetStatus", &timestamp, &statusRBV);
//		pInterface->readCSValue("/rootNode-TR0.FTE.SetCode", &timestamp, &codeRBV);
//		EXPECT_EQ((std::string)"WARNING", statusRBV);
//		EXPECT_EQ((std::int32_t)1, codeRBV);

        pInterface->writeCSValue("/rootNode-TR0.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::MAX_MODE_NUM); // PVVariables are thread safe
		//pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalSet",timestamp,(std::int32_t)NISYNC_PFI2); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR0.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.SetStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.SetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		//Change state:  ON -> (switchingOff) -> OFF && OFF -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		pInterface->writeCSValue("/rootNode-TR0.FTE.Suppress",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		//pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalSuppress",timestamp,(std::int32_t)NISYNC_TERMINAL_INVALID); // PVVariables are thread safe
//		pInterface->writeCSValue("/rootNode-TR0.FTE.Suppress",timestamp,(std::int32_t)1); // PVVariables are thread safe
//		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressStatus", &timestamp, &statusRBV);
//		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressCode", &timestamp, &codeRBV);
//		EXPECT_EQ((std::string)"WARNING", statusRBV);
//		EXPECT_EQ((std::int32_t)1, codeRBV);

//        pInterface->writeCSValue("/rootNode-TR0.FTE.ModeSuppress",timestamp,(std::int32_t)suppressMode_t::MAX_MODE_NUM); // PVVariables are thread safe
//		pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalSuppress",timestamp,(std::int32_t)NISYNC_PFI2); // PVVariables are thread safe
//		pInterface->writeCSValue("/rootNode-TR0.FTE.Suppress",timestamp,(std::int32_t)1); // PVVariables are thread safe
//		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressStatus", &timestamp, &statusRBV);
//		pInterface->readCSValue("/rootNode-TR0.FTE.SuppressCode", &timestamp, &codeRBV);
//		EXPECT_EQ((std::string)"WARNING", statusRBV);
//		EXPECT_EQ((std::int32_t)1, codeRBV);

		//Change state:  ON -> (switchingOff) -> OFF && OFF -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		pInterface->writeCSValue("/rootNode-TR0.FTE.ChgPeriod",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

//		pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalChgPeriod",timestamp,(std::int32_t)NISYNC_TERMINAL_INVALID); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR0.FTE.ChgPeriod",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalChgPeriod",timestamp,(std::int32_t)NISYNC_PFI2); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR0.FTE.ChgPeriod",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.ChgPeriodCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);


		//Change state:  ON -> (switchingOff) -> OFF && OFF -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalPending",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.PendingStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.PendingCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-TR0.FTE.TerminalPending",timestamp,(std::int32_t)NISYNC_PFI2); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.PendingStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.PendingCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		//Change state:  ON -> (switchingOff) -> OFF && OFF -> ON
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		pInterface->writeCSValue("/rootNode-TR0.FTE.checkFTELevels",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.checkFTELevelsStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.checkFTELevelsCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

		pInterface->writeCSValue("/rootNode-TR0.FTE.checkFTELevels",timestamp,(std::int32_t)NISYNC_PFI2); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR0.FTE.checkFTELevelsStatus", &timestamp, &statusRBV);
		pInterface->readCSValue("/rootNode-TR0.FTE.checkFTELevelsCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::string)"WARNING", statusRBV);
		EXPECT_EQ((std::int32_t)1, codeRBV);

//		//Change state:  ON -> (initializing) -> RUNNING
//		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
//		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
//		::sleep(1);
//		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
//
//		//Change state:  RUNNING -> (initializing) -> ON
//		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
//		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
//		::sleep(1);
//		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-TR0.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		// Destroy test device
		factory.destroyDevice("rootNode");

	}
	catch(const std::runtime_error& re)
	{
		// specific handling for runtime_error
		std::cerr << "Runtime error: " << re.what() << std::endl;
		factory.destroyDevice("rootNode");
	}
	catch(...){
		std::cout<<"Catched Unknown exception: "<<std::endl;
		factory.destroyDevice("rootNode");
	}

}

