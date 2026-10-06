#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"


TEST(TP_NPD,0007)
{
	//Status and Code RBV
	std::string statusRBV;
	const std::int32_t* codeRBV;
	std::int32_t pendingValueRBV;

	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0};
	std::vector<nds::timestamp_t > receivedTS;


	//FTE configuring variables
	timespec timeReadCS{0,0};

	//Results file
	std::ofstream resultsFile;
	resultsFile.open ("TP_NPD_0007.csv");

	std::cout<<FCYN;
	TestUtils::displayTitle("Executing test: TP_NPD_0007");
	std::cout<<RST;

	TestUtils::displayTitle("Test the generation of FTEs and timestamps for the same terminal.\n"
			"-Will check next SRS's: \n"
			"\tSYNC-SRS-F-59\n"
			"\tSYNC-SRS-F-60\n"
			"\tSYNC-SRS-F-61\n"
			"\tSYNC-SRS-F-62\n"
			"\tSYNC-SRS-F-63\n"
			"\tSYNC-SRS-F-64\n"
			"\tSYNC-SRS-F-63\n"
			"\tSYNC-SRS-F-64\n"
			"\tSYNC-SRS-F-66\n"
			"\tSYNC-SRS-F-72\n"
			"\tSYNC-SRS-F-73\n"
			"\tSYNC-SRS-F-74\n"
			"\tSYNC-SRS-F-75\n"
			"\tSYNC-SRS-F-76\n"
			"\tSYNC-SRS-F-77\n"
			"\tSYNC-SRS-F-78\n"
			"\tSYNC-SRS-F-79");
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
		 */
		parameters.emplace("TERMINALS","2");
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

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
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
        		::sleep(0.5); // Wait for synchronization
        		std::cout<<"."<<std::flush;
        		pInterface->readCSValue("/rootNode-Timing.SyncStatus", &timestamp, &readValue);
        	}catch(...){}
        }while((readValue < 1 ||  readValue > 2) &&  timeOut<=60);
        std::cout<<std::endl;
        if(timeOut==60){
        	throw std::runtime_error("Time out board synchronization");
        }
		EXPECT_TRUE((readValue >= 1) &&  (readValue <= 2));


		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 1. Connect oscilloscope to PFI2 terminal. Set time scale to 1 seconds. Press enter to continue.");
		std::cout<<RST;

		////////////////////////////////////////////////////////////////
		///Configuring TR15.FTE Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Setting level of terminal NISYNC_PFI2 to LOW...\n";
		pInterface->writeCSValue("/rootNode-TR2.FTE.Enable",timestamp, (std::int32_t)1); // PVVariables are thread safe
        pInterface->writeCSValue("/rootNode-TR2.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::inmLVL); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR2.FTE.LevelSet",timestamp,(std::int32_t)NISYNC_LEVEL_LOW); // PVVariables are thread safe

		//Set configuration
		pInterface->writeCSValue("/rootNode-TR2.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		////////////////////////////////////////////////////////////////
		///Configuring Timestamping Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Timestamping configuration: "<<std::endl;

		std::int32_t decimationType = 0; //0==Software; 1==Hardware
		pInterface->writeCSValue("/rootNode-TR2.TS.DecimationType", timestamp, (std::int32_t)decimationType);
		std::cout<<"\tDecimation Type: "<<(decimationType>0?"Hardware":"Software")<<std::endl;

		std::int32_t decimation = 1;
//		pInterface->writeCSValue("/rootNode-TR2.TS.DecimationHW", timestamp, (std::int32_t)decimation);
		std::cout<<"\tDecimation: "<<decimation<<std::endl;

		pInterface->writeCSValue("/rootNode-TR2.TS.Decimation", timestamp, (std::int32_t)decimation);

		const std::int32_t* edgeRBV;
		pInterface->writeCSValue("/rootNode-TR2.TS.Edge", timestamp, (std::int32_t)NISYNC_EDGE_ANY);
		pInterface->getPushedInt32("/rootNode-TR2.TS.Edge_RBV",pStateMachineSwitchTime,edgeRBV); // PVVariables are thread safe
		EXPECT_EQ((std::int32_t)NISYNC_EDGE_ANY, *edgeRBV);
		std::cout<<"\tEdge: "<<(*edgeRBV==0?"Rising":(*edgeRBV==1?"Falling":(*edgeRBV==2?"Any":"Invalid")))<<std::endl;

		////////////////////////////////////////////////////////////////
		///Configuring FTEs for Terminal 2
		////////////////////////////////////////////////////////////////
		std::cout<<"Scheduling pulse..."<<std::endl;
        pInterface->writeCSValue("/rootNode-TR2.FTE.ModeSet",timestamp,(std::int32_t)setMode_t::pulse); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-Timing.Time", &timestamp, &timeReadCS); //needed to push the data

		////////////////////////////////////////////////////////////////
		/// Setting first pulse
		////////////////////////////////////////////////////////////////
		pInterface->writeCSValue("/rootNode-TR2.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +4,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR2.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +5,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR2.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		////////////////////////////////////////////////////////////////
		/// Setting second pulse
		////////////////////////////////////////////////////////////////
		pInterface->writeCSValue("/rootNode-TR2.FTE.StartTimeSet",timestamp,(timespec){timeReadCS.tv_sec +6,timeReadCS.tv_nsec}); // PVVariables are thread safe
		pInterface->writeCSValue("/rootNode-TR2.FTE.StopTimeSet",timestamp,(timespec){timeReadCS.tv_sec +8,timeReadCS.tv_nsec}); // PVVariables are thread safe
		//Set configuration
		pInterface->writeCSValue("/rootNode-TR2.FTE.Set",timestamp,(std::int32_t)1); // PVVariables are thread safe

		pInterface->writeCSValue("/rootNode-TR2.FTE.TerminalPending",timestamp,(std::int32_t)1); // PVVariables are thread safe
		pInterface->readCSValue("/rootNode-TR2.FTE.PendingValue", &timestamp, &pendingValueRBV); //needed to push the data
		std::cout<<"Pending FTEs value read: "<<pendingValueRBV<<std::endl;
		EXPECT_EQ((std::int32_t)4, pendingValueRBV);
		pInterface->getPushedInt32("/rootNode-TR2.FTE.PendingCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-TR2.FTE.PendingStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);

		////////////////////////////////////////////////////////////////
		///Starting TR2 Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Starting TR2 node...\n";
		//Change state:  ON -> (starting) -> RUNNING
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

		std::cout<<"Waiting for the timestamps...\n";

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
		}while(timeOut<30  && receivedTS.size()<(std::uint32_t)pendingValueRBV);
		if(receivedTS.size()<(std::uint32_t)pendingValueRBV){
        	throw std::runtime_error("No timestamps received.");
		}
		std::cout<< "First pulse duration obtained from timestamps: "<<receivedTS[1].timestamp.tv_sec-receivedTS[0].timestamp.tv_sec<<" s"<<std::endl;
		std::cout<< "Second pulse duration obtained from timestamps: "<<receivedTS[3].timestamp.tv_sec-receivedTS[2].timestamp.tv_sec<<" s"<<std::endl;
		receivedTS.clear();

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
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

		resultsFile<<"TP_NPD_0007\n"
				"Test the generation of FTEs and timestamps for the same terminal"<<std::endl;
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

