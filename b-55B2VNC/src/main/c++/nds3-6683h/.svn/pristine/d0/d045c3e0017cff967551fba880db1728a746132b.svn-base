#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"


TEST(TP_NPD,0005)
{


	std::string statusRBV;
	const std::int32_t* codeRBV;
	std::int32_t termSrcRBV;

	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0};
	std::ofstream resultsFile;
	resultsFile.open ("TP_NPD_0005.csv");

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test: TP_NPD_0005");
	std::cout<<RST;

	TestUtils::displayTitle("This test verifies terminal connection and signal inversion.\n"
							"Routing backplane clocks, syncing signal output and FTE clock generation.\n\n"
							"Will check next SRS's: \n"
							"\tSYNC-SRS-F-55\n"
							"\tSYNC-SRS-F-56\n"
							"\tSYNC-SRS-F-57\n"
							"\tSYNC-SRS-F-67\n"
							"\tSYNC-SRS-F-68\n"
							"\tSYNC-SRS-F-69\n"
							"\tSYNC-SRS-F-70\n"
							"\tSYNC-SRS-F-71");

	std::cout<<FCYN;
	TestUtils::waitEnterKey("Test ready. Press enter to Start");
	std::cout<<RST;

	//Create factory
	nds::Factory factory("test");
	try{
		const timespec* pReadTimestamp;
		nds::namedParameters_t parameters;
		parameters.emplace("TERMINALS","12"); //PXI_TRIG1 terminal
		// Create test device of type Device and named rootNode
		factory.createDevice("nisyncDevice", "rootNode", parameters);

		//Get instance of the Test Control System
		nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");


		////////////////////////////////////////////////////////////////
		///Switch ON Device
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON device...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);


		////////////////////////////////////////////////////////////////
		///Switch ON Routing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON Routing node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Switch ON Terminal Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON TR12 node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-TR12.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR12.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR12.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR12.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Switch ON Timing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON Timing node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Starting Routing Node
		////////////////////////////////////////////////////////////////
		//Change state:  ON -> (starting) -> RUNNING
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Configuring Routing Node
		////////////////////////////////////////////////////////////////
		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 1.Connect oscilloscope to PFI2 terminal. Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Routing terminal CLK10 to PFI2...\n";
		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_CLK10);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->writeCSValue("/rootNode-Routing.TermInvertSet", timestamp, (std::int32_t)NISYNC_DONT_INVERT);
		pInterface->writeCSValue("/rootNode-Routing.TermSyncSet", timestamp, (std::int32_t)NISYNC_ASYNCHRONOUS);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);

		pInterface->getPushedInt32("/rootNode-Routing.TermSetCode", pReadTimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);
		std::cout<<"Connection status: "<<statusRBV<<std::endl;

		pInterface->writeCSValue("/rootNode-Routing.TermDstRead", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->readCSValue("/rootNode-Routing.TermSrcRead", &timestamp, &termSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_CLK10, termSrcRBV);

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 2. Connect oscilloscope to PFI0 terminal. Press enter to continue.");
		std::cout<<RST;
		std::cout<<"Routing terminal CLK10 to PFI0...\n";
		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_CLK10);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_PFI0);
		pInterface->writeCSValue("/rootNode-Routing.TermInvertSet", timestamp, (std::int32_t)NISYNC_DONT_INVERT);
		pInterface->writeCSValue("/rootNode-Routing.TermSyncSet", timestamp, (std::int32_t)NISYNC_ASYNCHRONOUS);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);

		pInterface->getPushedInt32("/rootNode-Routing.TermSetCode", pReadTimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);
		std::cout<<"Connection status: "<<statusRBV<<std::endl;

		pInterface->writeCSValue("/rootNode-Routing.TermDstRead", timestamp, (std::int32_t)NISYNC_PFI0);
		pInterface->readCSValue("/rootNode-Routing.TermSrcRead", &timestamp, &termSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_CLK10, termSrcRBV);

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 3. Inverting PFI0 terminal. Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Inverting PFI0 terminal...\n";
		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_CLK10);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_PFI0);
		pInterface->writeCSValue("/rootNode-Routing.TermInvertSet", timestamp, (std::int32_t)NISYNC_INVERT);
		pInterface->writeCSValue("/rootNode-Routing.TermSyncSet", timestamp, (std::int32_t)NISYNC_ASYNCHRONOUS);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);

		pInterface->getPushedInt32("/rootNode-Routing.TermSetCode", pReadTimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);
		std::cout<<"Connection status: "<<statusRBV<<std::endl;

		pInterface->writeCSValue("/rootNode-Routing.TermDstRead", timestamp, (std::int32_t)NISYNC_PFI0);
		pInterface->readCSValue("/rootNode-Routing.TermSrcRead", &timestamp, &termSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_CLK10, termSrcRBV);

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 4. Disconnect the PFI0 connector from the oscilloscope and connect it to CLK_OUT.\n"
								"Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Routing BACKPLANE_CLK to CLK_OUT....\n";
		pInterface->writeCSValue("/rootNode-Routing.ClkSrc", timestamp, (std::int32_t)NISYNC_BACKPLANE_CLK);
		pInterface->writeCSValue("/rootNode-Routing.ClkDst", timestamp, (std::int32_t)NISYNC_CLKOUT);
		pInterface->writeCSValue("/rootNode-Routing.ClkSet", timestamp, (std::int32_t)1);

		pInterface->getPushedInt32("/rootNode-Routing.ClkSetCode", pReadTimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);

		pInterface->writeCSValue("/rootNode-Routing.ClkDstRead", timestamp, (std::int32_t)NISYNC_CLKOUT);
		pInterface->readCSValue("/rootNode-Routing.ClkSrcRead", &timestamp, &termSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_BACKPLANE_CLK, termSrcRBV);

		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_PXI_TRIG1);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->writeCSValue("/rootNode-Routing.TermInvertSet", timestamp, (std::int32_t)NISYNC_DONT_INVERT);
		pInterface->writeCSValue("/rootNode-Routing.TermSyncSet", timestamp, (std::int32_t)NISYNC_ASYNCHRONOUS);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);

		pInterface->getPushedInt32("/rootNode-Routing.TermSetCode", pReadTimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);
		std::cout<<"Connection status: "<<statusRBV<<std::endl;

		pInterface->writeCSValue("/rootNode-Routing.TermDstRead", timestamp, (std::int32_t)NISYNC_PFI0);
		pInterface->readCSValue("/rootNode-Routing.TermSrcRead", &timestamp, &termSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_CLK10, termSrcRBV);

		////////////////////////////////////////////////////////////////
		///Scheduling FTE clock on PXI_TRIG1. NOT SYNCED
		////////////////////////////////////////////////////////////////
		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 5. Scheduling FTE clock on PXI_TRIG1, which is routed to FPI2. NOT SYNCED.\n"
								"Press enter to continue.");
		std::cout<<RST;

		// Set/Get TerminalSet
		//pInterface->writeCSValue("/rootNode-TR12.FTE.TerminalSet",timestamp,(std::int32_t)NISYNC_PXI_TRIG1); // PVVariables are thread safe

		// Set/Get ModeSet
		std::int32_t modeSet;
		pInterface->writeCSValue("/rootNode-TR12.FTE.Enable",timestamp, (std::int32_t)1); // PVVariables are thread safe
        pInterface->writeCSValue("/rootNode-TR12.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::clk); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR12.FTE.ModeSet",&timestamp,&modeSet); // PVVariables are thread safe
        EXPECT_EQ((std::int32_t)setMode_t::clk,modeSet);

		// Set/Get LevelSet
		std::int32_t levelSet;
		pInterface->writeCSValue("/rootNode-TR12.FTE.LevelSet",timestamp,(std::int32_t)0); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR12.FTE.LevelSet",&timestamp,&levelSet); // PVVariables are thread safe
		EXPECT_EQ((std::int32_t)0,levelSet);

		timespec timeReadCS{0,0};
		pInterface->readCSValue("/rootNode-Timing.Time", &timestamp, &timeReadCS); //needed to push the data

		timeReadCS.tv_sec += 1;
		std::cout<<"StartTime: "<<timeReadCS.tv_sec<<std::endl;
		// Set/Get StartTimeSet
		pInterface->writeCSValue("/rootNode-TR12.FTE.StartTimeSet",timestamp,timeReadCS); // PVVariables are thread safe

		// Set/Get SopTimeSet
		timespec stopTimeSet{0,0};
		stopTimeSet.tv_sec = timeReadCS.tv_sec+120;
		std::cout<<"StopTime: "<<stopTimeSet.tv_sec<<std::endl;
		pInterface->writeCSValue("/rootNode-TR12.FTE.StopTimeSet",timestamp,stopTimeSet); // PVVariables are thread safe

		std::int32_t period = 100;
		//period
		pInterface->writeCSValue("/rootNode-TR12.FTE.PeriodNsecSet",timestamp,period); // PVVariables are thread safe

		//DutyCycle
		pInterface->writeCSValue("/rootNode-TR12.FTE.DutyCycleSet",timestamp,50); // PVVariables are thread safe

		//Set configuration
		pInterface->writeCSValue("/rootNode-TR12.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 6. Changing Period of previous clock...\n"
								"Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Changing period clock: \n";
		std::cout<<"\tOld period: "<<period<<" ns"<<std::endl;
		std::cout<<"\tNew period: "<<period*10000<<" ns"<<std::endl;

		//pInterface->writeCSValue("/rootNode-TR12.FTE.TerminalChgPeriod",timestamp,(std::int32_t)NISYNC_PXI_TRIG1); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR12.FTE.PeriodChgPeriod",timestamp,(std::int32_t)period*10000); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR12.FTE.ChgPeriod",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->getPushedInt32("/rootNode-TR12.FTE.ChgPeriodCode", pReadTimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR12.FTE.ChgPeriodStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);

		std::cout<<"Period changed...\n";


		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 7. Syncing PFI2 to backplane Rising Edges...\n"
								"Press enter to continue.");
		std::cout<<RST;

		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_PXI_TRIG1);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->writeCSValue("/rootNode-Routing.TermInvertSet", timestamp, (std::int32_t)NISYNC_DONT_INVERT);
		pInterface->writeCSValue("/rootNode-Routing.TermSyncSet", timestamp, (std::int32_t)NISYNC_SYNCHRONOUS);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);
		::sleep(20);

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 8. Disable clock generation. Press enter to continue.");
		std::cout<<RST;

		//pInterface->writeCSValue("/rootNode-TR12.FTE.TerminalSuppress",timestamp,(std::int32_t)NISYNC_PXI_TRIG1); // PVVariables are thread safe
        pInterface->writeCSValue("/rootNode-TR12.FTE.ModeSuppress",timestamp,(std::int32_t)suppressMode_t::clk); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR12.FTE.Suppress",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->getPushedInt32("/rootNode-TR12.FTE.SuppressCode", pReadTimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR12.FTE.SuppressStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);



		std::cout<<FGRN;
		TestUtils::waitEnterKey("Test complete. Press enter to switch off nds-nisync device.");
		std::cout<<RST;

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-TR12.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR12.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR12.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  RUNNING -> (stopping) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		resultsFile<<"TP_NPD_0005;"
				"This test verifies terminal connection and signal inversion. Routing backplane clocks, syncing signal output and FTE clock generation.;"
				"SYNC-SRS-F-55 SYNC-SRS-F-56 SYNC-SRS-F-57 SYNC-SRS-F-67 SYNC-SRS-F-68 SYNC-SRS-F-69 SYNC-SRS-F-70 SYNC-SRS-F-71"<<std::endl;
		resultsFile.close();

		// Destroy test device
		factory.destroyDevice("rootNode");

	}
	catch(const std::runtime_error& re)
	{
		// specific handling for runtime_error
		std::cerr << "Runtime error: " << re.what() << std::endl;
		factory.destroyDevice("rootNode");
		resultsFile.close();

	}
	catch(...){
		std::cout<<"Catched Unknown exception: "<<std::endl;
		factory.destroyDevice("rootNode");
		resultsFile.close();

	}

}

