/*
 * Generated with the FPGA Interface C API Generator 18.0.0
 * for NI-RIO 18.0.0 or later.
 */

#ifndef __NiFpga_exp_test_FIC_h__
#define __NiFpga_exp_test_FIC_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1800
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_exp_test_FIC_Bitfile;
 */
#define NiFpga_exp_test_FIC_Bitfile "NiFpga_exp_test_FIC.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_exp_test_FIC_Signature = "697EBA8139F933149549CF69FEF17105";

typedef enum
{
   NiFpga_exp_test_FIC_IndicatorBool_Clock100PLLUnlocked = 0x1000E,
   NiFpga_exp_test_FIC_IndicatorBool_EEPROMPowerEnabled = 0x1001E,
   NiFpga_exp_test_FIC_IndicatorBool_IOModuleIOEnabled = 0x1002A,
   NiFpga_exp_test_FIC_IndicatorBool_IOModuleInitializationDone = 0x1000A,
   NiFpga_exp_test_FIC_IndicatorBool_IOModulePowerEnabled = 0x1002E,
   NiFpga_exp_test_FIC_IndicatorBool_IOModulePowerGood = 0x10032,
   NiFpga_exp_test_FIC_IndicatorBool_IOModulePresent = 0x10036,
   NiFpga_exp_test_FIC_IndicatorBool_Key = 0x8001007E,
} NiFpga_exp_test_FIC_IndicatorBool;

typedef enum
{
   NiFpga_exp_test_FIC_IndicatorI8_DeviceTemperature = 0x1001A,
} NiFpga_exp_test_FIC_IndicatorI8;

typedef enum
{
   NiFpga_exp_test_FIC_IndicatorI16_ResultAfterSub = 0x10066,
   NiFpga_exp_test_FIC_IndicatorI16_ResultInsideCounter = 0x10072,
} NiFpga_exp_test_FIC_IndicatorI16;

typedef enum
{
   NiFpga_exp_test_FIC_IndicatorU16_Device12VPower = 0x10016,
   NiFpga_exp_test_FIC_IndicatorU16_Device33VPower = 0x10012,
} NiFpga_exp_test_FIC_IndicatorU16;

typedef enum
{
   NiFpga_exp_test_FIC_IndicatorI32_Counter1 = 0x80010090,
   NiFpga_exp_test_FIC_IndicatorI32_Counter2 = 0x10068,
   NiFpga_exp_test_FIC_IndicatorI32_Counter3 = 0x1006C,
   NiFpga_exp_test_FIC_IndicatorI32_CounterTemp = 0x10038,
   NiFpga_exp_test_FIC_IndicatorI32_SinglePulsesCountRate = 0x10054,
   NiFpga_exp_test_FIC_IndicatorI32_SinglePulsesRegistered = 0x10044,
   NiFpga_exp_test_FIC_IndicatorI32_TotalCountRate = 0x10050,
   NiFpga_exp_test_FIC_IndicatorI32_TotalPulsesRegistered = 0x10048,
} NiFpga_exp_test_FIC_IndicatorI32;

typedef enum
{
   NiFpga_exp_test_FIC_IndicatorU32_ExpectedIOModuleID = 0x10020,
   NiFpga_exp_test_FIC_IndicatorU32_InsertedIOModuleID = 0x10024,
} NiFpga_exp_test_FIC_IndicatorU32;

typedef enum
{
   NiFpga_exp_test_FIC_ControlBool_ControlKey = 0x8001008E,
   NiFpga_exp_test_FIC_ControlBool_ThresholdValid = 0x1005A,
} NiFpga_exp_test_FIC_ControlBool;

typedef enum
{
   NiFpga_exp_test_FIC_ControlI16_ALPHA16bitfirst = 0x80010082,
   NiFpga_exp_test_FIC_ControlI16_ALPHA16bitsecond = 0x80010086,
   NiFpga_exp_test_FIC_ControlI16_Threshold = 0x1005E,
} NiFpga_exp_test_FIC_ControlI16;

typedef enum
{
   NiFpga_exp_test_FIC_ControlU16_Filtertype = 0x8001008A,
} NiFpga_exp_test_FIC_ControlU16;

typedef enum
{
   NiFpga_exp_test_FIC_ControlU32_maxticksperpulse = 0x1004C,
   NiFpga_exp_test_FIC_ControlU32_minticksperpulse = 0x10040,
   NiFpga_exp_test_FIC_ControlU32_minticksperpulse2 = 0x10060,
} NiFpga_exp_test_FIC_ControlU32;

typedef enum
{
   NiFpga_exp_test_FIC_ControlI64_CalibratedcoefficientINT64 = 0x1003C,
} NiFpga_exp_test_FIC_ControlI64;

typedef enum
{
   NiFpga_exp_test_FIC_TargetToHostFifoI16_1stage = 6,
   NiFpga_exp_test_FIC_TargetToHostFifoI16_Correctedsignal = 5,
   NiFpga_exp_test_FIC_TargetToHostFifoI16_PulsePeaksFIFO = 2,
   NiFpga_exp_test_FIC_TargetToHostFifoI16_RawData = 1,
   NiFpga_exp_test_FIC_TargetToHostFifoI16_TarToHost = 0,
} NiFpga_exp_test_FIC_TargetToHostFifoI16;

typedef enum
{
   NiFpga_exp_test_FIC_TargetToHostFifoU64_Countratefluctuation = 4,
   NiFpga_exp_test_FIC_TargetToHostFifoU64_MSVADC = 3,
} NiFpga_exp_test_FIC_TargetToHostFifoU64;

#endif
