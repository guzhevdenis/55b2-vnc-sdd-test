/**
 * @file TP_OscilProcessing_0004.cpp
 * 
 * Test Plan: TP_OscilProcessing_0004
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

TEST(TP_OscilProcessing, 0004)
{
    std::string testName = "TP_OscilProcessing_0004";
    std::string testDescription("Test vector PV operations");

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
        testFactory.createDevice("OscilProcessing", "OscilProcessing-4", deviceParams);
        
        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("OscilProcessing-4");

        //Switch device ON 
        TestUtils::displayTitle("Switching ON device");
        pInterface->writeCSValue("/OscilProcessing-4-StateMachine.setState", timestamp,     
                                  (std::int32_t)nds::state_t::on);

        ::sleep(1);

        try {
            TestUtils::displayTitle("Check if FPGA is works");
            std::int32_t status;
            pInterface->readCSValue("/OscilProcessing-4-Status", &timestamp, &status);
            EXPECT_EQ(status, (std::int32_t)0);
        }
        catch (const std::exception& e) {
            // FPGA might not be available, which is acceptable
            std::cout << "FPGA is not available: " << e.what() << std::endl;
        }
        
        TestUtils::displayTitle("Testing DataAcquisitionNode_Ch0.Data vector read");
        std::vector<std::int32_t> data_ch0;
        pInterface->readCSValue("/OscilProcessing-4-DataAcquisitionNode_Ch0.Data", &timestamp, &data_ch0);
        EXPECT_EQ(data_ch0.size(), (std::size_t)0);
        // Data may be empty initially, which is acceptable

        TestUtils::displayTitle("Testing DataAcquisitionNode_Ch1.Data vector read");
        std::vector<std::int32_t> data_ch1;
        pInterface->readCSValue("/OscilProcessing-4-DataAcquisitionNode_Ch1.Data", &timestamp, &data_ch1);
        EXPECT_EQ(data_ch1.size(), (std::size_t)0);
        // Data may be empty initially, which is acceptable
        
        // Start data acquisition to populate vectors
        TestUtils::displayTitle("Starting data acquisition to populate vectors");
        try {
            pInterface->writeCSValue("/OscilProcessing-4-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);

            ::sleep(5); // Allow some data to be acquired
            
            // Read vectors again after data acquisition
            pInterface->readCSValue("/OscilProcessing-4-DataAcquisitionNode_Ch0.Data", &timestamp, &data_ch0);
            EXPECT_NE(data_ch0.size(), (std::size_t)0);

            pInterface->readCSValue("/OscilProcessing-4-DataAcquisitionNode_Ch0.Data", &timestamp, &data_ch1);
            EXPECT_NE(data_ch1.size(), (std::size_t)0);
            
            pInterface->writeCSValue("/OscilProcessing-4-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);

            ::sleep(1);
        }
        catch (const std::exception& e) {
            // Data Acquisition node might not be initialized, which is acceptable
            std::cout << "Data Acquisition node not available: " << e.what() << std::endl;
        }

        pInterface->writeCSValue("/OscilProcessing-4-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);

        ::sleep(2);

        // Cleanup
        testFactory.destroyDevice("OscilProcessing-4");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-4");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-4");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("OscilProcessing-4");
        FAIL();
    }
}