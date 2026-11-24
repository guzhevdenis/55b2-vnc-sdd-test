#ifndef DIAMOND_SESSION_H
#define DIAMOND_SESSION_H
#include "DiamondResources.h"
#include "NiFpga_DiamondDetectorDAQ_FPGA_Top.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace diamond {
// Caller serializes access. Bounded copying reads need no FIFO release calls.
class Session {
public:
    Session();
    ~Session();
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;
    const std::vector<Resource>& resources() const { return resources_; }
    const Resource& resource(const std::string&) const;
    void open(const std::string& bitfile, const std::string& rio);
    void close() noexcept;
    bool isOpen() const { return open_; }
    void reset();
    void prepareFifos(std::size_t depth);
    void stopFifos() noexcept;
    void run();
    void write(const std::string&, std::int64_t);
    std::uint64_t read(const std::string&);
    std::vector<std::int32_t> deviceConfig();
    NiFpga_Status status() const { return status_; }
    void hostFailure() { if (status_ >= 0) status_ = -1; }
    std::vector<std::int16_t> readI16(const std::string&, std::size_t);
    std::vector<std::int64_t> readI64(const std::string&, std::size_t);
    std::vector<std::uint32_t> readU32(const std::string&, std::size_t);
    std::vector<std::uint64_t> readU64(const std::string&, std::size_t, std::size_t alignment = 1);
private:
    NiFpga_Session session_;
    NiFpga_Status status_;
    bool open_, runtime_;
    std::string rio_;
    std::vector<Resource> resources_;
    std::vector<std::uint32_t> started_;
    void check(NiFpga_Status, const std::string&);
    void requireOpen() const;
};
}
#endif
