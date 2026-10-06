
#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <nds3/nds.h>
#include <fstream>
#include <stdio.h>
#include <gtest/gtest.h>

#define RST    "\x1B[0m"	//Foreground color, Reset
#define FBLA   "\x1b[30m"	//Foreground color, Black
#define FRED   "\x1b[31m"	//Foreground color, Red
#define FGRN   "\x1b[32m"	//Foreground color, Green
#define FYEL   "\x1b[33m"	//Foreground color, Yellow
#define FBLU   "\x1b[34m"	//Foreground color, Blue
#define FMAG   "\x1b[35m"	//Foreground color, Magenta
#define FCYN   "\x1b[36m"	//Foreground color, Cyan
#define FWHT   "\x1b[37m"	//Foreground color, White
#define FDEF   "\x1b[39m"	//Foreground color, Default(foreground color at startup)
#define FLGRA  "\x1b[90m"	//Foreground color, Light Gray
#define FLRED  "\x1b[91m"	//Foreground color, Light Red
#define FLGRN  "\x1b[92m"	//Foreground color, Light Green
#define FLYEL  "\x1b[93m"	//Foreground color, Light Yellow
#define FLBLU  "\x1b[94m"	//Foreground color, Light Blue
#define FLMAG  "\x1b[95m"	//Foreground color, Light Magenta
#define FLCYN  "\x1b[96m"	//Foreground color, Light Cyan
#define FLWHT  "\x1b[97m"	//Foreground color, Light White

#define BBLA   "\x1b[40m"	//Background color, Black
#define BRED   "\x1b[41m"	//Background color, Red
#define BGRN   "\x1b[42m"	//Background color, Green
#define BYEL   "\x1b[43m"	//Background color, Yellow
#define BBLU   "\x1b[44m"	//Background color, Blue
#define BMAG   "\x1b[45m"	//Background color, Magenta
#define BCYN   "\x1b[46m"	//Background color, Cyan
#define BWHT   "\x1b[47m"	//Background color, White
#define BDEF   "\x1b[49m"	//Background color, Default(background color at startup)
#define BLGRA  "\x1b[100m"	//Background color, Light Gray
#define BLRED  "\x1b[101m"	//Background color, Light Red
#define BLGRN  "\x1b[102m"	//Background color, Light Green
#define BLYEL  "\x1b[103m"	//Background color, Light Yellow
#define BLBLU  "\x1b[104m"	//Background color, Light Blue
#define BLMAG  "\x1b[105m"	//Background color, Light Magenta
#define BLCYN  "\x1b[106m"	//Background color, Light Cyan
#define BLWHT  "\x1b[107m"	//Background color, Light White

class TestUtils
{
public:
	TestUtils(std::string device_name){
		m_pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance(device_name);
	}

private:
	nds::tests::TestControlSystemInterfaceImpl* m_pInterface = NULL;
	const timespec* m_pvTime = NULL;
	const std::int32_t* m_pvValue = NULL;
	timespec m_timestamp = {0, 0};

public:
	void switchOn(std::string nodeStr, int trWaitS=1, int trWaitUs=10000, int trExtendsWaitS=0) {

		try {
			// Check initial state (OFF)
			m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
			EXPECT_EQ((std::int32_t)nds::state_t::off, *m_pvValue);
		}
		catch(const std::runtime_error& re) {
			std::cout<<"Checking off state get: "<< re.what() << std::endl;
		}

		std::cout<<"Switching to ON "+nodeStr +"..."<<std::endl;

		//Change state:  OFF -> (initializing) -> ON
		m_pInterface->writeCSValue(nodeStr+"-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::on);
		::sleep(trExtendsWaitS);
		::usleep(trWaitUs);
		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::initializing, *m_pvValue);
		::sleep(trWaitS);
		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *m_pvValue);

