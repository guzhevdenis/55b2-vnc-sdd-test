/*
 * Generated with the FPGA Interface C API Generator 26.0
 * for NI-RIO 26.0 or later.
 */
#include "NiFpga_Fission_Chamber_DAQ_FPGA_Top.h"

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type* const destination)
{
   (*destination).Expected_ID = 0;
   (*destination).Expected_ID |= (packedData[0] & 0xFFULL) << 24;
   (*destination).Expected_ID |= (packedData[1] & 0xFF) << 16;
   (*destination).Expected_ID |= (packedData[2] & 0xFF) << 8;
   (*destination).Expected_ID |= (packedData[3] & 0xFF);
   (*destination).Inserted_ID = 0;
   (*destination).Inserted_ID |= (packedData[4] & 0xFFULL) << 24;
   (*destination).Inserted_ID |= (packedData[5] & 0xFF) << 16;
   (*destination).Inserted_ID |= (packedData[6] & 0xFF) << 8;
   (*destination).Inserted_ID |= (packedData[7] & 0xFF);
   (*destination).EEPROMPowerEnabled = 0;
   (*destination).EEPROMPowerEnabled |= ((packedData[8] >> 7) & 0x1);
   (*destination).IOEnabled = 0;
   (*destination).IOEnabled |= ((packedData[8] >> 6) & 0x1);
   (*destination).PowerEnabled = 0;
   (*destination).PowerEnabled |= ((packedData[8] >> 5) & 0x1);
   (*destination).PowerGood = 0;
   (*destination).PowerGood |= ((packedData[8] >> 4) & 0x1);
   (*destination).Present = 0;
   (*destination).Present |= ((packedData[8] >> 3) & 0x1);
   (*destination).InitializationDone = 0;
   (*destination).InitializationDone |= ((packedData[8] >> 2) & 0x1);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type* const source)
{
   packedData[0] = (uint8_t)(((*source).Expected_ID >> 24) & 0xFF);
   packedData[1] = (uint8_t)(((*source).Expected_ID >> 16) & 0xFF);
   packedData[2] = (uint8_t)(((*source).Expected_ID >> 8) & 0xFF);
   packedData[3] = (uint8_t)((*source).Expected_ID & 0xFF);
   packedData[4] = (uint8_t)(((*source).Inserted_ID >> 24) & 0xFF);
   packedData[5] = (uint8_t)(((*source).Inserted_ID >> 16) & 0xFF);
   packedData[6] = (uint8_t)(((*source).Inserted_ID >> 8) & 0xFF);
   packedData[7] = (uint8_t)((*source).Inserted_ID & 0xFF);
   packedData[8] = (uint8_t)(((*source).EEPROMPowerEnabled & 0x1) << 7);
   packedData[8] |= (uint8_t)(((*source).IOEnabled & 0x1) << 6);
   packedData[8] |= (uint8_t)(((*source).PowerEnabled & 0x1) << 5);
   packedData[8] |= (uint8_t)(((*source).PowerGood & 0x1) << 4);
   packedData[8] |= (uint8_t)(((*source).Present & 0x1) << 3);
   packedData[8] |= (uint8_t)(((*source).InitializationDone & 0x1) << 2);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type* const destination)
{
   (*destination).Power3_3V = 0;
   (*destination).Power3_3V |= (packedData[0] & 0xFF) << 8;
   (*destination).Power3_3V |= (packedData[1] & 0xFF);
   (*destination).Power12V = 0;
   (*destination).Power12V |= (packedData[2] & 0xFF) << 8;
   (*destination).Power12V |= (packedData[3] & 0xFF);
   (*destination).Temperature_degC = 0;
   (*destination).Temperature_degC |= (packedData[4] & 0xFF);
   (*destination).Clk100PLL_Unlocked = 0;
   (*destination).Clk100PLL_Unlocked |= ((packedData[5] >> 7) & 0x1);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type* const source)
{
   packedData[0] = (uint8_t)(((*source).Power3_3V >> 8) & 0xFF);
   packedData[1] = (uint8_t)((*source).Power3_3V & 0xFF);
   packedData[2] = (uint8_t)(((*source).Power12V >> 8) & 0xFF);
   packedData[3] = (uint8_t)((*source).Power12V & 0xFF);
   packedData[4] = (uint8_t)((*source).Temperature_degC & 0xFF);
   packedData[5] = (uint8_t)(((*source).Clk100PLL_Unlocked & 0x1) << 7);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type* const destination)
{
   (*destination).FirstExpCoeff = 0;
   (*destination).FirstExpCoeff |= (packedData[0] & 0xFF) << 8;
   (*destination).FirstExpCoeff |= (packedData[1] & 0xFF);
   (*destination).SecondExpCoeff = 0;
   (*destination).SecondExpCoeff |= (packedData[2] & 0xFF) << 8;
   (*destination).SecondExpCoeff |= (packedData[3] & 0xFF);
   (*destination).Type = 0;
   (*destination).Type |= (packedData[4] & 0xFF) << 8;
   (*destination).Type |= (packedData[5] & 0xFF);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type* const source)
{
   packedData[0] = (uint8_t)(((*source).FirstExpCoeff >> 8) & 0xFF);
   packedData[1] = (uint8_t)((*source).FirstExpCoeff & 0xFF);
   packedData[2] = (uint8_t)(((*source).SecondExpCoeff >> 8) & 0xFF);
   packedData[3] = (uint8_t)((*source).SecondExpCoeff & 0xFF);
   packedData[4] = (uint8_t)(((*source).Type >> 8) & 0xFF);
   packedData[5] = (uint8_t)((*source).Type & 0xFF);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type* const destination)
{
   (*destination).MSVnoise = 0;
   (*destination).MSVnoise |= (packedData[0] & 0xFFULL) << 56;
   (*destination).MSVnoise |= (packedData[1] & 0xFFULL) << 48;
   (*destination).MSVnoise |= (packedData[2] & 0xFFULL) << 40;
   (*destination).MSVnoise |= (packedData[3] & 0xFFULL) << 32;
   (*destination).MSVnoise |= (packedData[4] & 0xFFULL) << 24;
   (*destination).MSVnoise |= (packedData[5] & 0xFF) << 16;
   (*destination).MSVnoise |= (packedData[6] & 0xFF) << 8;
   (*destination).MSVnoise |= (packedData[7] & 0xFF);
   (*destination).CalibrationCoefficient = 0;
   (*destination).CalibrationCoefficient |= (packedData[8] & 0xFFULL) << 56;
   (*destination).CalibrationCoefficient |= (packedData[9] & 0xFFULL) << 48;
   (*destination).CalibrationCoefficient |= (packedData[10] & 0xFFULL) << 40;
   (*destination).CalibrationCoefficient |= (packedData[11] & 0xFFULL) << 32;
   (*destination).CalibrationCoefficient |= (packedData[12] & 0xFFULL) << 24;
   (*destination).CalibrationCoefficient |= (packedData[13] & 0xFF) << 16;
   (*destination).CalibrationCoefficient |= (packedData[14] & 0xFF) << 8;
   (*destination).CalibrationCoefficient |= (packedData[15] & 0xFF);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type* const source)
{
   packedData[0] = (uint8_t)(((*source).MSVnoise >> 56) & 0xFF);
   packedData[1] = (uint8_t)(((*source).MSVnoise >> 48) & 0xFF);
   packedData[2] = (uint8_t)(((*source).MSVnoise >> 40) & 0xFF);
   packedData[3] = (uint8_t)(((*source).MSVnoise >> 32) & 0xFF);
   packedData[4] = (uint8_t)(((*source).MSVnoise >> 24) & 0xFF);
   packedData[5] = (uint8_t)(((*source).MSVnoise >> 16) & 0xFF);
   packedData[6] = (uint8_t)(((*source).MSVnoise >> 8) & 0xFF);
   packedData[7] = (uint8_t)((*source).MSVnoise & 0xFF);
   packedData[8] = (uint8_t)(((*source).CalibrationCoefficient >> 56) & 0xFF);
   packedData[9] = (uint8_t)(((*source).CalibrationCoefficient >> 48) & 0xFF);
   packedData[10] = (uint8_t)(((*source).CalibrationCoefficient >> 40) & 0xFF);
   packedData[11] = (uint8_t)(((*source).CalibrationCoefficient >> 32) & 0xFF);
   packedData[12] = (uint8_t)(((*source).CalibrationCoefficient >> 24) & 0xFF);
   packedData[13] = (uint8_t)(((*source).CalibrationCoefficient >> 16) & 0xFF);
   packedData[14] = (uint8_t)(((*source).CalibrationCoefficient >> 8) & 0xFF);
   packedData[15] = (uint8_t)((*source).CalibrationCoefficient & 0xFF);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type* const destination)
{
   (*destination).Threshold = 0;
   (*destination).Threshold |= (packedData[0] & 0xFF) << 8;
   (*destination).Threshold |= (packedData[1] & 0xFF);
   (*destination).SinglePulseMax = 0;
   (*destination).SinglePulseMax |= (packedData[2] & 0xFF) << 8;
   (*destination).SinglePulseMax |= (packedData[3] & 0xFF);
   (*destination).SinglePulseMin = 0;
   (*destination).SinglePulseMin |= (packedData[4] & 0xFF) << 8;
   (*destination).SinglePulseMin |= (packedData[5] & 0xFF);
   (*destination).TotalPulseMin = 0;
   (*destination).TotalPulseMin |= (packedData[6] & 0xFF) << 8;
   (*destination).TotalPulseMin |= (packedData[7] & 0xFF);
}

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type* const source)
{
   packedData[0] = (uint8_t)(((*source).Threshold >> 8) & 0xFF);
   packedData[1] = (uint8_t)((*source).Threshold & 0xFF);
   packedData[2] = (uint8_t)(((*source).SinglePulseMax >> 8) & 0xFF);
   packedData[3] = (uint8_t)((*source).SinglePulseMax & 0xFF);
   packedData[4] = (uint8_t)(((*source).SinglePulseMin >> 8) & 0xFF);
   packedData[5] = (uint8_t)((*source).SinglePulseMin & 0xFF);
   packedData[6] = (uint8_t)(((*source).TotalPulseMin >> 8) & 0xFF);
   packedData[7] = (uint8_t)((*source).TotalPulseMin & 0xFF);
}
