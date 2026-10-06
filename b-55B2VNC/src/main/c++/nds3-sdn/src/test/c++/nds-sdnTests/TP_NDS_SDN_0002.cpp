#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <nds3/ndsTestInterface.h>
#include <nds3/testUtils.h>

#include <ctime>

TEST(TP_NDS_SDN,0020)
{
	std::string testName="TP_NDS_SDN_0020";
	std::string testDescription("Checking that error is detected if PVS_CONFIG_PATH is not defined");

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
	deviceTesting.emplace("PV_CONF",pwd+"/../../../main/resources/test-nds-PVs.xml");
	actualpath= realpath(deviceTesting["PV_CONF"].c_str(), NULL);
	std::cout<<"XML PATH for the Testing Device: "<<std::string(actualpath)<<std::endl;
	free(actualpath);

	// Create test device of type DeviceTesting and named Test-0
	factory.createDevice("DeviceTesting", "Test-0", deviceTesting);

	//Create device DeviceNDS-SDNPUB
	nds::namedParameters_t ndssdnParams;
	ndssdnParams.emplace("INTERFACE", "lo");
	//forcing an error not defining the PVS_CONFIG_PATH
	//ndssdnParams.emplace("PVS_CONFIG_PATH", pwd+"/../../../main/resources/input.xml");
	//actualpath= realpath(ndssdnParams["PVS_CONFIG_PATH"].c_str(), NULL);
	//std::cout<<"XML PATH for the SDN Device: "<<std::string(actualpath)<<std::endl;
	//free(actualpath);

	// Create test device of type DeviceNDSSDNPUB and named SDN-0
	EXPECT_THROW(factory.createDevice("DeviceNDSSDNPUB", "SDN-0", ndssdnParams);, std::logic_error);
	factory.destroyDevice("Test-0");

}

TEST(TP_NDS_SDN,0021)
{
	std::string testName="TP_NDS_SDN_0021";
	std::string testDescription("Check error whether XML input file doesn't exist");

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
	deviceTesting.emplace("PV_CONF",pwd+"/../../../main/resources/test-nds-PVs.xml");
	actualpath= realpath(deviceTesting["PV_CONF"].c_str(), NULL);
	std::cout<<"XML PATH for the Testing Device: "<<std::string(actualpath)<<std::endl;
	free(actualpath);

	// Create test device of type DeviceTesting and named Test-0
	factory.createDevice("DeviceTesting", "Test-0", deviceTesting);

	//Create device DeviceNDS-SDNPUB
	nds::namedParameters_t ndssdnParams;
	ndssdnParams.emplace("INTERFACE", "lo");
	//Setting PVS_CONFIG_PATH pointing to an unexisting file
	ndssdnParams.emplace("PVS_CONFIG_PATH", pwd+"/../../../main/resources/inputerror.xml");
	// this file doesn't exist therefore actualpath will be null
	actualpath= realpath(ndssdnParams["PVS_CONFIG_PATH"].c_str(), NULL);

	if (actualpath!=NULL){
		std::cout<<"XML PATH for the SDN Device: "<<std::string(actualpath)<<std::endl;
		free(actualpath);
	}
	EXPECT_THROW(factory.createDevice("DeviceNDSSDNPUB", "SDN-0", ndssdnParams);, std::logic_error);
	factory.destroyDevice("Test-0");

}

TEST(TP_NDS_SDN,0022)
{
	std::string testName="TP_NDS_SDN_0022";
	std::string testDescription("Check error when the XML content is invalid");

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
	deviceTesting.emplace("PV_CONF",pwd+"/../../../main/resources/test-nds-PVs.xml");
	actualpath= realpath(deviceTesting["PV_CONF"].c_str(), NULL);
	std::cout<<"XML PATH for the Testing Device: "<<std::string(actualpath)<<std::endl;
	free(actualpath);

	// Create test device of type DeviceTesting and named Test-0
	factory.createDevice("DeviceTesting", "Test-0", deviceTesting);

	//Create device DeviceNDS-SDNPUB
	nds::namedParameters_t ndssdnParams;
	ndssdnParams.emplace("INTERFACE", "lo");
	//Setting PVS_CONFIG_PATH pointing to an unexisting file
	ndssdnParams.emplace("PVS_CONFIG_PATH", pwd+"/../../../main/resources/inputwitherror.xml");
	// this file doesn't exist therefore actualpath will be null
	actualpath= realpath(ndssdnParams["PVS_CONFIG_PATH"].c_str(), NULL);

	if (actualpath!=NULL){
		std::cout<<"XML PATH for the SDN Device: "<<std::string(actualpath)<<std::endl;
		free(actualpath);
	}

	// Create test device of type DeviceNDSSDNPUB and named SDN-0
	EXPECT_THROW(factory.createDevice("DeviceNDSSDNPUB", "SDN-0", ndssdnParams);, std::logic_error);
	factory.destroyDevice("Test-0");

}

