/**
 * @file TP_OscilProcessing_0000.cpp
 * 
 * Test Plan: TP_OscilProcessing_0000
 * Test Description: Basic device creation and state machine transitions
 * 
 * @authors Test Suite
 * @date 2024
 */

#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>
#include <unistd.h>

TEST(TP_OscilProcessing, 0000)
{
    std::string testName = "TP_OscilProcessing_0000";
    std::string testDescription("Basic device creation and state machine transitions");

    std::cout << FCYN;
    TestUtils::displayTitle("\t\tExecuting test: " + testName);
    TestUtils::displayTitle(testDescription);
    std::cout << RST;

    // Create factory with test control system
    nds::Factory testFactory("test");

    nds::Factory::registerControlSystem(testFactory);

    // Create device parameters
    nds::namedParameters_t deviceParams;
    deviceParams.emplace("INIT", "OFF"); // Start in OFF state

    try {
        const timespec* pStateMachineSwitchTime;
	    const std::int32_t* pStateMachineState;
        timespec timestamp = {0, 0};

        // Create device
        TestUtils::displayTitle("Creating OscilProcessing device");
        testFactory.createDevice("OscilProcessing", "OscilProcessing-0", deviceParams);
        
        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("OscilProcessing-0");

        // Test initial state (should be OFF)
        TestUtils::displayTitle("Checking initial state");
        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        // Test switching ON
        TestUtils::displayTitle("Switching ON device");
        pInterface->writeCSValue("/OscilProcessing-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);

        ::sleep(1);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        // Test starting device
        TestUtils::displayTitle("Starting device");
        pInterface->writeCSValue("/OscilProcessing-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::running);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);

        ::sleep(1);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

        // Test stopping device
        TestUtils::displayTitle("Stopping device");
        pInterface->writeCSValue("/OscilProcessing-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);

        ::sleep(1);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        // Test switching OFF
        TestUtils::displayTitle("Switching OFF device");
        pInterface->writeCSValue("/OscilProcessing-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);

        ::sleep(1);

        pInterface->getPushedInt32("/OscilProcessing-0-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        // Cleanup
        testFactory.destroyDevice("OscilProcessing-0");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-0");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-0");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("OscilProcessing-0");
        FAIL();
    }
}