#include <ctime>
#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"

/**
 * Test verifies driver’s ability to read status of the firmware, the serial number, board model, device driver version, and the reset function.
 *
 * SYNC-SRS-F-01
 * SYNC-SRS-F-07
 * SYNC-SRS-F-08
 * SYNC-SRS-F-09
 * SYNC-SRS-F-10
 *
 */
TEST(TP_NPD,0000)
{
	std::ofstream resultsFile;
	const std::int32_t* pReadInt32;
	timespec timestamp = {0, 0};
	const timespec* pReadTimestamp;


	resultsFile.open ("TP_NPD_0000.csv");

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test: TP_NPD_0000");
	std::cout<<RST;
	TestUtils::displayTitle("This test verifies driver’s ability to read the serial number, board model, device driver version, and the reset function.\n\n"
			"Will check next SRS's: \n"
			"\tSYNC-SRS-F-01\n"
			"\tSYNC-SRS-F-07\n"
			"\tSYNC-SRS-F-08\n"
			"\tSYNC-SRS-F-09\n"
			"\tSYNC-SRS-F-10");
	std::cout<<FCYN;
	TestUtils::waitEnterKey("Test ready. Press enter to Start");
	std::cout<<RST;


	//Create factory
	nds::Factory factory("test");
	nds::namedParameters_t parameters;
	parameters.emplace("TERMINALS","0-2.7.8");
	// Create test device of type Device and named rootNode
	factory.createDevice("nisyncDevice", "rootNode", parameters);

	//Get instance of the Test Control System
	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	try{
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
		///Switch ON Firmware Node
		////////////////////////////////////////////////////////////////
		std::cout<<"Switching ON Firmware node...\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		std::string readValue;
		std::cout<<"Board Information: "<<std::endl;
		pInterface->readCSValue("/rootNode-Firm.SerialNumber",&timestamp, &readValue);
		std::cout<<"\tSerialNumber: "		<<readValue <<std::endl;
		pInterface->readCSValue("/rootNode-Firm.Model", &timestamp, &readValue);
		std::cout<<"\tModel: "			<<readValue <<std::endl;
		pInterface->readCSValue("/rootNode-Firm.DriverVersion", &timestamp, &readValue);
		std::cout<<"\tDriver Version: "	<<readValue <<std::endl;
		pInterface->readCSValue("/rootNode-Firm.HWRevision", &timestamp, &readValue);
		std::cout<<"\tHWRevision: "		<<readValue <<std::endl;

		//Change state:  ON -> (initializing) -> RUNNING
		pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pReadInt32);

		////////////////////////////////////////////////////////////////
		///Switching Off the device
		////////////////////////////////////////////////////////////////
		std::cout<<FGRN;
		TestUtils::waitEnterKey("Test complete. Press enter to switch off nds-nisync device.");
		std::cout<<RST;

		//Change state:  RUNNING -> (stopping) -> ON
		pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

      	//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

      	//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		resultsFile<<"testBoardInformation;"
				"TP_NPD_0000;"
				"Test verifies driver’s ability to read status of the firmware, the serial number, board model, device driver version, and the reset function.;"
				"SYNC-SRS-F-01\nSYNC-SRS-F-07\nSYNC-SRS-F-08\nSYNC-SRS-F-09\nSYNC-SRS-F-10"<<std::endl;
				//"SYNC-SRS-F-01 SYNC-SRS-F-07 SYNC-SRS-F-08 SYNC-SRS-F-09 SYNC-SRS-F-10"<<std::endl;
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
