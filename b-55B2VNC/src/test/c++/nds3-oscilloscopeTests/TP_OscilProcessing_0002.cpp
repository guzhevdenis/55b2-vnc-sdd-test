/**
 * @file TP_OscilProcessing_0002.cpp
 * 
 * Test Plan: TP_OscilProcessing_0002
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

TEST(TP_OscilProcessing, 0002)
{
    std::string testName = "TP_OscilProcessing_0002";
    std::string testDescription("Test Data Acquisition node state machine and operations");

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
        const timespec* pStateMachineSwitchTime;
	    const std::int32_t* pStateMachineState;
        timespec timestamp = {0, 0};

        // Create device
        //TestUtils::displayTitle("Creating OscilProcessing device");
        testFactory.createDevice("OscilProcessing", "OscilProcessing-2", deviceParams);
        
        // Get test control system interface
        nds::tests::TestControlSystemInterfaceImpl* pInterface = 
            nds::tests::TestControlSystemInterfaceImpl::getInstance("OscilProcessing-2");

        // Test initial state (should be OFF)
        TestUtils::displayTitle("Checking initial state");
        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        TestUtils::displayTitle("Checking initial Data Acquisition Node Ch0 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        TestUtils::displayTitle("Checking initial Data Acquisition Node Ch1 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        //Test switching device ON 
        TestUtils::displayTitle("Switching ON device");
        pInterface->writeCSValue("/OscilProcessing-2-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);

        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);
        
        //Data Acquisition Nodes should follow the root node
        TestUtils::displayTitle("Checking Data Acquisition Node Ch0 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        TestUtils::displayTitle("Checking Data Acquisition Node Ch1 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        // Test starting device
        TestUtils::displayTitle("Starting device");
        pInterface->writeCSValue("/OscilProcessing-2-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::running);

        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

        //Data Acquisition Nodes should follow the root node
        TestUtils::displayTitle("Checking Data Acquisition Node Ch0 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

        TestUtils::displayTitle("Checking Data Acquisition Node Ch1 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

        // Test Data Acquisition Ch0 PV writes
        TestUtils::displayTitle("Testing Data Acquisition Ch0 PV writes");

        double gainValue = 1.5;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Gain", timestamp, gainValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Gain_RBV", &timestamp, &gainValue);
        EXPECT_EQ((double)1.5, gainValue);
        ::usleep(100000);

        double offsetValue = 0.5;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Offset", timestamp, offsetValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Offset_RBV", &timestamp, &offsetValue);
        EXPECT_EQ((double)0.5, offsetValue);
        ::usleep(100000);

        double bandwidthValue = 1000000.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.BandWidth", timestamp, bandwidthValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.BandWidth_RBV", &timestamp, &bandwidthValue);
        EXPECT_EQ((double)1000000.0, bandwidthValue);
        ::usleep(100000);
        
        double resolutionValue = 16.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Resolution", timestamp, resolutionValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Resolution_RBV", &timestamp, &resolutionValue);
        EXPECT_EQ((double)16.0, resolutionValue);
        ::usleep(100000);

        double impedanceValue = 10.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Impedance", timestamp, impedanceValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Impedance_RBV", &timestamp, &impedanceValue);
        EXPECT_EQ((double)10.0, impedanceValue);
        ::usleep(100000);

        std::int32_t couplingValue = 1;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Coupling", timestamp, couplingValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Coupling_RBV", &timestamp, &couplingValue);
        EXPECT_EQ((std::int32_t)1, couplingValue);
        ::usleep(100000);

        std::int32_t signalRefTypeValue = 1;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.SignalRefType", timestamp, signalRefTypeValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.SignalRefType_RBV", &timestamp, &signalRefTypeValue);
        EXPECT_EQ((std::int32_t)1, signalRefTypeValue);
        ::usleep(100000);

        std::int32_t groundValue = 10;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Ground", timestamp, groundValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.Ground_RBV", &timestamp, &groundValue);
        EXPECT_EQ((std::int32_t)10, groundValue);
        ::usleep(100000);

        std::int32_t DMAEnableValue = 1;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.DMAEnable", timestamp, DMAEnableValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.DMAEnable_RBV", &timestamp, &DMAEnableValue);
        EXPECT_EQ((std::int32_t)1, DMAEnableValue);
        ::usleep(100000);

        double samplingRateValue = 100000000.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.SamplingRate", timestamp, samplingRateValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch0.SamplingRate_RBV", &timestamp, &samplingRateValue);
        EXPECT_EQ((double)100000000.0, samplingRateValue);
        ::usleep(100000);

        // Test Data Acquisition Ch1 PV writes
        TestUtils::displayTitle("Testing Data Acquisition Ch1 PV writes");

        gainValue = 1.5;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Gain", timestamp, gainValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Gain_RBV", &timestamp, &gainValue);
        EXPECT_EQ((double)1.5, gainValue);
        ::usleep(100000);

        offsetValue = 0.5;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Offset", timestamp, offsetValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Offset_RBV", &timestamp, &offsetValue);
        EXPECT_EQ((double)0.5, offsetValue);
        ::usleep(100000);

        bandwidthValue = 1000000.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.BandWidth", timestamp, bandwidthValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.BandWidth_RBV", &timestamp, &bandwidthValue);
        EXPECT_EQ((double)1000000.0, bandwidthValue);
        ::usleep(100000);
        
        resolutionValue = 16.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Resolution", timestamp, resolutionValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Resolution_RBV", &timestamp, &resolutionValue);
        EXPECT_EQ((double)16.0, resolutionValue);
        ::usleep(100000);

        impedanceValue = 10.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Impedance", timestamp, impedanceValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Impedance_RBV", &timestamp, &impedanceValue);
        EXPECT_EQ((double)10.0, impedanceValue);
        ::usleep(100000);

        couplingValue = 1;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Coupling", timestamp, couplingValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Coupling_RBV", &timestamp, &couplingValue);
        EXPECT_EQ((std::int32_t)1, couplingValue);
        ::usleep(100000);

        signalRefTypeValue = 1;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.SignalRefType", timestamp, signalRefTypeValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.SignalRefType_RBV", &timestamp, &signalRefTypeValue);
        EXPECT_EQ((std::int32_t)1, signalRefTypeValue);
        ::usleep(100000);

        groundValue = 10;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Ground", timestamp, groundValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.Ground_RBV", &timestamp, &groundValue);
        EXPECT_EQ((std::int32_t)10, groundValue);
        ::usleep(100000);

        DMAEnableValue = 1;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.DMAEnable", timestamp, DMAEnableValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.DMAEnable_RBV", &timestamp, &DMAEnableValue);
        EXPECT_EQ((std::int32_t)1, DMAEnableValue);
        ::usleep(100000);

        samplingRateValue = 100000000.0;
        pInterface->writeCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.SamplingRate", timestamp, samplingRateValue);
        pInterface->readCSValue("/OscilProcessing-2-DataAcquisitionNode_Ch1.SamplingRate_RBV", &timestamp, &samplingRateValue);
        EXPECT_EQ((double)100000000.0, samplingRateValue);
        ::usleep(100000);

        // Run for a short time to allow data acquisition
        TestUtils::displayTitle("Running data acquisition for 2 seconds");
        ::sleep(2);

        // Test stopping device
        TestUtils::displayTitle("Stopping device");
        pInterface->writeCSValue("/OscilProcessing-2-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::on);

        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        TestUtils::displayTitle("Checking Data Acquisition Node Ch0 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        TestUtils::displayTitle("Checking Data Acquisition Node Ch1 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

        // Test switching OFF
        TestUtils::displayTitle("Switching OFF device");
        pInterface->writeCSValue("/OscilProcessing-2-StateMachine.setState", timestamp, 
                                  (std::int32_t)nds::state_t::off);

        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        TestUtils::displayTitle("Checking Data Acquisition Node Ch0 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch0.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        TestUtils::displayTitle("Checking Data Acquisition Node Ch1 state");
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
        ::sleep(1);
        pInterface->getPushedInt32("/OscilProcessing-2-DataAcquisitionNode_Ch1.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
        EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

        // Cleanup
        testFactory.destroyDevice("OscilProcessing-2");
    }
    catch (const std::runtime_error& re) {
        std::cerr << "Runtime error: " << re.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-2");
        FAIL();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        testFactory.destroyDevice("OscilProcessing-2");
        FAIL();
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        testFactory.destroyDevice("OscilProcessing-2");
        FAIL();
    }
}