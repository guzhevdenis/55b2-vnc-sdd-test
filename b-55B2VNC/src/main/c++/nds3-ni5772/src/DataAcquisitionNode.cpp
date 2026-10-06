#include "./../include/Ni5772Digitizer.h"

#define SIZE 800000

#define NDS_EPOCH 1514764800

/*Data Acquisiton functions*/

double count_average(std::vector <uint64_t> & data_buffer, size_t size);

void Ni5772Digitizer::switchOn_DataAcquisition() {}

void Ni5772Digitizer::switchOff_DataAcquisition() {}

void Ni5772Digitizer::start_DataAcquisition() {

  m_bStop_DataAcquisition = false; //starts the cycle 
  m_DataAcquisition_Thread =
    m_DataAcquisition.runInThread("ProcessedDAQ",
      std::bind( & Ni5772Digitizer::DataAcquisition_thread_body, this)); // runs the thread in thread body method
  m_bStop_RawDataAcquisition = false; //starts the cycle 
  m_RawDataAcquisition_Thread =
    m_DataAcquisition.runInThread("RawDAQ",
      std::bind( & Ni5772Digitizer::RawDataAcquisition_thread_body, this));

}

void Ni5772Digitizer::stop_DataAcquisition() {
  m_bStop_DataAcquisition = true;
  m_DataAcquisition_Thread.join();
}

void Ni5772Digitizer::recover_DataAcquisition() {
  throw nds::StateMachineRollBack("Cannot recover");
}

bool Ni5772Digitizer::allow_DataAcquisition_Change(const nds::state_t,
  const nds::state_t,
    const nds::state_t) {
  return true;
}

//thread for Raw Data from LabVIEW FPGA DMA FIFO
void Ni5772Digitizer::RawDataAcquisition_thread_body() {

  std::int16_t* pRawData;
  std::vector<int16_t> RawData;
  size_t NumberOfPushedDataBlocks = 0;

  size_t elements_requested = 1023;

  //RawData FIFO (I16)
  size_t rawDataAvailable = 0;
  size_t rawDataAcquired = 0;

  while(!m_bStop_RawDataAcquisition){

    //Acquire all Target-to-Host DMA FIFOs
    //RawData
		status = NiFpga_AcquireFifoReadElementsI16(session,
				NiFpga_exp_test_FIC_TargetToHostFifoI16_RawData, &pRawData, 0, 0, 
				&rawDataAcquired, &rawDataAvailable);

		if (rawDataAvailable > 0){
			status = NiFpga_AcquireFifoReadElementsI16(session,
					NiFpga_exp_test_FIC_TargetToHostFifoI16_RawData, &pRawData, rawDataAvailable, 0, 
					&rawDataAcquired, &rawDataAvailable); 

				RawData.insert(RawData.end(), pRawData, pRawData+rawDataAcquired);

        status = NiFpga_ReleaseFifoElements(session,
				NiFpga_exp_test_FIC_TargetToHostFifoI16_RawData, rawDataAcquired);
				RawData.clear();
      m_DataAcquisition.push(m_DataAcquisition.getTimestamp(), RawData);
      ++NumberOfPushedDataBlocks;
		}

  }

  m_DataAcquisition.setNumberOfPushedDataBlocks(m_DataAcquisition.getTimestamp(),NumberOfPushedDataBlocks);



}

