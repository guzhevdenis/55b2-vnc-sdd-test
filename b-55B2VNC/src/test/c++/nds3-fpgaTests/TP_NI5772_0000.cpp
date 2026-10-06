/**
 * @file TP_NI5772_0000.cpp
 * 
 * Test Plan: TP_NI5772_0000
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

TEST(TP_NI5772, 0000)
{
    std::string testName = "TP_NI5772_0000";
    std::string testDescription("Basic device creation and state machine transitions");

    std::cout << FCYN;
    TestUtils::displayTitle("\t\tExecuting test: " + testName);
    TestUtils::displayTitle(testDescription);
    std::cout << RST;

    // Create factory with test control system
    nds::Factory testFactory(std::shared_ptr<nds::FactoryBaseImpl>(
        new nds::tests::TestControlSystemFactoryImpl()));

    // Load the driver library
    std::string pwd = get_current_dir_name();
    std::string driverPath = pwd + "/../../../../main/c++/nds3-ni5772/target/lib/libnds3-Ni5772Digitizer.so";
    nds::FactoryBaseImpl::loadDriver(driverPath);

    nds::Factory::registerControlSystem(testFactory);

    // Create device parameters
    nds::namedParameters_t deviceParams;
    deviceParams.emplace("INIT", "OFF"); // Start in OFF state

    try {
        // Create device
        TestUtils::displayTitle("Creating NI5772Digitizer device");
        testFactory.createDevice("Ni5772Digitizer", "NI5772-0", deviceParams);

        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("NI5772-0");

        const timespec* pvTime = nullptr;
        const std::int32_t* pvValue = nullptr;
        timespec timestamp = {0, 0};

        // Test initial state (should be OFF)
        TestUtils::displayTitle("Checking initial state");
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pvValue);

        // Test switching ON
        TestUtils::displayTitle("Switching ON device");
        pInterface->writeCSValue("NI5772-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pvValue);

        // Test starting device
        TestUtils::displayTitle("Starting device");
        pInterface->writeCSValue("NI5772-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::running);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::starting, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::running, *pvValue);

        // Test stopping device
        TestUtils::displayTitle("Stopping device");
        pInterface->writeCSValue("NI5772-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pvValue);

        // Test switching OFF
        TestUtils::displayTitle("Switching OFF device");
        pInterface->writeCSValue("NI5772-0-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-0-StateMachine.getState", pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pvValue);

        // Cleanup
        testFactory.destroyDevice("NI5772-0");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("NI5772-0");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("NI5772-0");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("NI5772-0");
        FAIL();
    }
}

