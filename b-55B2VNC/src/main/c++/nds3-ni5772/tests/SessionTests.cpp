#include "DiamondSession.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#define CHECK(c) do { if (!(c)) throw std::runtime_error(#c); } while (false)
namespace fake {
int initialized = 0, finalized = 0, opened = 0, closed = 0, resets = 0;
int started = 0, stopped = 0;
bool failOpen = false, failRead = false;
int failStart = -1;
std::size_t available = 11, lastCount = 0;
std::int64_t lastValue = 0;
std::string lastWrite;
std::uint64_t readValue = UINT64_MAX;
}
extern "C" {
NiFpga_Status NiFpga_Initialize() { ++fake::initialized; return 0; }
NiFpga_Status NiFpga_Finalize() { ++fake::finalized; return 0; }
NiFpga_Status NiFpga_Open(const char*, const char* sig, const char*, uint32_t flags, NiFpga_Session* session) {
    CHECK(std::string(sig) == "06B443BD4564E81B9835D621C0EEF203");
    CHECK(flags == NiFpga_OpenAttribute_NoRun);
    if (fake::failOpen) return -100;
    *session = ++fake::opened; return 0;
}
NiFpga_Status NiFpga_Close(NiFpga_Session, uint32_t) { ++fake::closed; return 0; }
NiFpga_Status NiFpga_Reset(NiFpga_Session) { ++fake::resets; return 0; }
NiFpga_Status NiFpga_Run(NiFpga_Session, uint32_t) { return 0; }
NiFpga_Status NiFpga_ConfigureFifo2(NiFpga_Session, uint32_t, size_t depth, size_t* actual) { *actual = depth; return 0; }
NiFpga_Status NiFpga_StartFifo(NiFpga_Session, uint32_t) {
    if (fake::failStart >= 0 && fake::started == fake::failStart) return -101;
    ++fake::started; return 0;
}
NiFpga_Status NiFpga_StopFifo(NiFpga_Session, uint32_t) { ++fake::stopped; return 0; }
#define WRITE(SUFFIX, T) NiFpga_Status NiFpga_Write##SUFFIX(NiFpga_Session, uint32_t, T v) { \
    fake::lastWrite = #SUFFIX; fake::lastValue = v; return 0; }
WRITE(Bool, NiFpga_Bool)
WRITE(I16, int16_t)
WRITE(I32, int32_t)
WRITE(U8, uint8_t)
WRITE(U16, uint16_t)
WRITE(U32, uint32_t)
#undef WRITE
#define READ(SUFFIX, T) NiFpga_Status NiFpga_Read##SUFFIX(NiFpga_Session, uint32_t, T* v) { \
    *v = static_cast<T>(fake::readValue); return 0; }
READ(Bool, NiFpga_Bool)
READ(I32, int32_t)
READ(U32, uint32_t)
READ(U64, uint64_t)
#undef READ
NiFpga_Status NiFpga_ReadArrayU8(NiFpga_Session, uint32_t, uint8_t* v, size_t count) {
    CHECK(count == 9);
    const uint8_t bytes[] = {0, 0, 0x16, 0x8c, 0, 0, 0x16, 0x8c, 0xfc};
    std::memcpy(v, bytes, 9); return 0;
}
#define FIFO(SUFFIX, T) NiFpga_Status NiFpga_ReadFifo##SUFFIX(NiFpga_Session, uint32_t, T* v, size_t count, uint32_t timeout, size_t* remaining) { \
    CHECK(timeout == 0); \
    if (fake::failRead) return -102; \
    CHECK(count <= fake::available); \
    if (count) { for (size_t i = 0; i < count; ++i) v[i] = static_cast<T>(i); fake::lastCount = count; } \
    *remaining = fake::available - count; return 0; }
FIFO(I16, int16_t)
FIFO(I64, int64_t)
FIFO(U32, uint32_t)
FIFO(U64, uint64_t)
#undef FIFO
}

template<typename F> void throws(F f) {
    bool failed = false;
    try { f(); } catch (const std::exception&) { failed = true; }
    CHECK(failed);
}
int main() {
    try {
        diamond::Session first, second;
        throws([&] { first.read("Run_Ch0"); });
        first.open("test.lvbitx", "RIO0"); second.open("test.lvbitx", "RIO1");
        CHECK(fake::initialized == 1 && fake::opened == 2 && fake::resets == 2);
        {
            diamond::Session duplicate;
            throws([&] { duplicate.open("test.lvbitx", "RIO0"); });
        }
        CHECK(fake::finalized == 0);
        first.prepareFifos(65536); CHECK(fake::started == 10);
        first.write("PeakMin_Ch1", -32768); CHECK(fake::lastWrite == "I16" && fake::lastValue == -32768);
        first.write("PeakShift_Ch0", 13); CHECK(fake::lastWrite == "I32");
        first.write("GateTicks", 4294967295LL); CHECK(fake::lastWrite == "U32" && fake::lastValue == 4294967295LL);
        CHECK(first.read("ScopeLost_Ch0") == UINT64_MAX);
        auto config = first.deviceConfig(); CHECK(config.size() == 8 && config[0] == 5772 && config[1] == 5772);
        for (size_t i = 2; i < config.size(); ++i) CHECK(config[i] == 1);
        CHECK(first.readI64("PulseRecords_ToHost_Ch0", 7).size() == 4);
        CHECK(first.readU64("RateRecords_ToHost_Ch0", 20, 4).size() == 8);
        CHECK(first.readU64("ScopeRecords_ToHost_Ch0", 20).size() == 11);
        CHECK(first.readU32("SpectrumBlocks_ToHost_Ch0", 6).size() == 6);
        CHECK(first.readI16("RawData_ToHost_Ch0", 20).size() == 8);
        throws([&] { first.readI16("PulseRecords_ToHost_Ch0", 10); });
        fake::failRead = true;
        throws([&] { first.readI64("PulseRecords_ToHost_Ch0", 10); });
        CHECK(first.status() == -102); fake::failRead = false;
        first.close(); CHECK(fake::stopped == 10 && fake::finalized == 0);
        second.close(); CHECK(fake::finalized == 1 && fake::closed == 2);
        second.close(); CHECK(fake::finalized == 1);
        fake::failOpen = true;
        throws([&] { first.open("test.lvbitx", "RIO0"); });
        CHECK(!first.isOpen() && first.status() == -100 && fake::finalized == 2);
        fake::failOpen = false;
        first.open("test.lvbitx", "RIO0"); // failed open released resource reservation
        fake::failStart = fake::started + 2;
        throws([&] { first.prepareFifos(65536); });
        first.close(); CHECK(fake::stopped == 12 && fake::finalized == 3);
        std::cout << "DiamondSession: all mock transport tests passed\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n'; return EXIT_FAILURE;
    }
}
