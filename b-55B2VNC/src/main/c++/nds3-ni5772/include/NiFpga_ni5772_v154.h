/*
 * Generated with the FPGA Interface C API Generator 18.0.0
 * for NI-RIO 18.0.0 or later.
 */

#ifndef __NiFpga_main_v154_h__
#define __NiFpga_main_v154_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1800
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_main_v154_Bitfile;
 */
#define NiFpga_main_v154_Bitfile "NiFpga_main_v154.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_main_v154_Signature = "5444E3522C18F4437A8BA0B589152E06";

typedef enum
{
   NiFpga_main_v154_IndicatorBool_Clock100PLLUnlocked = 0x1006A,
   NiFpga_main_v154_IndicatorBool_EEPROMPowerEnabled = 0x1007A,
   NiFpga_main_v154_IndicatorBool_IOModuleIOEnabled = 0x10086,
   NiFpga_main_v154_IndicatorBool_IOModuleInitializationDone = 0x10066,
   NiFpga_main_v154_IndicatorBool_IOModulePowerEnabled = 0x1008A,
   NiFpga_main_v154_IndicatorBool_IOModulePowerGood = 0x1008E,
   NiFpga_main_v154_IndicatorBool_IOModulePresent = 0x10092,
   NiFpga_main_v154_IndicatorBool_Reg3 = 0x10052,
   NiFpga_main_v154_IndicatorBool_Reg5 = 0x1004A,
   NiFpga_main_v154_IndicatorBool_reg4 = 0x1004E,
} NiFpga_main_v154_IndicatorBool;

typedef enum
{
   NiFpga_main_v154_IndicatorI8_DeviceTemperature = 0x10076,
} NiFpga_main_v154_IndicatorI8;

typedef enum
{
   NiFpga_main_v154_IndicatorU8_streamstate = 0x80010016,
} NiFpga_main_v154_IndicatorU8;

typedef enum
{
   NiFpga_main_v154_IndicatorI16_Element = 0x10042,
   NiFpga_main_v154_IndicatorI16_IOModuleAI1DataN3 = 0x8001001A,
} NiFpga_main_v154_IndicatorI16;

typedef enum
{
   NiFpga_main_v154_IndicatorU16_Device12VPower = 0x10072,
   NiFpga_main_v154_IndicatorU16_Device33VPower = 0x1006E,
} NiFpga_main_v154_IndicatorU16;

typedef enum
{
   NiFpga_main_v154_IndicatorU32_ExpectedIOModuleID = 0x1007C,
   NiFpga_main_v154_IndicatorU32_InsertedIOModuleID = 0x10080,
} NiFpga_main_v154_IndicatorU32;

typedef enum
{
   NiFpga_main_v154_IndicatorU64_1cyclesum = 0x10044,
   NiFpga_main_v154_IndicatorU64_INTEGRAL = 0x1001C,
   NiFpga_main_v154_IndicatorU64_Reg4Data = 0x10054,
   NiFpga_main_v154_IndicatorU64_SinglePulsesCountRate = 0x10034,
   NiFpga_main_v154_IndicatorU64_SinglePulsesRegistered = 0x10024,
   NiFpga_main_v154_IndicatorU64_TotalCountRate = 0x10030,
   NiFpga_main_v154_IndicatorU64_TotalPulsesRegistered = 0x10028,
   NiFpga_main_v154_IndicatorU64_streammetadata = 0x8001000C,
   NiFpga_main_v154_IndicatorU64_streamsamplestransferred = 0x80010010,
} NiFpga_main_v154_IndicatorU64;

typedef enum
{
   NiFpga_main_v154_ControlBool_ThresholdValid = 0x1003A,
   NiFpga_main_v154_ControlBool_start = 0x1005E,
} NiFpga_main_v154_ControlBool;

typedef enum
{
   NiFpga_main_v154_ControlU8_streamrequeststate = 0x8001000A,
} NiFpga_main_v154_ControlU8;

typedef enum
{
   NiFpga_main_v154_ControlI16_Threshold = 0x1003E,
   NiFpga_main_v154_ControlI16_hysteresis = 0x1005A,
} NiFpga_main_v154_ControlI16;

typedef enum
{
   NiFpga_main_v154_ControlU16_ChannelSelect = 0x80010002,
} NiFpga_main_v154_ControlU16;

typedef enum
{
   NiFpga_main_v154_ControlU32_maxticksperpulse = 0x1002C,
   NiFpga_main_v154_ControlU32_minticksperpulse = 0x10020,
} NiFpga_main_v154_ControlU32;

typedef enum
{
   NiFpga_main_v154_ControlU64_streamnumsamples = 0x80010004,
} NiFpga_main_v154_ControlU64;

typedef enum
{
   NiFpga_main_v154_TargetToHostFifoI16_PulsePeaksFIFO = 1,
   NiFpga_main_v154_TargetToHostFifoI16_ToHostFIFO = 0,
} NiFpga_main_v154_TargetToHostFifoI16;

#endif
