#include "DiamondProtocol.h"

#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace diamond {
namespace {
template <typename T>
void append(std::vector<T>& pending, const T* words, std::size_t count) {
    if (count) {
        if (!words) throw std::invalid_argument("null FIFO data with nonzero count");
        pending.insert(pending.end(), words, words + count);
    }
}

template <typename T>
void consume(std::vector<T>& pending, std::size_t count) {
    pending.erase(pending.begin(), pending.begin() + count);
}

template <typename Signed, typename Unsigned>
Signed signedBits(Unsigned bits) {
    static_assert(sizeof(Signed) == sizeof(Unsigned), "wire value size mismatch");
    Signed value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool continuousGate(bool haveGate, uint32_t previous, uint32_t current) {
    return !haveGate || current == static_cast<uint32_t>(previous + uint32_t(1));
}
}

Rates computeRates(const RateRecord& r) {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    Rates result;
    result.measured = r.gateLength ? r.windows / (r.gateLength * TickSeconds) : nan;
    result.corrected = r.gateLength > r.busyTicks
        ? r.windows / ((r.gateLength - r.busyTicks) * TickSeconds) : nan;
    result.corrected2 = r.gateLength > r.aboveTicks
        ? r.crossings / ((r.gateLength - r.aboveTicks) * TickSeconds) : nan;
    result.deadFraction = r.gateLength
        ? static_cast<double>(r.busyTicks) / r.gateLength : nan;
    result.pileUpFraction = r.windows
        ? static_cast<double>(r.pileUpWindows) / r.windows : nan;
    return result;
}

std::vector<PulseRecord> PulseDecoder::feed(const int64_t* words, std::size_t count) {
    append(pending_, words, count);
    const std::size_t end = pending_.size() - pending_.size() % 4;
    std::vector<PulseRecord> records;
    records.reserve(end / 4);
    for (std::size_t p = 0; p < end; p += 4) {
        const uint64_t packed = static_cast<uint64_t>(pending_[p + 3]);
        PulseRecord r;
        r.startTime = static_cast<uint64_t>(pending_[p]);
        r.integral = pending_[p + 1];
        r.peak = pending_[p + 2];
        r.crossings = static_cast<uint32_t>(packed >> 32);
        r.length = static_cast<uint32_t>(packed);
        records.push_back(r);
    }
    consume(pending_, end);
    return records;
}

std::vector<RateRecord> RateDecoder::feed(const uint64_t* words, std::size_t count) {
    append(pending_, words, count);
    const std::size_t end = pending_.size() - pending_.size() % 4;
    std::vector<RateRecord> records;
    records.reserve(end / 4);
    for (std::size_t p = 0; p < end; p += 4) {
        RateRecord r;
        r.gateNumber = static_cast<uint32_t>(pending_[p]);
        r.windows = static_cast<uint32_t>(pending_[p + 1] >> 32);
        r.crossings = static_cast<uint32_t>(pending_[p + 1]);
        r.busyTicks = static_cast<uint32_t>(pending_[p + 2] >> 32);
        r.aboveTicks = static_cast<uint32_t>(pending_[p + 2]);
        r.gateLength = static_cast<uint32_t>(pending_[p + 3] >> 32);
        r.pileUpWindows = static_cast<uint32_t>(pending_[p + 3]);
        r.continuous = continuousGate(haveGate_, lastGate_, r.gateNumber);
        if (!r.continuous) ++gateGaps_;
        lastGate_ = r.gateNumber;
        haveGate_ = true;
        records.push_back(r);
    }
    consume(pending_, end);
    return records;
}

void RateDecoder::reset() {
    pending_.clear();
    haveGate_ = false;
    lastGate_ = 0;
    gateGaps_ = 0;
}

std::vector<SpectrumBlock> SpectrumDecoder::feed(const uint32_t* words, std::size_t count) {
    append(pending_, words, count);
    std::vector<SpectrumBlock> records;
    std::size_t p = 0;
    while (p < pending_.size()) {
        if (pending_[p] != SpectrumMagic) {
            ++p;
            ++badWords_;
            continue;
        }
        if (pending_.size() - p < SpectrumWords) break;
        SpectrumBlock r;
        r.gateNumber = pending_[p + 1];
        r.peakMin = signedBits<int32_t>(pending_[p + 2]);
        r.integralMin = signedBits<int32_t>(pending_[p + 3]);
        const uint32_t shifts = pending_[p + 4];
        r.peakShift = static_cast<uint8_t>(shifts);
        r.integralShift = static_cast<uint8_t>(shifts >> 8);
        r.peakOutOfRange = pending_[p + 5];
        r.integralOutOfRange = pending_[p + 6];
        r.acceptedWindows = pending_[p + 7];
        std::copy(pending_.begin() + p + 8,
                  pending_.begin() + p + 8 + PeakBins, r.peak.begin());
        std::copy(pending_.begin() + p + 8 + PeakBins,
                  pending_.begin() + p + SpectrumWords, r.integral.begin());
        uint64_t peakTotal = r.peakOutOfRange;
        uint64_t integralTotal = r.integralOutOfRange;
        for (std::size_t i = 0; i < PeakBins; ++i) peakTotal += r.peak[i];
        for (std::size_t i = 0; i < IntegralBins; ++i) integralTotal += r.integral[i];
        if ((shifts & 0xffff0000u) != 0 || r.peakShift > 31 || r.integralShift > 31 ||
            peakTotal != r.acceptedWindows || integralTotal != r.acceptedWindows) {
            ++badBlocks_;
            ++badWords_;
            ++p; // Search again: a lost word may have moved the next header inside.
            continue;
        }
        r.continuous = continuousGate(haveGate_, lastGate_, r.gateNumber);
        if (!r.continuous) ++gateGaps_;
        lastGate_ = r.gateNumber;
        haveGate_ = true;
        records.push_back(r);
        p += SpectrumWords;
    }
    consume(pending_, p);
    return records;
}

void SpectrumDecoder::reset() {
    pending_.clear();
    haveGate_ = false;
    lastGate_ = 0;
    badWords_ = badBlocks_ = gateGaps_ = 0;
}

ScopeDecoder::ScopeDecoder(int expectedChannel)
    : expectedChannel_(expectedChannel), badWords_(0) {
    if (expectedChannel < -1 || expectedChannel > 1)
        throw std::invalid_argument("scope channel must be -1, 0 or 1");
}

std::vector<ScopeRecord> ScopeDecoder::feed(const uint64_t* words, std::size_t count) {
    append(pending_, words, count);
    std::vector<ScopeRecord> records;
    std::size_t p = 0;
    while (p < pending_.size()) {
        const uint64_t header = pending_[p];
        const uint8_t channel = static_cast<uint8_t>(header >> 24);
        const uint8_t pre = static_cast<uint8_t>(header >> 16);
        const uint16_t rec = static_cast<uint16_t>(header);
        if ((header >> 32) != ScopeMagic || channel > 1 ||
            (expectedChannel_ >= 0 && channel != expectedChannel_) ||
            pre > MaxScopePreTicks || rec == 0 || rec > MaxScopeRecordTicks) {
            ++p;
            ++badWords_;
            continue;
        }
        const std::size_t length = 3 + static_cast<std::size_t>(rec);
        if (pending_.size() - p < length) break;
        ScopeRecord r;
        r.channel = channel;
        r.preTicks = pre;
        r.recordTicks = rec;
        r.timestamp = pending_[p + 1];
        r.number = static_cast<uint32_t>(pending_[p + 2] >> 32);
        r.skipped = static_cast<uint32_t>(pending_[p + 2]);
        r.samples.reserve(4 * static_cast<std::size_t>(rec));
        for (std::size_t i = 0; i < rec; ++i) {
            const uint64_t samples = pending_[p + 3 + i];
            for (unsigned s = 0; s < 4; ++s)
                r.samples.push_back(signedBits<int16_t>(static_cast<uint16_t>(samples >> (s * 16))));
        }
        records.push_back(r);
        p += length;
    }
    consume(pending_, p);
    return records;
}

std::vector<int16_t> RawDecoder::feed(const int16_t* samples, std::size_t count) {
    append(pending_, samples, count);
    const std::size_t end = pending_.size() - pending_.size() % 4;
    std::vector<int16_t> result(pending_.begin(), pending_.begin() + end);
    consume(pending_, end);
    return result;
}

} // namespace diamond
