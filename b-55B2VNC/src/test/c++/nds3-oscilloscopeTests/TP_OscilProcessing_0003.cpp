/**
 * @file TP_OscilProcessing_0003.cpp
 * 
 * Test Plan: TP_OscilProcessing_0003
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

TEST(TP_OscilProcessing, 0003)
{
    std::string testName = "TP_OscilProcessing_0003";
    std::string testDescription("Test helper functions and utility methods");

    std::cout << FCYN;
    TestUtils::displayTitle("\t\tExecuting test: " + testName);
    TestUtils::displayTitle(testDescription);
    std::cout << RST;

    // Create factory with test control system
    nds::Factory testFactory("test");

    nds::Factory::registerControlSystem(testFactory);

    // Create device parameters with Data Acquisition initialization
    nds::namedParameters_t deviceParams;
    deviceParams.emplace("INIT", "OFF"); // Enable Data Acquisition initialization

    try {
        timespec timestamp = {0, 0};

        // Create device
        //TestUtils::displayTitle("Creating OscilProcessing device");
        testFactory.createDevice("OscilProcessing", "OscilProcessing-3", deviceParams);
        
        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("OscilProcessing-3");

        //Switch device ON 
        TestUtils::displayTitle("Switching ON device");
        pInterface->writeCSValue("/OscilProcessing-3-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);

        ::sleep(2);
       
        // Test Data Acquisition Ch0 PV writes
        TestUtils::displayTitle("Testing Delta_Ch0");
        
        std::int32_t deltaValue = 0;
        pInterface->writeCSValue("/OscilProcessing-3-Delta_Ch0", timestamp, deltaValue);
        pInterface->readCSValue("/OscilProcessing-3-Delta_Ch0", &timestamp, &deltaValue);
        EXPECT_EQ((std::int32_t)0, deltaValue);
        ::usleep(100000);

        deltaValue = 1;
        pInterface->writeCSValue("/OscilProcessing-3-Delta_Ch0", timestamp, deltaValue);
        pInterface->readCSValue("/OscilProcessing-3-Delta_Ch0", &timestamp, &deltaValue);
        EXPECT_EQ((std::int32_t)1, deltaValue);
        ::usleep(100000);

        deltaValue = 1000;
        pInterface->writeCSValue("/OscilProcessing-3-Delta_Ch0", timestamp, deltaValue);
        pInterface->readCSValue("/OscilProcessing-3-Delta_Ch0", &timestamp, &deltaValue);
        EXPECT_EQ((std::int32_t)1000, deltaValue);
        ::usleep(100000);

        deltaValue = 2147483647;
        pInterface->writeCSValue("/OscilProcessing-3-Delta_Ch0", timestamp, deltaValue);
        pInterface->readCSValue("/OscilProcessing-3-Delta_Ch0", &timestamp, &deltaValue);
        EXPECT_EQ((std::int32_t)2147483647, deltaValue);
        ::usleep(100000);

        // Test device instance management
        TestUtils::displayTitle("Testing device instance management");

        testFactory.createDevice("OscilProcessing", "OscilProcessing-3B", deviceParams);
        ::sleep(1);

        pInterface->writeCSValue("/OscilProcessing-3-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);

        // Cleanup
        testFactory.destroyDevice("OscilProcessing-3");
        testFactory.destroyDevice("OscilProcessing-3B");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-3");
        testFactory.destroyDevice("OscilProcessing-3B");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-3");
        testFactory.destroyDevice("OscilProcessing-3B");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("OscilProcessing-3");
        testFactory.destroyDevice("OscilProcessing-3B");
        FAIL();
    }
}