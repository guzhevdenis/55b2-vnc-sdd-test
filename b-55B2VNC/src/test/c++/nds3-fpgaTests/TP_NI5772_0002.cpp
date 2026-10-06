/**
 * @file TP_NI5772_0002.cpp
 * 
 * Test Plan: TP_NI5772_0002
 * Test Description: Test Data Acquisition node state machine and operations
 * 
 * @authors Test Suite
 * @date 2024
 */

#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>
#include <unistd.h>

TEST(TP_NI5772, 0002)
{
    std::string testName = "TP_NI5772_0002";
    std::string testDescription("Test Data Acquisition node state machine and operations");

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

    // Create device parameters with Data Acquisition initialization
    nds::namedParameters_t deviceParams;
    deviceParams.emplace("INIT", "YES"); // Enable Data Acquisition initialization

    try {
        // Create device
        testFactory.createDevice("Ni5772Digitizer", "NI5772-2", deviceParams);

        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("NI5772-2");

        const timespec* pvTime = nullptr;
        const std::int32_t* pvValue = nullptr;
        timespec timestamp = {0, 0};

        // Switch device ON
        TestUtils::displayTitle("Switching device ON");
        pInterface->writeCSValue("NI5772-2-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);
        ::sleep(2);

        // Test Data Acquisition node initial state
        TestUtils::displayTitle("Checking Data Acquisition node initial state");
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pvValue);

        // Test switching Data Acquisition ON
        TestUtils::displayTitle("Switching Data Acquisition ON");
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-StateMachine.setState", 
                                  timestamp, (std::int32_t)nds::state_t::on);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pvValue);

        // Test starting Data Acquisition
        TestUtils::displayTitle("Starting Data Acquisition");
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-StateMachine.setState", 
                                  timestamp, (std::int32_t)nds::state_t::running);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::starting, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::running, *pvValue);

        // Test Data Acquisition PV writes
        TestUtils::displayTitle("Testing Data Acquisition PV writes");
        
        double gainValue = 1.5;
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-Gain", timestamp, gainValue);
        ::sleep(1);

        double offsetValue = 0.1;
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-Offset", timestamp, offsetValue);
        ::sleep(1);

        double bandwidthValue = 1000000.0;
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-Bandwidth", timestamp, bandwidthValue);
        ::sleep(1);

        double resolutionValue = 16.0;
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-Resolution", timestamp, resolutionValue);
        ::sleep(1);

        std::int32_t couplingValue = 1;
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-Coupling", timestamp, couplingValue);
        ::sleep(1);

        double samplingRateValue = 100000000.0;
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-SamplingRate", timestamp, samplingRateValue);
        ::sleep(1);

        // Run for a short time to allow data acquisition
        TestUtils::displayTitle("Running data acquisition for 2 seconds");
        ::sleep(2);

        // Test stopping Data Acquisition
        TestUtils::displayTitle("Stopping Data Acquisition");
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-StateMachine.setState", 
                                  timestamp, (std::int32_t)nds::state_t::on);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pvValue);

        // Test switching Data Acquisition OFF
        TestUtils::displayTitle("Switching Data Acquisition OFF");
        pInterface->writeCSValue("NI5772-2-DataAcquisitionNode-StateMachine.setState", 
                                  timestamp, (std::int32_t)nds::state_t::off);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pvValue);
        ::sleep(1);
        pInterface->getPushedInt32("NI5772-2-DataAcquisitionNode-StateMachine.getState", 
                                    pvTime, pvValue);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pvValue);

        // Cleanup
        pInterface->writeCSValue("NI5772-2-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);
        ::sleep(2);
        testFactory.destroyDevice("NI5772-2");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("NI5772-2");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("NI5772-2");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("NI5772-2");
        FAIL();
    }
}


