#include "./../include/DanDataGenerator.h"

#include <algorithm>
#include <iostream>
#include <time.h>
#include <unistd.h>

timespec DanDataGenerator::wallClockTimestamp()
{
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts;
}

/*Data Acquisiton functions*/

void DanDataGenerator::switchOn_DataAcquisition() {
}

void DanDataGenerator::switchOff_DataAcquisition() {
}

void DanDataGenerator::start_DataAcquisition() {
	const timespec ts = wallClockTimestamp();
	m_DataAcquisition.setNumberOfPushedDataBlocks(ts, 0);

	m_bStop_DataAcquisition = false;
	m_acquisitionThreadRunning = true;
	m_DataAcquisition_Thread =
		m_DataAcquisition.runInThread("DAQ",
			std::bind(&DanDataGenerator::DataAcquisition_thread_body, this));
}

void DanDataGenerator::stop_DataAcquisition() {
	m_bStop_DataAcquisition = true;
	if (m_acquisitionThreadRunning) {
		m_DataAcquisition_Thread.join();
		m_acquisitionThreadRunning = false;
	}
}

void DanDataGenerator::recover_DataAcquisition() {
	throw nds::StateMachineRollBack("Cannot recover");
}

bool DanDataGenerator::allow_DataAcquisition_Change(const nds::state_t,
	const nds::state_t, const nds::state_t) {
	return true;
}

void DanDataGenerator::DataAcquisition_thread_body() {
	const size_t nElm = m_DataAcquisition.getMaxElements();
	std::vector<std::int16_t> outputData(nElm, 0);
	double counter(0);

	std::int32_t NumberOfPushedDataBlocks(0);

	const double Gain = m_DataAcquisition.getGain();
	const double Bandwidth = m_DataAcquisition.getBandwidth();
	const double Resolution = m_DataAcquisition.getResolution();
	const double Coupling = m_DataAcquisition.getCoupling();
	const double SignalRefTYpe = m_DataAcquisition.getSignalRefType();
	const double Ground = m_DataAcquisition.getGround();
	const double Offset = m_DataAcquisition.getOffset();
	const std::int32_t Impedance = m_DataAcquisition.getImpedance();
	const double SamplingRate = m_DataAcquisition.getSamplingRate();

	std::cout << "\tGain = " << Gain << std::endl;
	std::cout << "\tBandwidth = " << Bandwidth << std::endl;
	std::cout << "\tResolution = " << Resolution << std::endl;
	std::cout << "\tCoupling = " << Coupling << std::endl;
	std::cout << "\tSignalRefType = " << SignalRefTYpe << std::endl;
	std::cout << "\tGround = " << Ground << std::endl;
	std::cout << "\tOffset = " << Offset << std::endl;
	std::cout << "\tImpedance = " << Impedance << std::endl;
	std::cout << "\tSamplingRate = " << SamplingRate << std::endl;

	while (!m_bStop_DataAcquisition) {
		const std::int16_t sample = static_cast<std::int16_t>(counter);
		std::fill(outputData.begin(), outputData.end(), sample);
		++counter;

		const timespec blockTimestamp = wallClockTimestamp();
		m_DataAcquisition.push(blockTimestamp, outputData);
		++NumberOfPushedDataBlocks;

		// 100 ms between blocks (simulation); avoids flooding the IOC with I/O Intr scans.
		::usleep(100000);
	}

	const timespec stopTimestamp = wallClockTimestamp();
	m_DataAcquisition.setNumberOfPushedDataBlocks(stopTimestamp,
		NumberOfPushedDataBlocks);
}


void DanDataGenerator::PV_DataAcquisition_Gain_Writer(const timespec& /*timestamp*/,
	const double& value) {
	m_DataAcquisition.setGain(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_Offset_Writer(const timespec& /*timestamp*/,
	const double& value) {
	m_DataAcquisition.setOffset(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_Bandwidth_Writer(const timespec& /*timestamp*/,
	const double& value) {
	m_DataAcquisition.setBandwidth(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_Resolution_Writer(const timespec& /*timestamp*/,
	const double& value) {
	m_DataAcquisition.setResolution(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_Impedance_Writer(const timespec& /*timestamp*/,
	const double& value) {
	m_DataAcquisition.setImpedance(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_Coupling_Writer(const timespec& /*timestamp*/,
	const std::int32_t& value) {
	m_DataAcquisition.setCoupling(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_SignalRefType_Writer(const timespec& /*timestamp*/,
	const std::int32_t& value) {
	m_DataAcquisition.setSignalRefType(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_Ground_Writer(const timespec& /*timestamp*/,
	const std::int32_t& value) {
	m_DataAcquisition.setGround(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_DMAEnable_Writer(const timespec& /*timestamp*/,
	const std::int32_t& value) {
	m_DataAcquisition.setDMAEnable(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_SamplingRate_Writer(const timespec& /*timestamp*/,
	const double& value) {
	m_DataAcquisition.setSamplingRate(wallClockTimestamp(), value);
}

void DanDataGenerator::PV_DataAcquisition_Gain_Initializer(timespec* timestamp,
	double* value) {
	*timestamp = wallClockTimestamp();
	*value = 2.5;
}

void DanDataGenerator::PV_DataAcquisition_Offset_Initializer(timespec* timestamp,
	double* value) {
	*timestamp = wallClockTimestamp();
	*value = 3.0;
}

void DanDataGenerator::PV_DataAcquisition_Bandwidth_Initializer(timespec* timestamp,
	double* value) {
	*timestamp = wallClockTimestamp();
	*value = 10.0;
}

void DanDataGenerator::PV_DataAcquisition_Resolution_Initializer(timespec* timestamp,
	double* value) {
	*timestamp = wallClockTimestamp();
	*value = 15.0;
}

void DanDataGenerator::PV_DataAcquisition_Impedance_Initializer(timespec* timestamp,
	double* value) {
	*timestamp = wallClockTimestamp();
	*value = 2.2;
}

void DanDataGenerator::PV_DataAcquisition_Coupling_Initializer(timespec* timestamp,
	int32_t* value) {
	*timestamp = wallClockTimestamp();
	*value = 1;
}

void DanDataGenerator::PV_DataAcquisition_SignalRefType_Initializer(
	timespec* timestamp, int32_t* value) {
	*timestamp = wallClockTimestamp();
	*value = 0;
}

void DanDataGenerator::PV_DataAcquisition_Ground_Initializer(timespec* timestamp,
	int32_t* value) {
	*timestamp = wallClockTimestamp();
	*value = 0;
}

void DanDataGenerator::PV_DataAcquisition_DMAEnable_Initializer(timespec* timestamp,
	int32_t* value) {
	*timestamp = wallClockTimestamp();
	*value = 0;
}

void DanDataGenerator::PV_DataAcquisition_SamplingRate_Initializer(timespec* timestamp,
	double* value) {
	*timestamp = wallClockTimestamp();
	*value = 10.0;
}
