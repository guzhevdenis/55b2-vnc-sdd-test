#include <fstream>
#include <iostream>
#include <ctime>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"

/**
 * Reads board synchronization status and verifies sync, not-sync state changes.
 *
 * SYNC-SRS-F-28
 */
TEST(TP_NPD,0001)
{

	std::ofstream resultsFile;


	resultsFile.open ("TP_NPD_0001.csv");

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test: TP_NPD_0001");
	std::cout<<RST;
	TestUtils::displayTitle("This test Reads board synchronization status and verifies sync, not-sync state changes.\n\n"
			"Will check next SRS's: \n"
			"\tSYNC-SRS-F-20\n"
			"\tSYNC-SRS-F-28");
	std::cout<<FCYN;
	TestUtils::waitEnterKey("Test ready. Press enter to Start");
	std::cout<<RST;


	const std::int32_t* pReadInt32;
	timespec timestamp = {0, 0};

	nds::namedParameters_t parameters;
	parameters["devnum"]="0";
	parameters["devtype"]=NISYNC_DEVICE_TYPE_PXI6683H;

	//Create factory
	nds::Factory factory("test");

	// Create test device of type Device and named rootNode
	factory.createDevice("nisyncDevice", "rootNode", nds::namedParameters_t());

	//Get instance of the Test Control System
	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	try{
		const timespec* pReadTimestamp;
		////////////////////////////////////////////////////////////////
		///Switch ON Device
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON rootnode...\n";
		//	// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		////////////////////////////////////////////////////////////////
		///Switch ON Timing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON Timing node...\n";
		//	// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		////////////////////////////////////////////////////////////////
		///Switch ON Timing Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Starting Timing node...\n";
		//Change state:  ON -> (initializing) -> RUNNING
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pReadInt32);

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 1. Disconnect the Ethernet cable. Press enter to continue.");
		std::cout<<RST;

		////////////////////////////////////////////////////////////////
		///Getting synchronization status
		////////////////////////////////////////////////////////////////
		std::int32_t readValue=0;
		std::cout<<"Sync Information: "<<std::endl;
		pInterface->readCSValue("/rootNode-Timing.SyncStatus", &timestamp, &readValue);
		EXPECT_EQ((std::int32_t)NISYNC_LOST_MASTER, readValue);
		//EXPECT_TRUE((readValue >= 1) && (readValue <= 2));

		std::cout<<"\tSync Status: "<<readValue<<" ("
				<<(readValue==0?"NISYNC_INACTIVE_OR_NO_MASTER":readValue==1?"NISYNC_ACTIVE_MASTER":
					readValue==2?"NISYNC_WITHIN_GMC_OFFSET":"NISYNC_LOST_MASTER")<<")"<<std::endl;
		pInterface->readCSValue("/rootNode-Timing.SecsLastSync", &timestamp, &readValue);
		std::cout<<"\tSecsLastSync: "		<<readValue <<std::endl;

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 2. Connect the Ethernet cable. Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Waiting for synchronization ..."<<std::endl;
		//pInterface->readCSValue("/rootNode-Timing.SyncStatus", &timestamp, &readValue);

		int timeOut=0;
        do{
        	try{
        		pInterface->readCSValue("/rootNode-Timing.SyncStatus", &timestamp, &readValue);
        		timeOut++;
        		::sleep(1); // Wait for synchronization
        	}catch(...){}
        }while(readValue != 1 && readValue != 2 &&  timeOut<=60);
		EXPECT_TRUE((readValue >= 1) && (readValue <= 2));

		std::cout<<"Sync Information: "<<std::endl;
		std::cout<<"\tSync Status: "<<readValue<<" ("
				<<(readValue==0?"NISYNC_INACTIVE_OR_NO_MASTER":readValue==1?"NISYNC_ACTIVE_MASTER":
					readValue==2?"NISYNC_WITHIN_GMC_OFFSET":"NISYNC_LOST_MASTER")<<")"<<std::endl;
		pInterface->readCSValue("/rootNode-Timing.SecsLastSync", &timestamp, &readValue);
		std::cout<<"\tSecsLastSync: "		<<readValue <<std::endl;

		//Change state:  RUNNING -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		std::cout<<FGRN;
		TestUtils::waitEnterKey("Test complete. Press enter to switch off nds-nisync device.");
		std::cout<<RST;

      	//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

      	//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);



		resultsFile<<"TP_NPD_0001;"
				"Reads board synchronization status and verifies sync, not-sync state changes.;"
				"SYNC-SRS-F-20 SYNC-SRS-F-28"<<std::endl;
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

