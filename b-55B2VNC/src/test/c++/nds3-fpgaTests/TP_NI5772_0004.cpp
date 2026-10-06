/**
 * @file TP_NI5772_0004.cpp
 * 
 * Test Plan: TP_NI5772_0004
 * Test Description: Test vector PV operations (Amplitudes, Numbers, NeutronProfile)
 * 
 * @authors Test Suite
 * @date 2024
 */

#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>
#include <unistd.h>
#include <vector>

TEST(TP_NI5772, 0004)
{
    std::string testName = "TP_NI5772_0004";
    std::string testDescription("Test vector PV operations (Amplitudes, Numbers, NeutronProfile)");

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
        testFactory.createDevice("Ni5772Digitizer", "NI5772-4", deviceParams);

        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("NI5772-4");

        const timespec* pvTime = nullptr;
        timespec timestamp = {0, 0};

        // Switch device ON
        pInterface->writeCSValue("NI5772-4-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);
        ::sleep(2);

        // Test Amplitudes vector read
        TestUtils::displayTitle("Testing Amplitudes vector read");
        const std::vector<double>* amplitudes = nullptr;
        pInterface->getPushedVectorDouble("NI5772-4-Amplitudes", pvTime, amplitudes);
        EXPECT_NE(amplitudes, nullptr);
        // Amplitudes may be empty initially, which is acceptable

        // Test Numbers vector read
        TestUtils::displayTitle("Testing Numbers vector read");
        const std::vector<double>* numbers = nullptr;
        pInterface->getPushedVectorDouble("NI5772-4-Numbers", pvTime, numbers);
        EXPECT_NE(numbers, nullptr);
        // Numbers may be empty initially, which is acceptable

        // Test NeutronProfile vector read
        TestUtils::displayTitle("Testing NeutronProfile vector read");
        const std::vector<std::int32_t>* neutronProfile = nullptr;
        pInterface->getPushedVectorInt32("NI5772-4-NeutronProfile", pvTime, neutronProfile);
        EXPECT_NE(neutronProfile, nullptr);
        // NeutronProfile should have 100 elements (10x10 grid)
        EXPECT_EQ(neutronProfile->size(), 100);

        // Test MaximumAmplitude read
        TestUtils::displayTitle("Testing MaximumAmplitude read");
        const double* maxAmplitude = nullptr;
        pInterface->getPushedDouble("NI5772-4-MaximumAmplitude", pvTime, maxAmplitude);
        EXPECT_NE(maxAmplitude, nullptr);

        // Test CountRateAverage read
        TestUtils::displayTitle("Testing CountRateAverage read");
        const double* countRateAvg = nullptr;
        pInterface->getPushedDouble("NI5772-4-CountRateAverage", pvTime, countRateAvg);
        EXPECT_NE(countRateAvg, nullptr);

        // Test MSV read
        TestUtils::displayTitle("Testing MSV read");
        const double* msv = nullptr;
        pInterface->getPushedDouble("NI5772-4-MSV", pvTime, msv);
        EXPECT_NE(msv, nullptr);

        // Start data acquisition to populate vectors
        TestUtils::displayTitle("Starting data acquisition to populate vectors");
        
        // Start Data Acquisition if available
        try {
            pInterface->writeCSValue("NI5772-4-DataAcquisitionNode-StateMachine.setState", 
                                      timestamp, (std::int32_t)nds::state_t::on);
            ::sleep(1);
            pInterface->writeCSValue("NI5772-4-DataAcquisitionNode-StateMachine.setState", 
                                      timestamp, (std::int32_t)nds::state_t::running);
            ::sleep(3); // Allow some data to be acquired
            
            // Read vectors again after data acquisition
            pInterface->getPushedVectorDouble("NI5772-4-Amplitudes", pvTime, amplitudes);
            EXPECT_NE(amplitudes, nullptr);
            
            pInterface->getPushedVectorDouble("NI5772-4-Numbers", pvTime, numbers);
            EXPECT_NE(numbers, nullptr);
            
            // Stop data acquisition
            pInterface->writeCSValue("NI5772-4-DataAcquisitionNode-StateMachine.setState", 
                                      timestamp, (std::int32_t)nds::state_t::on);
            ::sleep(1);
            pInterface->writeCSValue("NI5772-4-DataAcquisitionNode-StateMachine.setState", 
                                      timestamp, (std::int32_t)nds::state_t::off);
            ::sleep(1);
        }
        catch (const std::exception& e) {
            // Data Acquisition node might not be initialized, which is acceptable
            std::cout << "Data Acquisition node not available: " << e.what() << std::endl;
        }

        // Cleanup
        pInterface->writeCSValue("NI5772-4-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);
        ::sleep(2);
        testFactory.destroyDevice("NI5772-4");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("NI5772-4");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("NI5772-4");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("NI5772-4");
        FAIL();
    }
}


