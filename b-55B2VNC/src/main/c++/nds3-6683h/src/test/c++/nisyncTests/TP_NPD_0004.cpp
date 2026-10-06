#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"


TEST(TP_NPD,0004)
{

	const std::int32_t* pReadInt32;
	timespec timestamp = {0, 0};
	std::ofstream resultsFile;
	resultsFile.open ("TP_NPD_0004.csv");

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test: TP_NPD_0004");
	std::cout<<RST;

	TestUtils::displayTitle("This test routes signal from one terminal to other terminals. "
			"The source is the board clock. To check routing event timestamping is used.Event time stamping can be enabled or disabled.\n"
			"When board clock generates high frequency clock event time stamping decimation is used.\n\n"
	"-Will check next SRS's: \n"
			"\tSYNC-SRS-F-51\n"
			"\tSYNC-SRS-F-52\n"
			"\tSYNC-SRS-F-53\n"
			"\tSYNC-SRS-F-57\n"
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
		parameters.emplace("TERMINALS","2");
		// Create test device of type Device and named rootNode
		factory.createDevice("nisyncDevice", "rootNode", parameters);

		//Get instance of the Test Control System
		nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

		////////////////////////////////////////////////////////////////
		///Switch ON Device
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON rootnode...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);


		////////////////////////////////////////////////////////////////
		///Switch ON Routing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON Routing node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		////////////////////////////////////////////////////////////////
		///Switch ON TR2-TS Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON TR2 node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32,2000);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		////////////////////////////////////////////////////////////////
		///Starting Routing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Starting Routing node...\n";

		//Change state:  ON -> (starting) -> RUNNING
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pReadInt32);

		////////////////////////////////////////////////////////////////
		///Configuring Routing Node
		////////////////////////////////////////////////////////////////
		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 1.Connect oscilloscope to PFI2 terminal, and read routing configuration. Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Routing terminal CLK10 to PFI2...\n";
		pInterface->writeCSValue("/rootNode-Routing.TermSrc", timestamp, (std::int32_t)NISYNC_CLK10);
		pInterface->writeCSValue("/rootNode-Routing.TermDst", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->writeCSValue("/rootNode-Routing.TermInvertSet", timestamp, (std::int32_t)NISYNC_DONT_INVERT);
		pInterface->writeCSValue("/rootNode-Routing.TermSet", timestamp, (std::int32_t)1);

		std::string statusRBV;
		const std::int32_t* codeRBV;
		pInterface->getPushedInt32("/rootNode-Routing.TermSetCode", ptimestamp, codeRBV);
		pInterface->readCSValue("/rootNode-Routing.TermSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::int32_t)0, *codeRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);
		std::cout<<"Connection status: "<<statusRBV<<std::endl;

		std::int32_t termSrcRBV;
		pInterface->writeCSValue("/rootNode-Routing.TermDstRead", timestamp, (std::int32_t)NISYNC_PFI2);
		pInterface->readCSValue("/rootNode-Routing.TermSrcRead", &timestamp, &termSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_CLK10, termSrcRBV);


		////////////////////////////////////////////////////////////////
		///Configuring TR2.TS Node
		////////////////////////////////////////////////////////////////
		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 2.Enabling Timestamping on PFI2. Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Timestamp configuration: "<<std::endl;

		std::int32_t decimationType = 1; //0==Software; 1==Hardware
		pInterface->writeCSValue("/rootNode-TR2.TS.DecimationType", timestamp, (std::int32_t)decimationType);
		std::cout<<"\tDecimation Type: "<<(decimationType>0 ? "Hardware":"Software")<<std::endl;

		std::int32_t decimation = 1000000;
		pInterface->writeCSValue("/rootNode-TR2.TS.Decimation", timestamp, (std::int32_t)decimation);
		std::cout<<"\tDecimation: "<<decimation<<std::endl;

		const std::int32_t* edgeRBV;
		pInterface->writeCSValue("/rootNode-TR2.TS.Edge", timestamp, (std::int32_t)NISYNC_EDGE_RISING);
		pInterface->getPushedInt32("/rootNode-TR2.TS.Edge_RBV",pStateMachineSwitchTime,edgeRBV); // PVVariables are thread safe
		EXPECT_EQ((std::int32_t)NISYNC_EDGE_RISING, *edgeRBV);
		std::cout<<"\tEdge: "<<(*edgeRBV==0?"Rising":(*edgeRBV==1?"Falling":(*edgeRBV==2?"Any":"Invalid")))<<std::endl;

		////////////////////////////////////////////////////////////////
		///Starting TR2.TS Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Starting TR2 node...\n";
		//Change state:  ON -> (starting) -> RUNNING
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pReadInt32);

		std::cout<<"Reading timestamps for some time..."<<std::endl;
		///Waiting for timestamping in PFI2
		::sleep(1);

		////////////////////////////////////////////////////////////////
		///Stopping TR2.TS Node
		////////////////////////////////////////////////////////////////
		//Change state:  RUNNING -> (stopping) -> ON
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);


		const std::int32_t* NumberOfPushedDataBlocks;
		pInterface->getPushedInt32("/rootNode-TR2.TS.NumberOfPushedTimestamps",pStateMachineSwitchTime,NumberOfPushedDataBlocks); // PVVariables are thread safe
		std::cout<<"Timestamps read: "<< *NumberOfPushedDataBlocks<<std::endl;

		if(*NumberOfPushedDataBlocks>2){
			float freq=0;
			const nds::timestamp_t* timestamps;
			pInterface->getPushedTimestamp("/rootNode-TR2.TS.Timestamps",pStateMachineSwitchTime,timestamps); // PVVariables are thread safe
			nds::timestamp_t firstTs = *timestamps;
			pInterface->getPushedTimestamp("/rootNode-TR2.TS.Timestamps",pStateMachineSwitchTime,timestamps); // PVVariables are thread safe
			nds::timestamp_t secondTs= *timestamps;;

			float diffSec=0;
			float diffNsec=0;
			if((secondTs.timestamp.tv_nsec - firstTs.timestamp.tv_nsec)>0){
				diffSec = (secondTs.timestamp.tv_sec - firstTs.timestamp.tv_sec)*NSEC_PER_SEC;
				diffNsec = secondTs.timestamp.tv_nsec - firstTs.timestamp.tv_nsec;
				freq = 1/((diffSec + diffNsec)/NSEC_PER_SEC);
				std::cout << std::fixed;
				std::cout << std::setprecision(2);
				std::cout<< "Board Clock Frequency: "<<freq<<" Hz"<<std::endl;

			}else{
				diffSec = (secondTs.timestamp.tv_sec - firstTs.timestamp.tv_sec - 1)*NSEC_PER_SEC;
				diffNsec = NSEC_PER_SEC - firstTs.timestamp.tv_nsec + secondTs.timestamp.tv_nsec;
				freq = 1/((diffSec + diffNsec)/NSEC_PER_SEC);
				std::cout << std::fixed;
				std::cout << std::setprecision(2);
				std::cout<< "Board Clock Frequency (calculated with 2 consecutive timestamps): "<<freq<<" Hz"<<std::endl;
			}
		}

		std::cout<<FGRN;
		TestUtils::waitEnterKey("Test complete. Press enter to switch off nds-nisync device.");
		std::cout<<RST;

		//Change state:  RUNNING -> (stopping) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  ON -> (switching off) -> OFF
		pInterface->writeCSValue("/rootNode-TR2.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-TR2.StateMachine.getState", pStateMachineSwitchTime, pReadInt32,2000);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		resultsFile<<"TP_NPD_0004;"
				"Routes signal from one terminal to other terminals. The source is the board clock. To check routing event time stamping is used. "
				"Event time stamping can be enabled or disabled. When board clock generates high frequency clock event time stamping decimation is used;"
				"SYNC-SRS-F-51 SYNC-SRS-F-52 SYNC-SRS-F-53 SYNC-SRS-F-57 SYNC-SRS-F-72 SYNC-SRS-F-73 SYNC-SRS-F-74 SYNC-SRS-F-75 SYNC-SRS-F-76"
				"SYNC-SRS-F-77 SYNC-SRS-F-78 SYNC-SRS-F-79"<<std::endl;

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