		this->displayTitle(nodeStr +" node ON!", FCYN);
		::sleep(1);
	}

	void switchRun(std::string nodeStr, int trWaitS=1, int trWaitUs=10000, int trExtendsWaitS=0) {
		std::cout<<"Switching to RUNNING "+nodeStr+"..."<<std::endl;

		//Change state:  ON -> (starting) -> RUNNING
		m_pInterface->writeCSValue(nodeStr+"-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::running);
		::sleep(trExtendsWaitS);
		::usleep(trWaitUs);
		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::starting, *m_pvValue);
		::sleep(trWaitS);
		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::running, *m_pvValue);

		TestUtils::displayTitle(nodeStr +" node RUNNING!", FCYN);
		::sleep(1);
	}

	void switchStop(std::string nodeStr, int trWaitS=1, int trWaitUs=10000) {
		std::cout<<"Switching to ON running node "+nodeStr+"..."<<std::endl;
		//Change state:  RUNNING -> (stopping) -> ON
		m_pInterface->writeCSValue(nodeStr+"-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::on);
		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::stopping, *m_pvValue);
		::sleep(trWaitS);

		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::on, *m_pvValue);
		::usleep(trWaitUs);

		TestUtils::displayTitle(nodeStr+" node ON!", FCYN);
		::sleep(1);
	}

	void switchOff(std::string nodeStr, int trWaitS=1, int trWaitUs=10000) {
		std::cout<<"Switching to OFF "+nodeStr+"..."<<std::endl;
		//Change state:  ON -> (switchingOff) -> OFF
		m_pInterface->writeCSValue(nodeStr+"-StateMachine.setState", m_timestamp, (std::int32_t)nds::state_t::off);
		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *m_pvValue);
		::sleep(trWaitS);

		m_pInterface->getPushedInt32(nodeStr+"-StateMachine.getState", m_pvTime, m_pvValue);
		EXPECT_EQ((std::int32_t)nds::state_t::off, *m_pvValue);
		::usleep(trWaitUs);

		TestUtils::displayTitle(nodeStr+" node OFF!", FCYN);
		::sleep(1);
	}

	nds::tests::TestControlSystemInterfaceImpl* getPInterface() {
		return m_pInterface;
	}

	std::vector<double> getPushedVectorDouble(std::string nodePV);

	/**
	 * @brief Get a string by identifying the type of data (std::int32_t) and its value
	 */
	static std::string getString(const std::int32_t & data);

	/**
	 * @brief Get a string by identifying the type of data (double) and its value
	 */
	static std::string getString(const double & data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<bool> data) and its value
	 */
	static std::string getString(std::vector<bool> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint8_t>) and its value
	 */
	static std::string getString(std::vector<std::uint8_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint16_t>) and its value
	 */
	static std::string getString(std::vector<std::uint16_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::uint32_t>) and its value
	 */
	static std::string getString(std::vector<std::uint32_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int8_t>) and its value
	 */
	static std::string getString(std::vector<std::int8_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int16_t>) and its value
	 */
	static std::string getString(std::vector<std::int16_t> data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<std::int32_t>) and its value
	 */
	static std::string getString(std::vector<std::int32_t> data);

	/**
	 * @brief Get a string by identifying the type of data (double) and its value
	 */
	static std::string getString(std::vector<double> data);

	/**
	 * @brief Get a string by identifying the type of data (std::string) and its value
	 */
	static std::string getString(std:: string data);

	/**
	 * @brief Get a string by identifying the type of data (timespec data) and its value
	 */
	static std::string getString(timespec data);

	/**
	 * @brief Get a string by identifying the type of data (std::vector<timespec> data) and its value
	 */
	static std::string getString(std::vector<timespec> data);

	/**
	 * @brief Get a string by identifying the type of data (nds::timestamp_t) and its value
	 */
	static std::string getString(nds::timestamp_t data);

	static void displayTitle(std::string msg, std::string forecolor=FWHT, std::string backcolor=BBLA) {
		std::cout<<forecolor<<backcolor<<"+-----------------------------------------------------------------------------"<<std::endl;
		std::cout<<msg<<std::endl;
		std::cout<<"+-----------------------------------------------------------------------------"<<std::endl<<RST<<std::endl;
	}

	static void waitEnterKey(std::string msg, std::string forecolor=FWHT, std::string backcolor=BBLA) {
		std::cout<<forecolor<<backcolor<<"+-----------------------------------------------------------------------------"<<std::endl;
		std::cout<<msg<<std::endl;
		std::cout<<"+-----------------------------------------------------------------------------"<<std::endl<<RST;

		while(std::cin.get()!='\n');
	}
};




#endif /* TESTUTILS_H_ */
