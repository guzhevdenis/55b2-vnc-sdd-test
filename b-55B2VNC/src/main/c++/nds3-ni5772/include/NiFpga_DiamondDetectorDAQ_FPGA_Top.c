/*
 * Generated with the FPGA Interface C API Generator 26.0
 * for NI-RIO 26.0 or later.
 */
#include "NiFpga_DiamondDetectorDAQ_FPGA_Top.h"

void NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Type* const destination)
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

void NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_PackCluster(
   uint8_t* const packedData,
   const NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Type* const source)
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
