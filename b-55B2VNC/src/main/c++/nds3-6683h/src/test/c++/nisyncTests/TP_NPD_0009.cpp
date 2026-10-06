#include <iostream>
#include <cmath>

#include <gtest/gtest.h>

#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>
#include <algorithm>
#include <TimeTools.h>

#include "nds3/nisyncDevice.h"


std::string execAndGetStdOut(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe){
    	throw std::runtime_error("popen() failed!");
    }
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

TEST(TP_NPD,0009)
{

	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0};

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test: TP_NPD_0009");
	std::cout<<RST;

	TestUtils::displayTitle("Checking TAI to UTC conversion");

	std::cout<<FCYN;
	TestUtils::waitEnterKey("Test ready. Press enter to Start");
	std::cout<<RST;

	//Create factory
	nds::Factory factory("test");
	try{
		const timespec* pStateMachineSwitchTime;
		timespec timeReadCS{0,0};

		nds::namedParameters_t parameters;
		parameters.emplace("TERMINALS","0"); //PFI0 terminal

		// Create test device of type Device and named rootNode
		factory.createDevice("nisyncDevice", "rootNode", parameters);

		//Get instance of the Test Control System
		nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

		// Check initial state (OFF)
		pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

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

		//Get TAI time
		std::string strTAI = execAndGetStdOut("/opt/codac/tests/nisync/tools/get_time pxi6683h 0");
		//Get UTC time
		pInterface->readCSValue("/rootNode-Timing.Time", &timestamp, &timeReadCS); //needed to push the data
		strTAI.erase(std::remove(strTAI.begin(), strTAI.end(), '\n'),
				strTAI.end());

		int pos = strTAI.find('.');
		ASSERT_NE(pos, -1);
		unsigned long taiSec = std::stoul(strTAI.substr(0, pos));
		unsigned long tains = std::stoul(strTAI.substr(pos+1));
		unsigned long absTAITime = ccs::HelperTools::FromFractionalToAbsoluteTime(taiSec, tains);
		//unsigned long absUTCTime = ccs::HelperTools::FromFractionalToAbsoluteTime(timeReadCS.tv_sec, timeReadCS.tv_nsec);
		unsigned int offsetTAI2UTC = ccs::HelperTools::GetOffsetTAI2UTC(absTAITime);
		taiSec = std::round(taiSec + tains/1e9);
		unsigned long utcSec = std::round(timeReadCS.tv_sec + timeReadCS.tv_nsec/1e9);

		std::cout << "UTC time: " << timeReadCS.tv_sec << "." << timeReadCS.tv_nsec << " s" << std::endl;
		std::cout << "TAI time: " << strTAI << " s" << std::endl;
		std::cout << "Expected time offset (TAI-UTC): " << offsetTAI2UTC << " s" << std::endl;
		std::cout << "Actual time offset (TAI-UTC): " << taiSec - utcSec << " s" << std::endl;

		EXPECT_EQ(taiSec - utcSec, offsetTAI2UTC);

//		EXPECT_NEAR(absTAITime, ccs::HelperTools::ConvertUTC2TAI(absUTCTime), 0.005e9); //Difference less than 5 ms (both data are given in ns)

		//Change state:  ON -> (stopping) -> OFF
		pInterface->writeCSValue("/rootNode-Timing.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
		pInterface->getPushedInt32("/rootNode-Timing.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
		//Change state:  ON -> (stopping) -> OFF
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

