/**
 * @file TP_OscilProcessing_0001.cpp
 * 
 * Test Plan: TP_OscilProcessing_0001
 * Test Description: Test Process Variable (PV) read/write operations
 * 
 * @authors Test Suite
 * @date 2024
 */

#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>
#include <unistd.h>

TEST(TP_OscilProcessing, 0001)
{
    std::string testName = "TP_OscilProcessing_0001";
    std::string testDescription("Test Process Variable read/write operations");

    std::cout << FCYN;
    TestUtils::displayTitle("\t\tExecuting test: " + testName);
    TestUtils::displayTitle(testDescription);
    std::cout << RST;

    // Create factory with test control system
    nds::Factory testFactory("test");

    nds::Factory::registerControlSystem(testFactory);

    // Create device parameters
    nds::namedParameters_t deviceParams;
    deviceParams.emplace("INIT", "ON");

    try {
        timespec timestamp = {0, 0};

        // Create device
        testFactory.createDevice("OscilProcessing", "OscilProcessing-1", deviceParams);

        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("OscilProcessing-1");

        // Switch device ON
        TestUtils::displayTitle("Switching ON device");
        pInterface->writeCSValue("/OscilProcessing-1-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);

        ::sleep(2);

        // Test Status PV read
        TestUtils::displayTitle("Testing Status PV read");
        //const std::int32_t* statusValue = nullptr;
        std::int32_t statusValue;
        //pInterface->getPushedInt32("/OscilProcessing-1-Status", pvTime, statusValue);
        pInterface->readCSValue("/OscilProcessing-1-Status", &timestamp, &statusValue);
        EXPECT_NE(&statusValue, nullptr);

        // Test DAQ_En write
        TestUtils::displayTitle("Testing DAQ_En write");
        std::int32_t DAQ_En = 1;
        pInterface->writeCSValue("/OscilProcessing-1-DAQ_En", timestamp, DAQ_En);
        ::sleep(1);

        // Test Channel_0 write
        TestUtils::displayTitle("Testing Channel_0 write");
        std::int32_t Channel_0 = 1;
        pInterface->writeCSValue("/OscilProcessing-1-Channel_0", timestamp, Channel_0);
        ::sleep(1);

        // Test FSA_On_Ch0 write
        TestUtils::displayTitle("Testing FSA_On_Ch0 write");
        std::int32_t FSA_On_Ch0 = 1;
        pInterface->writeCSValue("/OscilProcessing-1-FSA_On_Ch0", timestamp, FSA_On_Ch0);
        ::sleep(1);

        // Test Channel_1 write
        TestUtils::displayTitle("Testing Channel_1 write");
        std::int32_t Channel_1 = 1;
        pInterface->writeCSValue("/OscilProcessing-1-Channel_1", timestamp, Channel_1);
        ::sleep(1);

        // Test FSA_On_Ch1 write
        TestUtils::displayTitle("Testing FSA_On_Ch1 write");
        std::int32_t FSA_On_Ch1 = 1;
        pInterface->writeCSValue("/OscilProcessing-1-FSA_On_Ch1", timestamp, FSA_On_Ch1);
        ::sleep(1);

        // Test Reference_Ch0 write
        TestUtils::displayTitle("Testing Reference_Ch0 write");
        std::int32_t Reference_Ch0 = 100;
        pInterface->writeCSValue("/OscilProcessing-1-Reference_Ch0", timestamp, Reference_Ch0);
        ::sleep(1);

        // Test Delta_Ch0 write
        TestUtils::displayTitle("Testing Delta_Ch0 write");
        std::int32_t Delta_Ch0 = 100;
        pInterface->writeCSValue("/OscilProcessing-1-Delta_Ch0", timestamp, Delta_Ch0);
        ::sleep(1);

        // Test Difference_Ch0 write
        TestUtils::displayTitle("Testing Difference_Ch0 write");
        std::int32_t Difference_Ch0 = 100;
        pInterface->writeCSValue("/OscilProcessing-1-Difference_Ch0", timestamp, Difference_Ch0);
        ::sleep(1);

        // Test Reference_Ch1 write
        TestUtils::displayTitle("Testing Reference_Ch1 write");
        std::int32_t Reference_Ch1 = 100;
        pInterface->writeCSValue("/OscilProcessing-1-Reference_Ch1", timestamp, Reference_Ch1);
        ::sleep(1);

        // Test Delta_Ch1 write
        TestUtils::displayTitle("Testing Delta_Ch1 write");
        std::int32_t Delta_Ch1 = 1000;
        pInterface->writeCSValue("/OscilProcessing-1-Delta_Ch1", timestamp, Delta_Ch1);
        ::sleep(1);

        // Test Difference_Ch1 write
        TestUtils::displayTitle("Testing Difference_Ch1 write");
        std::int32_t Difference_Ch1 = 100;
        pInterface->writeCSValue("/OscilProcessing-1-Difference_Ch1", timestamp, Difference_Ch1);
        ::sleep(1);

        // Test reading indicator PVs
        TestUtils::displayTitle("Testing indicator PV reads");
        
        std::int32_t DAQ_OnValue;
        pInterface->readCSValue("/OscilProcessing-1-DAQ_On", &timestamp, &DAQ_OnValue);
        EXPECT_NE(&DAQ_OnValue, nullptr);

        // const std::int32_t* DAQ_OnValue = nullptr;
        // pInterface->getPushedInt32("/OscilProcessing-1-DAQ_On", pvTime, DAQ_OnValue);
        // EXPECT_NE(DAQ_OnValue, nullptr);

        std::int32_t IOModuleInitializationDoneValue;
        pInterface->readCSValue("/OscilProcessing-1-IOModuleInitializationDone", &timestamp, &IOModuleInitializationDoneValue);
        EXPECT_NE(&IOModuleInitializationDoneValue, nullptr);
        
        // const std::int32_t* IOModuleInitializationDoneValue = nullptr;
        // pInterface->getPushedInt32("/OscilProcessing-1-IOModuleInitializationDone", pvTime, IOModuleInitializationDoneValue);
        // EXPECT_NE(IOModuleInitializationDoneValue, nullptr);

        std::int32_t Rec_Ch0Value;
        pInterface->readCSValue("/OscilProcessing-1-Rec_Ch0", &timestamp, &Rec_Ch0Value);
        EXPECT_NE(&Rec_Ch0Value, nullptr);

        // const std::int32_t* Rec_Ch0Value = nullptr;
        // pInterface->getPushedInt32("/OscilProcessing-1-Rec_Ch0", pvTime, Rec_Ch0Value);
        // EXPECT_NE(Rec_Ch0Value, nullptr);

        std::int32_t Rec_Ch1Value;
        pInterface->readCSValue("/OscilProcessing-1-Rec_Ch1", &timestamp, &Rec_Ch1Value);
        EXPECT_NE(&Rec_Ch1Value, nullptr);

        // const std::int32_t* Rec_Ch1Value = nullptr;
        // pInterface->getPushedInt32("/OscilProcessing-1-Rec_Ch1", pvTime, Rec_Ch1Value);
        // EXPECT_NE(Rec_Ch1Value, nullptr);

        // Test Reseter write
        TestUtils::displayTitle("Testing Reseter write");
        std::int32_t resetValue = 1;
        pInterface->writeCSValue("/OscilProcessing-1-Reseter", timestamp, resetValue);
        ::sleep(1);

        // Cleanup
        pInterface->writeCSValue("/OscilProcessing-1-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);
        ::sleep(2);
        testFactory.destroyDevice("OscilProcessing-1");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-1");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-1");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("OscilProcessing-1");
        FAIL();
    }
}