//thread for Processed FIFO Data from LabVIEW FPGA DMA FIFO
void Ni5772Digitizer::DataAcquisition_thread_body() {

	std::int16_t* pPulsePeaksFIFO;
  std::uint64_t* pMSV;
  std::uint64_t* pCountRateFluctuation;

  std::vector<int16_t> PulsePeaksFIFO;
	std::vector<uint64_t> MSVADC;
	std::vector<uint64_t> CountRateFluctuation;

  size_t NumberOfPushedDataBlocks = 0;

  size_t elements_requested = 1023;

    // PulsePeaks FIFO (I16)
  size_t pulsePeaksAvailable = 0;
  size_t pulsePeaksAcquired = 0;

  // MSV ADC FIFO (U64)
  size_t msvAvailable = 0;
  size_t msvAcquired = 0;

  // CountRate fluctuation FIFO (U64)
  size_t countRateAvailable = 0;
  size_t countRateAcquired = 0;

	while(!m_bStop_DataAcquisition){

    //Acquire all Target-to-Host DMA FIFOs
    //PulsePeaksFIFOs
		status = NiFpga_AcquireFifoReadElementsI16(session,
				NiFpga_exp_test_FIC_TargetToHostFifoI16_PulsePeaksFIFO, &pPulsePeaksFIFO, 0, 0, 
				&pulsePeaksAcquired, &pulsePeaksAvailable);

		if (pulsePeaksAvailable > 0){
			status = NiFpga_AcquireFifoReadElementsI16(session,
					NiFpga_exp_test_FIC_TargetToHostFifoI16_PulsePeaksFIFO, &pPulsePeaksFIFO, pulsePeaksAvailable, 0, 
					&pulsePeaksAcquired, &pulsePeaksAvailable); 

				PulsePeaksFIFO.insert(PulsePeaksFIFO.end(), pPulsePeaksFIFO, pPulsePeaksFIFO+pulsePeaksAcquired);

        status = NiFpga_ReleaseFifoElements(session,
				NiFpga_exp_test_FIC_TargetToHostFifoI16_PulsePeaksFIFO, pulsePeaksAcquired);
				PulsePeaksFIFO.clear();
				MaximumAmplitude_m.push(m_DataAcquisition.getTimestamp(), double((*std::max_element(PulsePeaksFIFO.begin(), PulsePeaksFIFO.end()))));

		}

		status = NiFpga_AcquireFifoReadElementsU64(session, 
		NiFpga_exp_test_FIC_TargetToHostFifoU64_MSVADC, &pMSV, 0, 0,
		&msvAcquired, &msvAvailable);

		if (msvAvailable > 0){

		status = NiFpga_AcquireFifoReadElementsU64(session, 
				NiFpga_exp_test_FIC_TargetToHostFifoU64_MSVADC, &pMSV, msvAvailable, 0,
				&msvAcquired, &msvAvailable);

				MSVADC.insert(MSVADC.end(), pMSV, pMSV + msvAcquired);

				status = NiFpga_ReleaseFifoElements(session,
				NiFpga_exp_test_FIC_TargetToHostFifoU64_MSVADC, msvAcquired);	
				MSVADC.clear();

		}

		status = NiFpga_AcquireFifoReadElementsU64(session, 
				NiFpga_exp_test_FIC_TargetToHostFifoU64_Countratefluctuation, &pCountRateFluctuation, 0, 0,
				&countRateAcquired, &countRateAvailable);

		if (countRateAvailable > 0) {

		status = NiFpga_AcquireFifoReadElementsU64(session, 
				NiFpga_exp_test_FIC_TargetToHostFifoU64_Countratefluctuation, &pCountRateFluctuation, countRateAvailable, 0,
				&countRateAcquired, &countRateAvailable);

				CountRateFluctuation.insert(CountRateFluctuation.end(), pCountRateFluctuation, pCountRateFluctuation + countRateAcquired);

				status = NiFpga_ReleaseFifoElements(session,
				NiFpga_exp_test_FIC_TargetToHostFifoU64_Countratefluctuation, countRateAcquired);
        
        CountRateFluctuation.clear();
		}

		CountRate_Average = count_average(CountRateFluctuation, CountRateFluctuation.size());
		MSV = count_average(CountRateFluctuation, CountRateFluctuation.size());

    NumberOfPushedDataBlocks++;

		m_DataAcquisition.setNumberOfPushedDataBlocks(m_DataAcquisition.getTimestamp(),NumberOfPushedDataBlocks);
	}
} 


void Ni5772Digitizer::PV_DataAcquisition_Gain_Writer(const timespec & timestamp,
  const double & value) {
  double HW_value;
  HW_value = value;
  m_DataAcquisition.setGain(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_Offset_Writer(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Offset to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Offset programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setOffset(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_Bandwidth_Writer(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Bandwidth to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Bandwidth programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setBandwidth(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_Resolution_Writer(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Resolution to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Resolution programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setResolution(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_Impedance_Writer(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Impedance to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Impedance programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setImpedance(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_Coupling_Writer(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Coupling to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Coupling programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setCoupling(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_SignalRefType_Writer(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the SignalRefType to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SignalRefType programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setSignalRefType(timestamp, HW_value);
}
void Ni5772Digitizer::PV_DataAcquisition_Ground_Writer(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Ground to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Ground programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setGround(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_DMAEnable_Writer(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the DMAEnable value to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real DMAEnable value programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setDMAEnable(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_SamplingRate_Writer(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the SamplingRate to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SamplingRate programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition.setSamplingRate(timestamp, HW_value);
}

void Ni5772Digitizer::PV_DataAcquisition_Gain_Initializer(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    10
  };
  * value = 2.5; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_Offset_Initializer(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    20
  };
  * value = 3.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_Bandwidth_Initializer(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    30
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.

}

void Ni5772Digitizer::PV_DataAcquisition_Resolution_Initializer(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    40
  };
  * value = 15.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_Impedance_Initializer(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    50
  };
  * value = 2.2; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_Coupling_Initializer(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    60
  };
  * value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_SignalRefType_Initializer(
  timespec * timestamp, int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    70
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_Ground_Initializer(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    80
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_DMAEnable_Initializer(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    90
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5772Digitizer::PV_DataAcquisition_SamplingRate_Initializer(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    100
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.
}