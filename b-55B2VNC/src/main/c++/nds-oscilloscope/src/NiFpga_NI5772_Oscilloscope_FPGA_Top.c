/*
 * Generated with the FPGA Interface C API Generator 26.0
 * for NI-RIO 26.0 or later.
 */
#include "NiFpga_NI5772_Oscilloscope_FPGA_Top.h"

void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type* const destination)
{
   (*destination).Ch_On = 0;
   (*destination).Ch_On |= ((packedData[0] >> 7) & 0x1);
   (*destination).FSA_On = 0;
   (*destination).FSA_On |= ((packedData[0] >> 6) & 0x1);
   (*destination).delta = 0;
   (*destination).delta |= (packedData[0] & 0x3F) << 10;
   (*destination).delta |= (packedData[1] & 0xFF) << 2;
   (*destination).delta |= ((packedData[2] >> 6) & 0x3);
   (*destination).reference = 0;
   (*destination).reference |= (packedData[2] & 0x3F) << 10;
   (*destination).reference |= (packedData[3] & 0xFF) << 2;
   (*destination).reference |= ((packedData[4] >> 6) & 0x3);
   (*destination).difference = 0;
   (*destination).difference |= (packedData[4] & 0x3F) << 10;
   (*destination).difference |= (packedData[5] & 0xFF) << 2;
   (*destination).difference |= ((packedData[6] >> 6) & 0x3);
}

void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type* const source)
{
   packedData[0] = (uint8_t)(((*source).Ch_On & 0x1) << 7);
   packedData[0] |= (uint8_t)(((*source).FSA_On & 0x1) << 6);
   packedData[0] |= (uint8_t)(((*source).delta >> 10) & 0x3F);
   packedData[1] = (uint8_t)(((*source).delta >> 2) & 0xFF);
   packedData[2] = (uint8_t)(((*source).delta & 0x3) << 6);
   packedData[2] |= (uint8_t)(((*source).reference >> 10) & 0x3F);
   packedData[3] = (uint8_t)(((*source).reference >> 2) & 0xFF);
   packedData[4] = (uint8_t)(((*source).reference & 0x3) << 6);
   packedData[4] |= (uint8_t)(((*source).difference >> 10) & 0x3F);
   packedData[5] = (uint8_t)(((*source).difference >> 2) & 0xFF);
   packedData[6] = (uint8_t)(((*source).difference & 0x3) << 6);
}

void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type* const destination)
{
   (*destination).Ch_On = 0;
   (*destination).Ch_On |= ((packedData[0] >> 7) & 0x1);
   (*destination).FSA_On = 0;
   (*destination).FSA_On |= ((packedData[0] >> 6) & 0x1);
   (*destination).delta = 0;
   (*destination).delta |= (packedData[0] & 0x3F) << 10;
   (*destination).delta |= (packedData[1] & 0xFF) << 2;
   (*destination).delta |= ((packedData[2] >> 6) & 0x3);
   (*destination).reference = 0;
   (*destination).reference |= (packedData[2] & 0x3F) << 10;
   (*destination).reference |= (packedData[3] & 0xFF) << 2;
   (*destination).reference |= ((packedData[4] >> 6) & 0x3);
   (*destination).difference = 0;
   (*destination).difference |= (packedData[4] & 0x3F) << 10;
   (*destination).difference |= (packedData[5] & 0xFF) << 2;
   (*destination).difference |= ((packedData[6] >> 6) & 0x3);
}

void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackCluster(
   uint8_t* const packedData,
   const NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type* const source)
{
   packedData[0] = (uint8_t)(((*source).Ch_On & 0x1) << 7);
   packedData[0] |= (uint8_t)(((*source).FSA_On & 0x1) << 6);
   packedData[0] |= (uint8_t)(((*source).delta >> 10) & 0x3F);
   packedData[1] = (uint8_t)(((*source).delta >> 2) & 0xFF);
   packedData[2] = (uint8_t)(((*source).delta & 0x3) << 6);
   packedData[2] |= (uint8_t)(((*source).reference >> 10) & 0x3F);
   packedData[3] = (uint8_t)(((*source).reference >> 2) & 0xFF);
   packedData[4] = (uint8_t)(((*source).reference & 0x3) << 6);
   packedData[4] |= (uint8_t)(((*source).difference >> 10) & 0x3F);
   packedData[5] = (uint8_t)(((*source).difference >> 2) & 0xFF);
   packedData[6] = (uint8_t)(((*source).difference & 0x3) << 6);
}
