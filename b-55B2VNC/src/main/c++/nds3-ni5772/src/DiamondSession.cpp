#include "DiamondSession.h"
#include "DiamondGenerated.h"
#include <algorithm>
#include <cstring>
#include <mutex>
#include <set>
#include <stdexcept>

namespace diamond {
namespace {
std::mutex runtimeMutex;
std::size_t runtimeUsers = 0;
std::set<std::string> activeResources;
bool isFifo(Kind k) {
    return k == Kind::FifoI16 || k == Kind::FifoI64 || k == Kind::FifoU32 || k == Kind::FifoU64;
}
std::int32_t bits32(std::uint32_t v) { std::int32_t r; std::memcpy(&r, &v, sizeof(r)); return r; }
}
Session::Session() : session_(0), status_(0), open_(false), runtime_(false), resources_(diamond::resources()) {}
Session::~Session() { close(); }
const Resource& Session::resource(const std::string& name) const {
    for (const auto& r : resources_) if (name == r.name) return r;
    throw std::invalid_argument("Unknown FPGA resource: " + name);
}
void Session::requireOpen() const {
    if (!open_) throw std::runtime_error("FPGA session is OFF");
}
void Session::check(NiFpga_Status result, const std::string& operation) {
    if (result < 0) {
        status_ = result;
        throw std::runtime_error(operation + ": NiFpga status " + std::to_string(result));
    }
    if (result > 0 && status_ == 0) status_ = result;
}
void Session::open(const std::string& bitfile, const std::string& rio) {
    if (open_) return;
    status_ = 0;
    {
        std::lock_guard<std::mutex> guard(runtimeMutex);
        if (activeResources.count(rio)) throw std::logic_error("RIO resource already used by a Diamond driver: " + rio);
        if (!runtimeUsers) check(NiFpga_Initialize(), "Initialize");
        activeResources.insert(rio);
        rio_ = rio;
        ++runtimeUsers;
        runtime_ = true;
    }
    try {
        check(NiFpga_Open(bitfile.c_str(), diamond::signature(), rio.c_str(),
                         NiFpga_OpenAttribute_NoRun, &session_), "Open " + bitfile);
        open_ = true;
        reset();
    } catch (...) { close(); throw; }
}
void Session::close() noexcept {
    if (open_) {
        stopFifos();
        NiFpga_Status r = NiFpga_Close(session_, 0);
        if (r < 0 && status_ >= 0) status_ = r;
        open_ = false;
        session_ = 0;
    }
    if (runtime_) {
        std::lock_guard<std::mutex> guard(runtimeMutex);
        activeResources.erase(rio_);
        if (--runtimeUsers == 0) {
            NiFpga_Status r = NiFpga_Finalize();
            if (r < 0 && status_ >= 0) status_ = r;
        }
        runtime_ = false;
    }
}
void Session::reset() {
    requireOpen();
    stopFifos();
    check(NiFpga_Reset(session_), "Reset");
    started_.clear();
}
void Session::prepareFifos(std::size_t depth) {
    requireOpen();
    for (const auto& r : resources_) {
        if (!isFifo(r.kind)) continue;
        std::size_t actual = 0;
        check(NiFpga_ConfigureFifo2(session_, r.id, depth, &actual), std::string("Configure ") + r.name);
        if (actual < depth) throw std::runtime_error("FIFO depth below requested depth");
        check(NiFpga_StartFifo(session_, r.id), std::string("Start ") + r.name);
        started_.push_back(r.id);
    }
}
void Session::stopFifos() noexcept {
    for (auto id : started_) {
        NiFpga_Status r = NiFpga_StopFifo(session_, id);
        if (r < 0 && status_ >= 0) status_ = r;
    }
    started_.clear();
}
void Session::run() { requireOpen(); check(NiFpga_Run(session_, 0), "Run"); }
void Session::write(const std::string& name, std::int64_t value) {
    requireOpen();
    const auto& r = resource(name);
    NiFpga_Status result;
    switch (r.kind) {
    case Kind::ControlBool: result = NiFpga_WriteBool(session_, r.id, value != 0); break;
    case Kind::ControlI16: result = NiFpga_WriteI16(session_, r.id, static_cast<std::int16_t>(value)); break;
    case Kind::ControlI32: result = NiFpga_WriteI32(session_, r.id, static_cast<std::int32_t>(value)); break;
    case Kind::ControlU8: result = NiFpga_WriteU8(session_, r.id, static_cast<std::uint8_t>(value)); break;
    case Kind::ControlU16: result = NiFpga_WriteU16(session_, r.id, static_cast<std::uint16_t>(value)); break;
    case Kind::ControlU32: result = NiFpga_WriteU32(session_, r.id, static_cast<std::uint32_t>(value)); break;
    default: throw std::invalid_argument("Not a control: " + name);
    }
    check(result, "Write " + name);
}
std::uint64_t Session::read(const std::string& name) {
    requireOpen();
    const auto& r = resource(name);
    NiFpga_Status result;
    std::uint64_t value = 0;
    switch (r.kind) {
    case Kind::IndicatorBool: { NiFpga_Bool v = 0; result = NiFpga_ReadBool(session_, r.id, &v); value = v; break; }
    case Kind::IndicatorI32: { std::int32_t v = 0; result = NiFpga_ReadI32(session_, r.id, &v); value = static_cast<std::uint64_t>(v); break; }
    case Kind::IndicatorU32: { std::uint32_t v = 0; result = NiFpga_ReadU32(session_, r.id, &v); value = v; break; }
    case Kind::IndicatorU64: result = NiFpga_ReadU64(session_, r.id, &value); break;
    default: throw std::invalid_argument("Not an indicator: " + name);
    }
    check(result, "Read " + name);
    return value;
}
std::vector<std::int32_t> Session::deviceConfig() {
    requireOpen();
    std::uint8_t packed[NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_PackedSizeInBytes] = {};
    check(NiFpga_ReadArrayU8(session_, NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Resource,
                           packed, sizeof(packed)), "Read DeviceConfig");
    NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_Type c = {};
    NiFpga_DiamondDetectorDAQ_FPGA_Top_IndicatorCluster_DeviceConfig_UnpackCluster(packed, &c);
    return {bits32(c.Expected_ID), bits32(c.Inserted_ID), c.EEPROMPowerEnabled,
            c.IOEnabled, c.PowerEnabled, c.PowerGood, c.Present, c.InitializationDone};
}

// One reader per FIFO: data counted by the zero-length query remains available.
// Every read is bounded and nonblocking; fixed records are read in groups of 4.
#define READ_FIFO(TYPE, SUFFIX, KIND, ALIGN) \
std::vector<TYPE> Session::read##SUFFIX(const std::string& name, std::size_t limit) { \
    requireOpen(); const auto& r = resource(name); \
    if (r.kind != Kind::KIND) throw std::invalid_argument("Wrong FIFO type: " + name); \
    std::size_t available = 0; TYPE dummy = 0; \
    check(NiFpga_ReadFifo##SUFFIX(session_, r.id, &dummy, 0, 0, &available), "Query " + name); \
    std::size_t count = std::min(available, limit); count -= count % ALIGN; \
    std::vector<TYPE> data(count); \
    if (count) check(NiFpga_ReadFifo##SUFFIX(session_, r.id, data.data(), count, 0, &available), "Read " + name); \
    return data; \
}
READ_FIFO(std::int16_t, I16, FifoI16, 4)
READ_FIFO(std::int64_t, I64, FifoI64, 4)
READ_FIFO(std::uint32_t, U32, FifoU32, 1)
#undef READ_FIFO
std::vector<std::uint64_t> Session::readU64(const std::string& name, std::size_t limit, std::size_t alignment) {
    requireOpen(); const auto& r = resource(name);
    if (r.kind != Kind::FifoU64 || !alignment) throw std::invalid_argument("Wrong FIFO read: " + name);
    std::size_t available = 0;
    std::uint64_t dummy = 0;
    check(NiFpga_ReadFifoU64(session_, r.id, &dummy, 0, 0, &available), "Query " + name);
    std::size_t count = std::min(available, limit); count -= count % alignment;
    std::vector<std::uint64_t> data(count);
    if (count) check(NiFpga_ReadFifoU64(session_, r.id, data.data(), count, 0, &available), "Read " + name);
    return data;
}
}
