#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>
#include <istream>

#include <ctime>

TEST(TP_NDS_SDN,0001)
{
	std::string testName="TP_NDS_SDN_0001";
	std::string testDescription("");

	std::cout<<FCYN;
	TestUtils::displayTitle("\t\tExecuting test:"+testName);
	TestUtils::displayTitle(testDescription);
	std::cout<<RST;

	//Create factory
	nds::Factory factory("test");
	std::ifstream rules ("iniSeparators.txt", std::ifstream::binary);
	factory.loadNamingRules(rules);
	std::string pwd = get_current_dir_name();
	char* actualpath;

	//Create device DeviceTesting
	nds::namedParameters_t deviceTesting;
	deviceTesting.emplace("PV_CONF",pwd+"/../../../main/resources/test-nds-ArrayPVs.xml");
	actualpath= realpath(deviceTesting["PV_CONF"].c_str(), NULL);
	std::cout<<"XML PATH for the Testing Device: "<<std::string(actualpath)<<std::endl;
	free(actualpath);

	// Create test device of type DeviceTesting and named Test-0
	factory.createDevice("DeviceTesting", "Test-0", deviceTesting);

	//Create device DeviceNDS-SDNPUB
	nds::namedParameters_t ndssdnParams;
	ndssdnParams.emplace("INTERFACE", "lo");
	ndssdnParams.emplace("PVS_CONFIG_PATH", pwd+"/../../../main/resources/input_arrays.xml");
	actualpath= realpath(ndssdnParams["PVS_CONFIG_PATH"].c_str(), NULL);
	std::cout<<"XML PATH for the SDN Device: "<<std::string(actualpath)<<std::endl;
	free(actualpath);

	// Create test device of type DeviceNDSSDNPUB and named SDN-0
	factory.createDevice("DeviceNDSSDNPUB", "SDN-0", ndssdnParams);

	//Get instance of the Test Control System with the TestUtils
	nds::tests::TestControlSystemInterfaceImpl* m_pInterfaceSDN = nds::tests::TestControlSystemInterfaceImpl::getInstance("SDN-0");
	nds::tests::TestControlSystemInterfaceImpl* m_pInterfacePVs = nds::tests::TestControlSystemInterfaceImpl::getInstance("Test-0");
	const timespec* m_pvTime = NULL;
	const std::int32_t* m_pvValue = NULL;
	timespec m_timestamp = {0, 0};

	try {
		/**
		 * Switch ON TestPVs Node
		 */
		TestUtils::displayTitle("Switching ON Test-0 Node");
		m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		m_pInterfacePVs->writeCSValue("Test-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::on);
		::sleep(1);
		m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *m_pvValue);
		::sleep(1);
		m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *m_pvValue);

		/**
		 * Switch ON SDN Node
		 */
		TestUtils::displayTitle("Switching ON SDN-0 Node");
		m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		m_pInterfaceSDN->writeCSValue("SDN-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::on);
		::sleep(1);
		m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *m_pvValue);
		::sleep(1);
		m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *m_pvValue);

		/**
		 * Running SDN Node
		 */
		TestUtils::displayTitle("Starting SDN-0 Node");
		m_pInterfaceSDN->writeCSValue("SDN-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::running);
		::sleep(1);
		m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *m_pvValue);
		::sleep(1);
		m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *m_pvValue);

		/**
		 * Running TestPVs Node
		 */
		TestUtils::displayTitle("Starting Test-0 Node");
		m_pInterfacePVs->writeCSValue("Test-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::running);
		::sleep(1);
		m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *m_pvValue);
		::sleep(1);
		m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *m_pvValue);

		 sleep(20);

		 /**
		  * Stopping TestPVs Node
		  */
		 TestUtils::displayTitle("Stopping Test-0 Node");
		 m_pInterfacePVs->writeCSValue("Test-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::on);
		 ::sleep(1);
		 m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::stopping, *m_pvValue);
		 ::sleep(1);
		 m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::on, *m_pvValue);

		 /**
		  * Stopping SDN Node
		  */
		 TestUtils::displayTitle("Stopping SDN-0 Node");
		 m_pInterfaceSDN->writeCSValue("SDN-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::on);
		 ::sleep(1);
		 m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::stopping, *m_pvValue);
		 ::sleep(1);
		 m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::on, *m_pvValue);

		 /**
		  * Switching Off SDN Node
		  */
		 TestUtils::displayTitle("Switching OFF SDN-0 Node");
		 m_pInterfaceSDN->writeCSValue("SDN-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::off);
		 ::sleep(1);
		 m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *m_pvValue);
		 ::sleep(1);
		 m_pInterfaceSDN->getPushedInt32("SDN-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::off, *m_pvValue);

		 /**
		  * Switching Off TestPVs Node
		  */
		 TestUtils::displayTitle("Switching OFF Test-0 Node");
		 m_pInterfacePVs->writeCSValue("Test-0-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::off);
		 ::sleep(1);
		 m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *m_pvValue);
		 ::sleep(1);
		 m_pInterfacePVs->getPushedInt32("Test-0-StateMachine.getState", m_pvTime, m_pvValue);
		 EXPECT_EQ((std::int32_t)nds::state_t::off, *m_pvValue);
		// Destroy test device
		 factory.destroyDevice("SDN-0");
		 factory.destroyDevice("Test-0");
	}
	catch(const std::runtime_error& re)
	{
		// specific handling for runtime_error
		std::cerr << "Runtime error: " << re.what() << std::endl;
//		FILE* file=NULL;
//		testerSDN.getPInterface()->report(file, 0);
		factory.destroyDevice("SDN-0");
		factory.destroyDevice("Test-0");
	}
	catch(...){
		std::cout<<"Caught Unknown exception: "<<std::endl;
//		FILE* file=NULL;
//		testerSDN.getPInterface()->report(file, 0);
		factory.destroyDevice("SDN-0");
		factory.destroyDevice("Test-0");
	}
}
