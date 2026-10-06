//State machine functions

#include "./../include/OscilProcessing.h"

void OscilProcessing::switchOn_Device() {
  NiFpga_MergeStatus(&status, NiFpga_Open("../../../c++/nds-oscilloscope/bitfile/NiFpga_NI5772_Oscilloscope_FPGA_Top.lvbitx", NiFpga_NI5772_Oscilloscope_FPGA_Top_Signature, "RIO0", 0, &session));
  
  NiFpga_Run(session, 0);

  FIFO_Ch0_status = NiFpga_ConfigureFifo2(
      session, NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch0,
      2047 * depthMultiplier, &ToHostFIFO_Ch0_depth);
  
  FIFO_Ch1_status = NiFpga_ConfigureFifo2(
      session, NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch1,
      2047 * depthMultiplier, &ToHostFIFO_Ch1_depth);
}

void OscilProcessing::switchOff_Device() {
  FIFO_Ch0_status = NiFpga_ReleaseFifoElements(
    session, NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch0,
    FIFO_Ch0_DataAcquired);
  FIFO_Ch1_status = NiFpga_ReleaseFifoElements(
    session, NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch1,
    FIFO_Ch1_DataAcquired);
  
  NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
}

void OscilProcessing::start_Device() {}

void OscilProcessing::stop_Device() {}

void OscilProcessing::recover_Device() {}

bool OscilProcessing::allow_Device_Change(const nds::state_t,
  const nds::state_t,
    const nds::state_t) {
  return true;
}