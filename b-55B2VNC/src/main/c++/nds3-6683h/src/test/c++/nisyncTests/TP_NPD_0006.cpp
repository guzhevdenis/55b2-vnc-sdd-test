#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"


TEST(TP_NPD,0006)
{
	//Status and Code RBV
	std::string statusRBV;
	const std::int32_t* codeRBV;
	std::int32_t termSrcRBV;
	std::int32_t pendingValueRBV;

	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0};
	std::vector<nds::timestamp_t > receivedTS;


	//FTE configuring variables
	timespec timeReadCS{0,0};

	//Results file
	std::ofstream resultsFile;
	resultsFile.open ("TP_NPD_0006.csv");

	std::cout<<FCYN;
	TestUtils::displayTitle("Executing test: TP_NPD_0006");
	std::cout<<RST;

	TestUtils::displayTitle("Test the generation and removing of future time events, and checks the correct order of FTEs generated. Also tests pulse generation.\n\n"
			"-Will check next SRS's: \n"
			"\tSYNC-SRS-F-59\n"
			"\tSYNC-SRS-F-60\n"
			"\tSYNC-SRS-F-61\n"
			"\tSYNC-SRS-F-62\n"
			"\tSYNC-SRS-F-63\n"
			"\tSYNC-SRS-F-64\n"
			"\tSYNC-SRS-F-66");
	std::cout<<FCYN;
	TestUtils::waitEnterKey("Test ready. Press enter to Start");
	std::cout<<RST;

	//Create factory
	nds::Factory factory("test");
	try{
		const timespec* pStateMachineSwitchTime;
		const timespec* ptimestamp;
		nds::namedParameters_t parameters;
		/*
		 * Terminals:
		 *		2 - PFI2
		 *		15 - NISYNC_PXI_TRIG4 terminal
		 */
		parameters.emplace("TERMINALS","2.15");
		parameters.emplace("RESET","SOFT");

		// Create test device of type Device and named rootNode
		factory.createDevice("nisyncDevice", "rootNode", parameters);

		//Get instance of the Test Control System
		nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");


		////////////////////////////////////////////////////////////////
		///Switch ON Device
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON device...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
		//::sleep(2);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);


		////////////////////////////////////////////////////////////////
		///Switch ON Routing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON Routing node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Switch ON TR2-TS Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON TR2 (PFI2) node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Switch ON TR15 Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON TR15 (NISYNC_PXI_TRIG4)  node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-TR15.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR15.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR15.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR15.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Switch ON Timing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON Timing node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Starting Routing Node
		////////////////////////////////////////////////////////////////
		//Change state:  ON -> (starting) -> RUNNING
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Starting Timing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Starting Timing node...\n";
		//Change state:  ON -> (initializing) -> RUNNING
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Waiting for board synchronization
		////////////////////////////////////////////////////////////////
		int32_t readValue=0;
		int timeOut=0;
		std::cout<<"Waiting for board synchronization.";
        do{
        	try{
        		timeOut++;
        		::usleep(500000); // Wait for synchronization
        		std::cout<<"."<<std::flush;
        		pInterface->readCSValue("/rootNode-Timing.SyncStatus", &timestamp, &readValue);
        	}catch(...){}
        }while((readValue != 1 && readValue != 2 ) &&  timeOut<=60);
        std::cout<<std::endl;
        if(timeOut==60){
        	throw std::runtime_error("Time out board synchronization");
        }
		EXPECT_TRUE((readValue >= 1) &&  (readValue <= 2));


		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 1. Connect oscilloscope to PFI2 terminal. Set time scale to 2 seconds. Press enter to continue.");
		std::cout<<RST;

		////////////////////////////////////////////////////////////////
		///Configuring TR15.FTE Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Setting level of terminal NISYNC_PXI_TRIG4 to LOW...\n";
		pInterface->writeCSValue("/rootNode-TR15.FTE.Enable",timestamp, (std::int32_t)1); // PVVariables are thread safe
		//pInterface->writeCSValue("/rootNode-TR15.FTE.TerminalSet",timestamp,(std::int32_t)NISYNC_PXI_TRIG4); // PVVariables are thread safe
        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::inmLVL); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.LevelSet",timestamp,(std::int32_t)NISYNC_LEVEL_LOW); // PVVariables are thread safe

		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		////////////////////////////////////////////////////////////////
		///Configuring Routing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Routing terminal NISYNC_PXI_TRIG4 to PFI2...\n";
		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_PXI_TRIG4);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->writeCSValue("/rootNode-Routing.TermInvertSet", timestamp, (std::int32_t)NISYNC_DONT_INVERT);
		pInterface->writeCSValue("/rootNode-Routing.TermSyncSet", timestamp, (std::int32_t)NISYNC_ASYNCHRONOUS);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);

		pInterface->getPushedInt32("/rootNode-Routing.TermSetCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);
		std::cout<<"Connection status: "<<statusRBV<<std::endl;

		pInterface->writeCSValue("/rootNode-Routing.TermDstRead", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->readCSValue("/rootNode-Routing.TermSrcRead", &timestamp, &termSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_PXI_TRIG4, termSrcRBV);


		////////////////////////////////////////////////////////////////
		///Configuring Timestamping Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Timestamping configuration: "<<std::endl;

		std::int32_t decimationType = 0; //0==Software; 1==Hardware
		pInterface->writeCSValue("/rootNode-TR2.TS.DecimationType", timestamp, (std::int32_t)decimationType);
		std::cout<<"\tDecimation Type: "<<(decimationType>0?"Hardware":"Software")<<std::endl;

		std::int32_t decimation = 1;
//		pInterface->writeCSValue("/rootNode-TR2.TS.DecimationHW", timestamp, (std::int32_t)decimation);
//		std::cout<<"\tDecimation: "<<decimation<<std::endl;

		pInterface->writeCSValue("/rootNode-TR2.TS.Decimation", timestamp, (std::int32_t)decimation);
		std::cout<<"\tDecimation: "<<decimation<<std::endl;
		const std::int32_t* edgeRBV;
		pInterface->writeCSValue("/rootNode-TR2.TS.Edge", timestamp, (std::int32_t)NISYNC_EDGE_ANY);
		pInterface->getPushedInt32("/rootNode-TR2.TS.Edge_RBV",pStateMachineSwitchTime,edgeRBV); // PVVariables are thread safe
		EXPECT_EQ((std::int32_t)NISYNC_EDGE_ANY, *edgeRBV);
		std::cout<<"\tEdge: "<<(*edgeRBV==0?"Rising":(*edgeRBV==1?"Falling":(*edgeRBV==2?"Any":"Invalid")))<<std::endl;

		//const std::int32_t* enableRBV;
		//pInterface->writeCSValue("/rootNode-TR2.TS.Enable", timestamp, (std::int32_t)1);
		//pInterface->getPushedInt32("/rootNode-TR2.TS.Enable_RBV",pStateMachineSwitchTime,enableRBV); // PVVariables are thread safe
		//EXPECT_EQ((std::int32_t)1, *enableRBV);
		//std::cout<<"\tEnable: "<<(*enableRBV==0?"Disabled":"Enabled")<<std::endl;

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 2.Terminals Routed. Next, enabling Timestamps on PFI2. Press enter to continue");
		std::cout<<RST;

		////////////////////////////////////////////////////////////////
		///Starting TR2.TS Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Starting TR2 node...\n";
		//Change state:  ON -> (starting) -> RUNNING
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		////////////////////////////////////////////////////////////////
		///Configuring TR15.FTE Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Scheduling pulse..."<<std::endl;
//		pInterface->writeCSValue("/rootNode-TR15.FTE.TerminalSet",timestamp,(std::int32_t)NISYNC_PXI_TRIG4); // PVVariables are thread safe
        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::pulse); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-Timing.Time", &timestamp, &timeReadCS); //needed to push the data
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +5,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +6,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		timeOut=0;
		const nds::timestamp_t* receivedTimestamp;
		do{
			try{
				pInterface->getPushedTimestamp("/rootNode-TR2.TS.Timestamps",pStateMachineSwitchTime,receivedTimestamp); // PVVariables are thread safe
				receivedTS.push_back( *receivedTimestamp);
			}catch(...){
				timeOut++;
				sleep(1);
			};
		}while(timeOut<(20)  && receivedTS.size()<2);
		if(receivedTS.size()<2){
        	throw std::runtime_error("No timestamps received.");
		}
		std::cout<< "Pulse duration obtained from timestamps: "<<receivedTS[1].timestamp.tv_sec-receivedTS[0].timestamp.tv_sec<<" s"<<std::endl;
		receivedTS.clear();

		std::cout<<FCYN;
		TestUtils::displayTitle("Checkpoint 3.Scheduling 2 pulses. Reading TR15.FTE pending status.");
		std::cout<<RST;
		////////////////////////////////////////////////////////////////
		///Configuring TR15.FTE Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Scheduling pulse 1..."<<std::endl;
		//pInterface->writeCSValue("/rootNode-TR15.FTE.TerminalSet",timestamp,(std::int32_t)NISYNC_PXI_TRIG4); // PVVariables are thread safe
        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::pulse); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-Timing.Time", &timestamp, &timeReadCS); //needed to push the data
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +120,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +121,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		std::cout<<"Scheduling pulse 2..."<<std::endl;
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +130,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +131,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		std::cout<<"Scheduling single TR15.FTE..."<<std::endl;
        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::single); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.LevelSet",timestamp,(std::int32_t)NISYNC_LEVEL_HIGH); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +135,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe
		std::cout<<FBLU;
		std::cout<<"\nChecking if all TR15.FTE Levels are correct (with correct Levels)..."<<std::endl;
		std::cout<<RST;
		const std::int32_t* checkFTENumErrors;
		pInterface->writeCSValue("/rootNode-TR15.FTE.checkFTELevels",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->getPushedInt32("/rootNode-TR15.FTE.checkFTENumErrors", ptimestamp, checkFTENumErrors);
		pInterface->getPushedInt32("/rootNode-TR15.FTE.checkFTELevelsCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR15.FTE.checkFTELevelsStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *checkFTENumErrors);
		EXPECT_EQ((std::string)"OK", statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		std::cout<<"\tTR15.FTE Levels errors found: "<< *checkFTENumErrors<<std::endl;


		std::cout<<"\nScheduling single TR15.FTE (with one incorrect level)...\n"<<std::endl;

        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::single); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.LevelSet",timestamp,(std::int32_t)NISYNC_LEVEL_HIGH); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +136,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe
		std::cout<<FBLU;
		std::cout<<"Checking if all FTE Levels are correct (with one incorrect Level)..."<<std::endl;
		std::cout<<RST;
		pInterface->writeCSValue("/rootNode-TR15.FTE.checkFTELevels",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->getPushedInt32("/rootNode-TR15.FTE.checkFTENumErrors", ptimestamp, checkFTENumErrors);
		pInterface->readCSValue("/rootNode-TR15.FTE.checkFTELevelsStatus", &timestamp, &statusRBV);
		pInterface->getPushedInt32("/rootNode-TR15.FTE.checkFTELevelsCode", ptimestamp, codeRBV);
		EXPECT_EQ((std::int32_t)1, *checkFTENumErrors);
		EXPECT_EQ((std::string)"OK", statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		std::cout<<"\tFTE Levels errors found: "<< *checkFTENumErrors<<std::endl;


		pInterface->getPushedInt32("/rootNode-TR15.FTE.SetCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR15.FTE.SetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);

		pInterface->writeCSValue("/rootNode-TR15.FTE.TerminalPending",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingValue", &timestamp, &pendingValueRBV); //needed to push the data
		std::cout<<"\nPending value read: "<<pendingValueRBV<<std::endl;

		EXPECT_EQ((std::int32_t)6, pendingValueRBV);

		pInterface->getPushedInt32("/rootNode-TR15.FTE.PendingCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);



		std::cout<<FCYN;
		TestUtils::displayTitle("Checkpoint 4.Suppressing(Aborting) ALL FTEs before they happen.");
		std::cout<<RST;

		pInterface->writeCSValue("/rootNode-TR15.FTE.AllSuppress",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.Suppress",timestamp,(std::int32_t)1); // PVVariables are thread safe

		pInterface->writeCSValue("/rootNode-TR15.FTE.TerminalPending",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingValue", &timestamp, &pendingValueRBV); //needed to push the data
		std::cout<<"Pending value read: "<<pendingValueRBV<<std::endl;
		EXPECT_EQ((std::int32_t)0, pendingValueRBV);
		pInterface->getPushedInt32("/rootNode-TR15.FTE.PendingCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);

		pInterface->writeCSValue("/rootNode-TR15.FTE.AllSuppress",timestamp,(std::int32_t)0); //after suppress all FTEs clean the flag


		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 5. Generates 3 pulses. Suppressing(Aborting) ONE FTEs before they happen. Press enter to continue.");
		std::cout<<RST;

		/*
		 * This is done due to a possible bug in ni-sync
		 */
        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::inmLVL); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.LevelSet",timestamp,(std::int32_t)NISYNC_LEVEL_LOW); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		pInterface->readCSValue("/rootNode-Timing.Time", &timestamp, &timeReadCS); //needed to push the data
		std::cout<<"Scheduling pulse 1 ..."<<std::endl;
        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::pulse); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +8,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +9,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		std::cout<<"Scheduling pulse 2 ..."<<std::endl;
		pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::pulse); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +10,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +11,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		std::cout<<"Scheduling pulse 3 ..."<<std::endl;
		pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::pulse); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +12,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +13,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR15.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		pInterface->writeCSValue("/rootNode-TR15.FTE.TerminalPending",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingValue", &timestamp, &pendingValueRBV); //needed to push the data
		std::cout<<"Pending FTEs value read: "<<pendingValueRBV<<std::endl;
		EXPECT_EQ((std::int32_t)6, pendingValueRBV);
		pInterface->getPushedInt32("/rootNode-TR15.FTE.PendingCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);

		std::cout<<"Suppressing pulse 2 ..."<<std::endl;
		pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSuppress",timestamp,(std::int32_t)suppressMode_t::single); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSuppress",timestamp,(timespec){timeReadCS.tv_sec +10,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.Suppress",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.StartTimeSuppress",timestamp,(timespec){timeReadCS.tv_sec +11,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.Suppress",timestamp,(std::int32_t)1); // PVVariables are thread safe

		//No suppressing mode available
		std::cout << "Trying to suppress with an invalid suppress mode" << std::endl;
        pInterface->writeCSValue("/rootNode-TR15.FTE.ModeSuppress",timestamp,(std::int32_t)suppressMode_t::MAX_MODE_NUM); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR15.FTE.Suppress",timestamp,(std::int32_t)1); // PVVariables are thread safe

		pInterface->writeCSValue("/rootNode-TR15.FTE.TerminalPending",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingValue", &timestamp, &pendingValueRBV); //needed to push the data
		std::cout<<"Pending FTEs value read: "<<pendingValueRBV<<std::endl;
		EXPECT_EQ((std::int32_t)4, pendingValueRBV);
		pInterface->getPushedInt32("/rootNode-TR15.FTE.PendingCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR15.FTE.PendingStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);

        std::cout<<FYEL;
		TestUtils::displayTitle("Look at the oscilloscope for the 2 generated pulses.");
		std::cout<<RST;

		sleep(15);

        std::cout<<FGRN;
		TestUtils::waitEnterKey("Test complete. Press enter to switch off nds-nisync device.");
		std::cout<<RST;

		////////////////////////////////////////////////////////////////
		///Stopping TR2 Node
		////////////////////////////////////////////////////////////////
		//Change state:  RUNNING -> (stopping) -> ON
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  RUNNING -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-TR15.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR15.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR15.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		//Change state:  RUNNING -> (stopping) -> ON
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

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		resultsFile<<"TP_NPD_0006;"
				"Test the generation and removing of future time events, and checks the correct order of FTEs generated. Also tests pulse generation.;"
				"SYNC-SRS-F-59 SYNC-SRS-F-60 SYNC-SRS-F-61 SYNC-SRS-F-62 SYNC-SRS-F-63 SYNC-SRS-F-64 SYNC-SRS-F-65 SYNC-SRS-F-66" <<std::endl;
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

