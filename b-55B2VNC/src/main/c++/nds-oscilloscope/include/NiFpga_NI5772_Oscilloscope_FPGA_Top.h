/*
 * Generated with the FPGA Interface C API Generator 26.0
 * for NI-RIO 26.0 or later.
 */
#ifndef __NiFpga_NI5772_Oscilloscope_FPGA_Top_h__
#define __NiFpga_NI5772_Oscilloscope_FPGA_Top_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 260
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_NI5772_Oscilloscope_FPGA_Top_Bitfile;
 */
#define NiFpga_NI5772_Oscilloscope_FPGA_Top_Bitfile "NiFpga_NI5772_Oscilloscope_FPGA_Top.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_NI5772_Oscilloscope_FPGA_Top_Signature = "6E08D498BD314388AAC8D2AE8C05F24F";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_DAQ_On = 0x8001001E,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_IO_ModuleInitialization_Done = 0x10022,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_Rec_0 = 0x80010006,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool_Rec_1 = 0x8001000E
} NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorBool;

typedef enum
{
   NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlBool_DAQ_En = 0x8001001A
} NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlBool;

typedef enum
{
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16_Elements_0 = 0x80010000,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16_Elements_1 = 0x80010008
} NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16;

typedef enum
{
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16Size_Elements_0 = 4,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16Size_Elements_1 = 4
} NiFpga_NI5772_Oscilloscope_FPGA_Top_IndicatorArrayI16Size;

typedef enum
{
   NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch0 = 1,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch1 = 0
} NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16;

/* Control: Ch_0 */
/* Use NiFpga_WriteArrayU8() to access Ch_0 */
static const uint32_t NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Resource = 0x80010010;
static const uint32_t NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackedSizeInBytes = 7;

typedef struct NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type{
   NiFpga_Bool Ch_On;
   NiFpga_Bool FSA_On;
   int16_t delta;
   int16_t reference;
   int16_t difference;
}NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type;


void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type* const destination);

void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_PackCluster(
   uint8_t* const packedData,
   const NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_0_Type* const source);

/* Control: Ch_1 */
/* Use NiFpga_WriteArrayU8() to access Ch_1 */
static const uint32_t NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Resource = 0x80010014;
static const uint32_t NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackedSizeInBytes = 7;

typedef struct NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type{
   NiFpga_Bool Ch_On;
   NiFpga_Bool FSA_On;
   int16_t delta;
   int16_t reference;
   int16_t difference;
}NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type;


void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_UnpackCluster(
   const uint8_t* const packedData,
   NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type* const destination);

void NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_PackCluster(
   uint8_t* const packedData,
   const NiFpga_NI5772_Oscilloscope_FPGA_Top_ControlCluster_Ch_1_Type* const source);


#if NiFpga_Cpp
}
#endif

#endif
