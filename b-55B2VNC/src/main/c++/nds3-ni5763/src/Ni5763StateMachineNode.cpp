//State machine functions

#include "./../include/Ni5763Main.h"

void Ni5763Main::switchOn_Device() {
    NiFpga_MergeStatus(&status, NiFpga_Open("../../../c++/nds3-ni5763/bitfile/NiFpga_Fission_Chamber_DAQ_FPGA_Top.lvbitx", NiFpga_Fission_Chamber_DAQ_FPGA_Top_Signature, "RIO0", 0, &session));
    
    NiFpga_Run(session, 0);

    status = NiFpga_ConfigureFifo2(
        session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch0,
        ConfigFIFO_Depth, &RawData_depth);

    status = NiFpga_ConfigureFifo2(
        session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_FluctCountRate_Hz_ToHost_Ch0,
        ConfigFIFO_Depth, &FluctCountRate_Hz_depth);

    status = NiFpga_ConfigureFifo2(
        session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_PulseChain_ToHost_Ch0,
        ConfigFIFO_Depth, &PulseChain_depth);
}

void Ni5763Main::switchOff_Device() {

    status = NiFpga_ReleaseFifoElements(
        session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch0,
        RawData_DataAcquired);

    status = NiFpga_ReleaseFifoElements(
        session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_FluctCountRate_Hz_ToHost_Ch0,
        FluctCountRate_Hz_DataAcquired);

    status = NiFpga_ReleaseFifoElements(
        session, NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_PulseChain_ToHost_Ch0,
        PulseChain_DataAcquired);

    NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
}

void Ni5763Main::start_Device() {}

void Ni5763Main::stop_Device() {}

void Ni5763Main::recover_Device() {}

bool Ni5763Main::allow_Device_Change(const nds::state_t,
  const nds::state_t,
    const nds::state_t) {
  return true;
}