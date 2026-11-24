#ifndef DIAMOND_PROTOCOL_H
#define DIAMOND_PROTOCOL_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

// Wire formats from DiamondDetectorDAQ_repaired, sections 4.11 and 6.
// No NI, NDS or EPICS dependencies: these decoders can be tested off hardware.
namespace diamond {

static const double TickSeconds = 5e-9;
static const double SampleNanoseconds = 1.25;
static const std::size_t PeakBins = 512;
static const std::size_t IntegralBins = 1024;
static const std::size_t SpectrumWords = 8 + PeakBins + IntegralBins;
static const uint32_t SpectrumMagic = 0x53504543u;
static const uint32_t ScopeMagic = 0x53434f50u;
static const uint16_t MaxScopeRecordTicks = 65532;
static const uint8_t MaxScopePreTicks = 248;

struct PulseRecord {
    uint64_t startTime; // FPGA ticks, not a host/EPICS absolute timestamp.
    int64_t integral;   // Sign-extended I32, in ADC codes summed over samples.
    int64_t peak;       // Sign-extended I32, in ADC codes.
    uint32_t crossings;
    uint32_t length;    // FPGA ticks.
};

struct RateRecord {
    uint32_t gateNumber;
    uint32_t windows;
    uint32_t crossings;
    uint32_t busyTicks;
    uint32_t aboveTicks;
    uint32_t gateLength;
    uint32_t pileUpWindows;
    bool continuous; // First record is continuous; wrap UINT32_MAX -> 0 is valid.
};

struct Rates {
    double measured;
    double corrected;
    double corrected2;
    double deadFraction;
    double pileUpFraction;
};

Rates computeRates(const RateRecord& record);

struct SpectrumBlock {
    uint32_t gateNumber;
    int32_t peakMin;
    int32_t integralMin;
    uint8_t peakShift;
    uint8_t integralShift;
    uint32_t peakOutOfRange;
    uint32_t integralOutOfRange;
    uint32_t acceptedWindows;
    std::array<uint32_t, PeakBins> peak;
    std::array<uint32_t, IntegralBins> integral;
    bool continuous;
};

struct ScopeRecord {
    uint8_t channel;
    uint8_t preTicks;
    uint16_t recordTicks;
    uint64_t timestamp; // Same FPGA tick domain as PulseRecord::startTime.
    uint32_t number;
    uint32_t skipped; // Triggers skipped by holdoff/busy, not DMA lost words.
    std::vector<int16_t> samples;
};

// Pulse/rate/raw writes are atomic groups of four FIFO elements. There is no
// magic/checksum in these formats; arbitrary word loss cannot be resynchronized.
// Reset on a new FPGA run/FIFO restart rather than joining separate sessions.
class PulseDecoder {
public:
    std::vector<PulseRecord> feed(const int64_t* words, std::size_t count);
    std::vector<PulseRecord> feed(const std::vector<int64_t>& words) {
        return feed(words.data(), words.size());
    }
    void reset() { pending_.clear(); }
    std::size_t pendingWords() const { return pending_.size(); }
private:
    std::vector<int64_t> pending_;
};

class RateDecoder {
public:
    RateDecoder() : haveGate_(false), lastGate_(0), gateGaps_(0) {}
    std::vector<RateRecord> feed(const uint64_t* words, std::size_t count);
    std::vector<RateRecord> feed(const std::vector<uint64_t>& words) {
        return feed(words.data(), words.size());
    }
    void reset();
    std::size_t pendingWords() const { return pending_.size(); }
    uint64_t gateGaps() const { return gateGaps_; }
private:
    std::vector<uint64_t> pending_;
    bool haveGate_;
    uint32_t lastGate_;
    uint64_t gateGaps_;
};

// Invalid headers or sum(bins)+out-of-range != acceptedWindows are rejected.
// Both FPGA spectrum-bank collisions and DMA corruption can break this invariant;
// badBlocks counts rejected candidate blocks, not a count of lost events.
class SpectrumDecoder {
public:
    SpectrumDecoder() : haveGate_(false), lastGate_(0), badWords_(0),
                        badBlocks_(0), gateGaps_(0) {}
    std::vector<SpectrumBlock> feed(const uint32_t* words, std::size_t count);
    std::vector<SpectrumBlock> feed(const std::vector<uint32_t>& words) {
        return feed(words.data(), words.size());
    }
    void reset();
    std::size_t pendingWords() const { return pending_.size(); }
    uint64_t badWords() const { return badWords_; }
    uint64_t badBlocks() const { return badBlocks_; }
    uint64_t gateGaps() const { return gateGaps_; }
private:
    std::vector<uint32_t> pending_;
    bool haveGate_;
    uint32_t lastGate_;
    uint64_t badWords_;
    uint64_t badBlocks_;
    uint64_t gateGaps_;
};

// Keeps incomplete records between reads; maximum retained tail is 65534 words.
// The firmware supports 65532 ticks (the LabVIEW host's 4096 limit is narrower).
// No checksum exists: payload word loss is not always detectable in this format.
class ScopeDecoder {
public:
    explicit ScopeDecoder(int expectedChannel = -1);
    std::vector<ScopeRecord> feed(const uint64_t* words, std::size_t count);
    std::vector<ScopeRecord> feed(const std::vector<uint64_t>& words) {
        return feed(words.data(), words.size());
    }
    void reset() { pending_.clear(); badWords_ = 0; }
    std::size_t pendingWords() const { return pending_.size(); }
    uint64_t badWords() const { return badWords_; }
private:
    int expectedChannel_;
    std::vector<uint64_t> pending_;
    uint64_t badWords_;
};

class RawDecoder {
public:
    std::vector<int16_t> feed(const int16_t* samples, std::size_t count);
    std::vector<int16_t> feed(const std::vector<int16_t>& samples) {
        return feed(samples.data(), samples.size());
    }
    void reset() { pending_.clear(); }
    std::size_t pendingWords() const { return pending_.size(); }
private:
    std::vector<int16_t> pending_;
};

} // namespace diamond
#endif
