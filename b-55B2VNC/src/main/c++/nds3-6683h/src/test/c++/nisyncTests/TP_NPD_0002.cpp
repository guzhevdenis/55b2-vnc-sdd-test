#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include "nds3/nisyncDevice.h"


TEST(TP_NPD,0002)
{
	const std::int32_t* pReadInt32;
	timespec timestamp = {0, 0};
	std::ofstream resultsFile;

	resultsFile.open ("TP_NPD_0002.csv");

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test: TP_NPD_0002");
	std::cout<<RST;

	TestUtils::displayTitle("This test verifies the possibility of routing board clock to external connector. \n"
			"And other Routing configurations. Physical connection is verified by oscilloscope.\n\n"
			"Will check next SRS's: \n"
			"\tSYNC-SRS-F-30\n"
			"\tSYNC-SRS-F-31\n"
			"\tSYNC-SRS-F-32");

	std::cout<<FCYN;
	TestUtils::waitEnterKey("Test ready. Press enter to Start");
	std::cout<<RST;

	//Create factory
	nds::Factory factory("test");
	try{
		const timespec* pReadTimestamp;
		// Create test device of type Device and named rootNode
		factory.createDevice("nisyncDevice", "rootNode", nds::namedParameters_t());

		//Get instance of the Test Control System
		nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

		std::cout<<"Switching ON device and Routing node\n\n";
		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);


		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  OFF -> (initializing) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		//Change state:  ON -> (starting) -> RUNNING
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *pReadInt32);

		std::cout<<FCYN;
		TestUtils::waitEnterKey("Checkpoint 1.Connect oscilloscope to CLK OUT terminal. Press enter to continue.");
		std::cout<<RST;

		std::cout<<"Routing BOARD_CLK to CLK_OUT\n";
		pInterface->writeCSValue("/rootNode-Routing.ClkSrc", timestamp, (std::int32_t)NISYNC_BOARD_CLK);
		pInterface->writeCSValue("/rootNode-Routing.ClkDst", timestamp, (std::int32_t)NISYNC_CLKOUT);
		pInterface->writeCSValue("/rootNode-Routing.ClkSet", timestamp, (std::int32_t)1);

		std::string statusRBV;
		std::int32_t codeRBV;
		pInterface->readCSValue("/rootNode-Routing.ClkSetStatus", &timestamp, &statusRBV);
		EXPECT_EQ((std::string)"OK", statusRBV);
		pInterface->readCSValue("/rootNode-Routing.ClkSetCode", &timestamp, &codeRBV);
		EXPECT_EQ((std::int32_t)0, codeRBV);
		std::cout<<"Connection status: "<<codeRBV<<std::endl;

		std::int32_t clkSrcRBV;
		pInterface->writeCSValue("/rootNode-Routing.ClkDstRead", timestamp, (std::int32_t)NISYNC_CLKOUT);
		pInterface->readCSValue("/rootNode-Routing.ClkSrcRead", &timestamp, &clkSrcRBV);
		EXPECT_EQ((std::int32_t)NISYNC_BOARD_CLK, clkSrcRBV);

		std::cout<<FGRN;
		TestUtils::waitEnterKey("Test complete. Press enter to switch off nds-nisync device.");
		std::cout<<RST;

		//Change state:  RUNNING -> (stopping) -> ON
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *pReadInt32);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-Routing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-Routing.StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		//Change state:  ON -> (switchingOff) -> OFF
		pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pReadInt32);
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pReadTimestamp, pReadInt32);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pReadInt32);

		resultsFile<<"testRouting;"
				"TP_NPD_0002;"
				"Test verifies the possibility of routing board clock to external connector. Physical connection is verified by oscilloscope.;"
				"SYNC-SRS-F-30 SYNC-SRS-F-31 SYNC-SRS-F-32"<<std::endl;

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
