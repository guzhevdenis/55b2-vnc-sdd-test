/*
 * Generated with the FPGA Interface C API Generator 26.0
 * for NI-RIO 26.0 or later.
 */
#ifndef __NiFpga_Fission_Chamber_DAQ_FPGA_Top_h__
#define __NiFpga_Fission_Chamber_DAQ_FPGA_Top_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 260
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Fission_Chamber_DAQ_FPGA_Top_Bitfile;
 */
#define NiFpga_Fission_Chamber_DAQ_FPGA_Top_Bitfile "NiFpga_Fission_Chamber_DAQ_FPGA_Top.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Fission_Chamber_DAQ_FPGA_Top_Signature = "35CCE30A3767256CA984A0A50B257C68";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorBool_DAQ_On = 0x80010012,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorBool_Input_Valid = 0x8001001E,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorBool_ReadyforInput = 0x80010022
} NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorBool;

typedef enum
{
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlBool_EnableDAQ = 0x80010016
} NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlBool;

typedef enum
{
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_Baseline_ToHost_Ch0 = 5,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_Exp1Stage_ToHost_Ch0 = 4,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_Filtered_ToHost_Ch0 = 3,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch0 = 0
} NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16;

typedef enum
{
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_FluctCountRate_Hz_ToHost_Ch0 = 2,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_PulseChain_ToHost_Ch0 = 1
} NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32;

/* Indicator: ADC_Status */
/* Use NiFpga_ReadArrayU8() to access ADC_Status */
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Resource = 0x10008;
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackedSizeInBytes = 9;

typedef struct NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type{
   uint32_t Expected_ID;
   uint32_t Inserted_ID;
   NiFpga_Bool EEPROMPowerEnabled;
   NiFpga_Bool IOEnabled;
   NiFpga_Bool PowerEnabled;
   NiFpga_Bool PowerGood;
   NiFpga_Bool Present;
   NiFpga_Bool InitializationDone;
}NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type;


void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type* const destination);

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_ADC_Status_Type* const source);

/* Indicator: Dev_Status */
/* Use NiFpga_ReadArrayU8() to access Dev_Status */
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Resource = 0x10004;
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_PackedSizeInBytes = 6;

typedef struct NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type{
   uint16_t Power3_3V;
   uint16_t Power12V;
   int8_t Temperature_degC;
   NiFpga_Bool Clk100PLL_Unlocked;
}NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type;


void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type* const destination);

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_IndicatorCluster_Dev_Status_Type* const source);

/* Control: Filter_Ch0 */
/* Use NiFpga_WriteArrayU8() to access Filter_Ch0 */
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Resource = 0x80010018;
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackedSizeInBytes = 6;

typedef struct NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type{
   int16_t FirstExpCoeff;
   int16_t SecondExpCoeff;
   uint16_t Type;
}NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type;


void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type* const destination);

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_Filter_Ch0_Type* const source);

/* Control: FluctChainCfg_Ch0 */
/* Use NiFpga_WriteArrayU8() to access FluctChainCfg_Ch0 */
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Resource = 0x1000C;
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_PackedSizeInBytes = 16;

typedef struct NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type{
   int64_t MSVnoise;
   uint64_t CalibrationCoefficient;
}NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type;


void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type* const destination);

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_FluctChainCfg_Ch0_Type* const source);

/* Control: PulseChainCfg_Ch0 */
/* Use NiFpga_WriteArrayU8() to access PulseChainCfg_Ch0 */
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Resource = 0x10000;
static const uint32_t NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackedSizeInBytes = 8;

typedef struct NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type{
   int16_t Threshold;
   uint16_t SinglePulseMax;
   uint16_t SinglePulseMin;
   uint16_t TotalPulseMin;
}NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type;


void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type* const destination);

void NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_Fission_Chamber_DAQ_FPGA_Top_ControlCluster_PulseChainCfg_Ch0_Type* const source);


#if NiFpga_Cpp
}
#endif

#endif
