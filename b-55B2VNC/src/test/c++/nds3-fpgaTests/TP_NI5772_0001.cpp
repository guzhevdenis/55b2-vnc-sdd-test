/**
 * @file TP_NI5772_0001.cpp
 * 
 * Test Plan: TP_NI5772_0001
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

TEST(TP_NI5772, 0001)
{
    std::string testName = "TP_NI5772_0001";
    std::string testDescription("Test Process Variable read/write operations");

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
    deviceParams.emplace("INIT", "ON");

    try {
        // Create device
        testFactory.createDevice("Ni5772Digitizer", "NI5772-1", deviceParams);

        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("NI5772-1");

        const timespec* pvTime = nullptr;
        timespec timestamp = {0, 0};

        // Switch device ON
        pInterface->writeCSValue("NI5772-1-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);
        ::sleep(2);

        // Test Status PV read
        TestUtils::displayTitle("Testing Status PV read");
        const std::int32_t* statusValue = nullptr;
        pInterface->getPushedInt32("NI5772-1-Status", pvTime, statusValue);
        EXPECT_NE(statusValue, nullptr);

        // Test Threshold write
        TestUtils::displayTitle("Testing Threshold write");
        std::int32_t thresholdValue = 1000;
        pInterface->writeCSValue("NI5772-1-Threshold", timestamp, thresholdValue);
        ::sleep(1);

        // Test Alpha16BitFirst write
        TestUtils::displayTitle("Testing Alpha16BitFirst write");
        std::int32_t alphaFirst = 500;
        pInterface->writeCSValue("NI5772-1-Alpha16BitFirst", timestamp, alphaFirst);
        ::sleep(1);

        // Test Alpha16BitSecond write
        TestUtils::displayTitle("Testing Alpha16BitSecond write");
        std::int32_t alphaSecond = 600;
        pInterface->writeCSValue("NI5772-1-Alpha16BitSecond", timestamp, alphaSecond);
        ::sleep(1);

        // Test FilterType write
        TestUtils::displayTitle("Testing FilterType write");
        std::int32_t filterType = 1;
        pInterface->writeCSValue("NI5772-1-FilterType", timestamp, filterType);
        ::sleep(1);

        // Test MaxTicksPerPulse write
        TestUtils::displayTitle("Testing MaxTicksPerPulse write");
        std::int64_t maxTicks = 10000;
        pInterface->writeCSValue("NI5772-1-MaxTicksPerPulse", timestamp, maxTicks);
        ::sleep(1);

        // Test MinTicksPerPulse write
        TestUtils::displayTitle("Testing MinTicksPerPulse write");
        std::int64_t minTicks = 100;
        pInterface->writeCSValue("NI5772-1-MinTicksPerPulse", timestamp, minTicks);
        ::sleep(1);

        // Test CalibrationCoefficient write
        TestUtils::displayTitle("Testing CalibrationCoefficient write");
        double calibrationCoeff = 1.234567;
        pInterface->writeCSValue("NI5772-1-CalibrationCoefficient", timestamp, calibrationCoeff);
        ::sleep(1);

        // Test Step write
        TestUtils::displayTitle("Testing Step write");
        std::int32_t stepValue = 200;
        pInterface->writeCSValue("NI5772-1-Step", timestamp, stepValue);
        ::sleep(1);

        // Test reading indicator PVs
        TestUtils::displayTitle("Testing indicator PV reads");
        
        const std::int32_t* counter1Value = nullptr;
        pInterface->getPushedInt32("NI5772-1-Counter1", pvTime, counter1Value);
        EXPECT_NE(counter1Value, nullptr);

        const std::int32_t* counter2Value = nullptr;
        pInterface->getPushedInt32("NI5772-1-Counter2", pvTime, counter2Value);
        EXPECT_NE(counter2Value, nullptr);

        const std::int32_t* deviceTempValue = nullptr;
        pInterface->getPushedInt32("NI5772-1-DeviceTemperature", pvTime, deviceTempValue);
        EXPECT_NE(deviceTempValue, nullptr);

        // Test Reseter write
        TestUtils::displayTitle("Testing Reseter write");
        std::int32_t resetValue = 1;
        pInterface->writeCSValue("NI5772-1-Reseter", timestamp, resetValue);
        ::sleep(1);

        // Cleanup
        pInterface->writeCSValue("NI5772-1-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);
        ::sleep(2);
        testFactory.destroyDevice("NI5772-1");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("NI5772-1");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("NI5772-1");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("NI5772-1");
        FAIL();
    }
}


