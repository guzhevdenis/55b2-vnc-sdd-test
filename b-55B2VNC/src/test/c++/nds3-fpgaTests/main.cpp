/**
 * @file main.cpp
 * 
 * Main entry point for NI5772Digitizer unit tests
 * 
 * @authors Test Suite
 * @date 2024
 */

#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestFactory.h>

int main(int argc, char **argv)
{
    // Create test control system factory
    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(
        new nds::tests::TestControlSystemFactoryImpl()));

    // Load the NI5772Digitizer driver library
    // Adjust path based on build configuration
    std::string driverPath = "../../../main/c++/nds3-ni5772/target/lib/libnds3-Ni5772Digitizer.so";
    
    try {
        nds::FactoryBaseImpl::loadDriver(driverPath);
    }
    catch (const std::exception& e) {
        std::cerr << "Warning: Could not load driver library: " << e.what() << std::endl;
        std::cerr << "Path attempted: " << driverPath << std::endl;
        std::cerr << "Some tests may fail if driver is not available." << std::endl;
    }

    // Register the test control system
    nds::Factory::registerControlSystem(testControlSystem);

    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // Run all tests
    return RUN_ALL_TESTS();
}


