/*
 * Generated with the FPGA Interface C API Generator 26.0
 * for NI-RIO 26.0 or later.
 */
#ifndef __NiFpga_DiamondDetectorDAQ_FPGA_Top_h__
#define __NiFpga_DiamondDetectorDAQ_FPGA_Top_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 260
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_DiamondDetectorDAQ_FPGA_Top_Bitfile;
 */
#define NiFpga_DiamondDetectorDAQ_FPGA_Top_Bitfile "NiFpga_DiamondDetectorDAQ_FPGA_Top.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_DiamondDetectorDAQ_FPGA_Top_Signature = "06B443BD4564E81B9835D621C0EEF203";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorBool_Run_Ch0 = 0x80010032,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorBool_Run_Ch1 = 0x8001002E
} NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorBool;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorI32_Baseline_Ch0 = 0x80010014,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorI32_Baseline_Ch1 = 0x80010020
} NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorI32;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU32_SpecOverrun_Ch0 = 0x80010088,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU32_SpecOverrun_Ch1 = 0x8001008C
} NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU32;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_PulseLost_Ch0 = 0x80010040,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_PulseLost_Ch1 = 0x80010048,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_RateLost_Ch0 = 0x80010050,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_RateLost_Ch1 = 0x80010054,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_RawLost_Ch0 = 0x8001003C,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_RawLost_Ch1 = 0x80010044,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_ScopeLost_Ch0 = 0x800100A0,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_ScopeLost_Ch1 = 0x800100B4,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_SpecLost_Ch0 = 0x80010070,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64_SpecLost_Ch1 = 0x80010084
} NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorU64;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_BaselineHold_Ch0 = 0x80010012,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_BaselineHold_Ch1 = 0x8001001E,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_ClearLossCounters = 0x8001003A,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_RawEnable_Ch0 = 0x800100A6,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_RawEnable_Ch1 = 0x800100BA,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_ScopeEnable_Ch0 = 0x80010092,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_ScopeEnable_Ch1 = 0x800100AA,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_Start_Ch0 = 0x80010026,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_Start_Ch1 = 0x8001002A,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool_TriggerReset = 0x80010036
} NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlBool;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU8_ScopePreTicks_Ch0 = 0x80010096,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU8_ScopePreTicks_Ch1 = 0x800100AE
} NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU8;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI16_IntegralMin_Ch1 = 0x8001007E,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI16_IntegralShift_Ch1 = 0x80010082,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI16_PeakMin_Ch1 = 0x80010076,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI16_PeakShift_Ch1 = 0x8001007A
} NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI16;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU16_ScopeRecordTicks_Ch0 = 0x8001009A,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU16_ScopeRecordTicks_Ch1 = 0x800100B2,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU16_TailTicks_Ch0 = 0x8001000E,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU16_TailTicks_Ch1 = 0x8001001A
} NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU16;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI32_IntegralMin_Ch0 = 0x80010068,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI32_IntegralShift_Ch0 = 0x8001006C,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI32_PeakMin_Ch0 = 0x80010060,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI32_PeakShift_Ch0 = 0x80010064,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI32_Threshold_Ch0 = 0x80010004,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI32_Threshold_Ch1 = 0x80010008
} NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlI32;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU32_GateTicks = 0x8001004C,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU32_MaxLength_Ch0 = 0x80010058,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU32_MaxLength_Ch1 = 0x8001005C,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU32_ScopeHoldOffTicks = 0x8001009C
} NiFpga_DiamondDetectorDAQ_FPGA_Top_ControlU32;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch0 = 5,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch1 = 4
} NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoI16;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU32_SpectrumBlocks_ToHost_Ch0 = 1,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU32_SpectrumBlocks_ToHost_Ch1 = 0
} NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU32;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoI64_PulseRecords_ToHost_Ch0 = 9,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoI64_PulseRecords_ToHost_Ch1 = 8
} NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoI64;

typedef enum
{
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU64_RateRecords_ToHost_Ch0 = 7,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU64_RateRecords_ToHost_Ch1 = 6,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU64_ScopeRecords_ToHost_Ch0 = 3,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU64_ScopeRecords_ToHost_Ch1 = 2
} NiFpga_DiamondDetectorDAQ_FPGA_Top_TargetToHostFifoU64;

/* Indicator: DeviceConfig */
/* Use NiFpga_ReadArrayU8() to access DeviceConfig */
static const uint32_t NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Resource = 0x10000;
static const uint32_t NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_PackedSizeInBytes = 9;

typedef struct NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Type{
   uint32_t Expected_ID;
   uint32_t Inserted_ID;
   NiFpga_Bool EEPROMPowerEnabled;
   NiFpga_Bool IOEnabled;
   NiFpga_Bool PowerEnabled;
   NiFpga_Bool PowerGood;
   NiFpga_Bool Present;
   NiFpga_Bool InitializationDone;
}NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Type;


void NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Type* const destination);

void NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_PackCluster(
   uint8_t* const packedData,
   const NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Type* const source);


#if NiFpga_Cpp
}
#endif

#endif
