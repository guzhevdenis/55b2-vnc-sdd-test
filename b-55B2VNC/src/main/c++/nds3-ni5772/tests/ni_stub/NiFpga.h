#ifndef TEST_NIFPGA_H
#define TEST_NIFPGA_H
// Test double only. Never add this directory to the production include path.
#include <stdint.h>
#include <stddef.h>
#define NiFpga_Cpp 1
typedef int32_t NiFpga_Status;
typedef uint32_t NiFpga_Session;
typedef uint8_t NiFpga_Bool;
enum { NiFpga_OpenAttribute_NoRun = 1 };
extern "C" {
NiFpga_Status NiFpga_Initialize(void);
NiFpga_Status NiFpga_Finalize(void);
NiFpga_Status NiFpga_Open(const char*, const char*, const char*, uint32_t, NiFpga_Session*);
NiFpga_Status NiFpga_Close(NiFpga_Session, uint32_t);
NiFpga_Status NiFpga_Reset(NiFpga_Session);
NiFpga_Status NiFpga_Run(NiFpga_Session, uint32_t);
NiFpga_Status NiFpga_ConfigureFifo2(NiFpga_Session, uint32_t, size_t, size_t*);
NiFpga_Status NiFpga_StartFifo(NiFpga_Session, uint32_t);
NiFpga_Status NiFpga_StopFifo(NiFpga_Session, uint32_t);
#define DECLARE_WRITE(SUFFIX, T) NiFpga_Status NiFpga_Write##SUFFIX(NiFpga_Session, uint32_t, T);
DECLARE_WRITE(Bool, NiFpga_Bool)
DECLARE_WRITE(I16, int16_t)
DECLARE_WRITE(I32, int32_t)
DECLARE_WRITE(U8, uint8_t)
DECLARE_WRITE(U16, uint16_t)
DECLARE_WRITE(U32, uint32_t)
#undef DECLARE_WRITE
#define DECLARE_READ(SUFFIX, T) NiFpga_Status NiFpga_Read##SUFFIX(NiFpga_Session, uint32_t, T*);
DECLARE_READ(Bool, NiFpga_Bool)
DECLARE_READ(I32, int32_t)
DECLARE_READ(U32, uint32_t)
DECLARE_READ(U64, uint64_t)
#undef DECLARE_READ
NiFpga_Status NiFpga_ReadArrayU8(NiFpga_Session, uint32_t, uint8_t*, size_t);
#define DECLARE_FIFO(SUFFIX, T) NiFpga_Status NiFpga_ReadFifo##SUFFIX(NiFpga_Session, uint32_t, T*, size_t, uint32_t, size_t*);
DECLARE_FIFO(I16, int16_t)
DECLARE_FIFO(I64, int64_t)
DECLARE_FIFO(U32, uint32_t)
DECLARE_FIFO(U64, uint64_t)
#undef DECLARE_FIFO
}
#endif
