#include "DiamondProtocol.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

// Standalone C++11 executable; failures remain active with -DNDEBUG.
#define CHECK(condition) do { if (!(condition)) throw std::runtime_error( \
    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " #condition); } while (false)

namespace {
bool near(double a, double b) {
    return std::fabs(a - b) <= 1e-10 * std::max(1.0, std::fabs(b));
}

void pulseTests() {
    // Appendix A, DecodePulseRecords.vi, including every possible split point.
    const std::vector<int64_t> words = {10, 100, 5, INT64_C(0x100000002),
                                       20, 200, 7, INT64_C(0x200000003)};
    for (std::size_t split = 0; split <= words.size(); ++split) {
        diamond::PulseDecoder decoder;
        std::vector<diamond::PulseRecord> records = decoder.feed(words.data(), split);
        const auto tail = decoder.feed(words.data() + split, words.size() - split);
        records.insert(records.end(), tail.begin(), tail.end());
        CHECK(records.size() == 2);
        CHECK(records[0].startTime == 10 && records[1].startTime == 20);
        CHECK(records[0].integral == 100 && records[1].integral == 200);
        CHECK(records[0].peak == 5 && records[1].peak == 7);
        CHECK(records[0].crossings == 1 && records[1].crossings == 2);
        CHECK(records[0].length == 2 && records[1].length == 3);
        CHECK(decoder.pendingWords() == 0);
    }
    diamond::PulseDecoder decoder;
    const auto signedRecord = decoder.feed({-1, -2147483648LL, -42, -1});
    CHECK(signedRecord[0].startTime == UINT64_MAX);
    CHECK(signedRecord[0].integral == INT32_MIN && signedRecord[0].peak == -42);
    CHECK(signedRecord[0].crossings == UINT32_MAX && signedRecord[0].length == UINT32_MAX);
    CHECK(decoder.feed({1, 2, 3}).empty());
    CHECK(decoder.pendingWords() == 3);
    decoder.reset();
    CHECK(decoder.pendingWords() == 0);
    CHECK(decoder.feed(nullptr, 0).empty());
}

void rateTests() {
    // Appendix A, DecodeRateRecords.vi.
    const std::vector<uint64_t> words = {7, UINT64_C(0x100000002), UINT64_C(0x300000004),
        UINT64_C(0x600000009), 8, UINT64_C(0x500000006), UINT64_C(0x700000008),
        UINT64_C(0x30d4000000005)};
    for (std::size_t split = 0; split <= words.size(); ++split) {
        diamond::RateDecoder decoder;
        auto records = decoder.feed(words.data(), split);
        auto tail = decoder.feed(words.data() + split, words.size() - split);
        records.insert(records.end(), tail.begin(), tail.end());
        CHECK(records.size() == 2);
        CHECK(records[0].gateNumber == 7 && records[1].gateNumber == 8);
        CHECK(records[0].windows == 1 && records[1].windows == 5);
        CHECK(records[0].crossings == 2 && records[1].crossings == 6);
        CHECK(records[0].busyTicks == 3 && records[1].busyTicks == 7);
        CHECK(records[0].aboveTicks == 4 && records[1].aboveTicks == 8);
        CHECK(records[0].gateLength == 6 && records[1].gateLength == 200000);
        CHECK(records[0].pileUpWindows == 9 && records[1].pileUpWindows == 5);
        CHECK(records[0].continuous && records[1].continuous && decoder.gateGaps() == 0);
    }
    // Appendix A, ComputeRates.vi, including saturation and empty gate behavior.
    diamond::RateRecord r = {0, 3680, 4000, 126000, 80000, 200000, 368, true};
    auto rates = diamond::computeRates(r);
    CHECK(near(rates.measured, 3680000));
    CHECK(near(rates.corrected, 9945945.945945945));
    CHECK(near(rates.corrected2, 6666666.666666667));
    CHECK(near(rates.deadFraction, .63) && near(rates.pileUpFraction, .1));
    r = {1, 1, 1, 6, 6, 6, 0, true};
    rates = diamond::computeRates(r);
    CHECK(near(rates.measured, 33333333.333333333));
    CHECK(std::isnan(rates.corrected) && std::isnan(rates.corrected2));
    CHECK(rates.deadFraction == 1 && rates.pileUpFraction == 0);
    r = {2, 0, 0, 0, 0, 200000, 0, true};
    rates = diamond::computeRates(r);
    CHECK(rates.measured == 0 && rates.corrected == 0 && rates.corrected2 == 0);
    CHECK(rates.deadFraction == 0 && std::isnan(rates.pileUpFraction));
    r = {3, 1, 1, 10, 11, 6, 0, true};
    rates = diamond::computeRates(r);
    CHECK(std::isnan(rates.corrected) && std::isnan(rates.corrected2));
    r.gateLength = 0;
    rates = diamond::computeRates(r);
    CHECK(std::isnan(rates.measured) && std::isnan(rates.deadFraction));

    diamond::RateDecoder decoder;
    CHECK(decoder.feed({UINT32_MAX, 0, 0, 0})[0].continuous);
    CHECK(decoder.feed({0, 0, 0, 0})[0].continuous);
    CHECK(!decoder.feed({2, 0, 0, 0})[0].continuous);
    CHECK(decoder.gateGaps() == 1);
    decoder.feed({1});
    decoder.reset();
    CHECK(decoder.pendingWords() == 0 && decoder.gateGaps() == 0);
    CHECK(decoder.feed({90, 0, 0, 0})[0].continuous);
}

std::vector<uint32_t> spectrum(uint32_t gate) {
    std::vector<uint32_t> words(diamond::SpectrumWords, 0);
    words[0] = diamond::SpectrumMagic;
    words[1] = gate;
    words[2] = static_cast<uint32_t>(-16);
    words[3] = static_cast<uint32_t>(-256);
    words[4] = 6 | (13 << 8);
    words[5] = 1;
    words[6] = 2;
    words[7] = 5;
    words[8] = 1;
    words[8 + 511] = 3;
    words[8 + 512 + 1023] = 3;
    return words;
}

void spectrumTests() {
    const auto words = spectrum(25);
    // Every header/payload boundary, including split inside the last bin.
    for (std::size_t split = 0; split <= words.size(); ++split) {
        diamond::SpectrumDecoder decoder;
        auto records = decoder.feed(words.data(), split);
        const auto tail = decoder.feed(words.data() + split, words.size() - split);
        records.insert(records.end(), tail.begin(), tail.end());
        CHECK(records.size() == 1 && records[0].gateNumber == 25);
        CHECK(records[0].peakMin == -16 && records[0].integralMin == -256);
        CHECK(records[0].peakShift == 6 && records[0].integralShift == 13);
        CHECK(records[0].peak[0] == 1 && records[0].peak[511] == 3);
        CHECK(records[0].integral[1023] == 3 && records[0].acceptedWindows == 5);
        CHECK(records[0].peakOutOfRange == 1 && records[0].integralOutOfRange == 2);
        CHECK(decoder.badBlocks() == 0 && decoder.badWords() == 0 && decoder.pendingWords() == 0);
    }
    diamond::SpectrumDecoder decoder;
    CHECK(decoder.feed(spectrum(UINT32_MAX))[0].continuous);
    CHECK(decoder.feed(spectrum(0))[0].continuous);
    CHECK(!decoder.feed(spectrum(2))[0].continuous);
    CHECK(decoder.gateGaps() == 1);

    decoder.reset();
    auto damaged = spectrum(3);
    damaged.erase(damaged.begin() + 8); // One DMA word missing: next block is misaligned.
    const auto good = spectrum(4);
    damaged.insert(damaged.end(), good.begin(), good.end());
    const auto recovered = decoder.feed(damaged);
    CHECK(recovered.size() == 1 && recovered[0].gateNumber == 4);
    CHECK(decoder.badBlocks() == 1 && decoder.badWords() == diamond::SpectrumWords - 1);
    CHECK(decoder.pendingWords() == 0);

    decoder.reset();
    damaged = spectrum(5);
    damaged[8] += 1; // Also detects documented FPGA same-bin update conflicts.
    CHECK(decoder.feed(damaged).empty() && decoder.badBlocks() == 1);
    CHECK(decoder.feed(spectrum(6)).size() == 1);
    decoder.reset();
    damaged = spectrum(7);
    damaged[4] |= 0x10000u; // Header has reserved bits set.
    CHECK(decoder.feed(damaged).empty() && decoder.badBlocks() == 1);

    decoder.reset();
    damaged = spectrum(8);
    damaged[4] = 32; // No legal I32 histogram shift can be larger than 31.
    CHECK(decoder.feed(damaged).empty() && decoder.badBlocks() == 1);

    decoder.reset();
    damaged.assign(diamond::SpectrumWords, 0);
    damaged[0] = diamond::SpectrumMagic;
    damaged[8] = UINT32_MAX;
    damaged[9] = 1; // A 32-bit summation would falsely equal AcceptedWindows == 0.
    CHECK(decoder.feed(damaged).empty() && decoder.badBlocks() == 1);

    decoder.reset();
    auto embeddedMagic = std::vector<uint32_t>(diamond::SpectrumWords, 0);
    embeddedMagic[0] = embeddedMagic[7] = embeddedMagic[8] =
        embeddedMagic[8 + diamond::PeakBins] = diamond::SpectrumMagic;
    CHECK(decoder.feed(embeddedMagic).size() == 1); // Magic is legal bin content.
    CHECK(decoder.badWords() == 0);
    decoder.feed({diamond::SpectrumMagic});
    CHECK(decoder.pendingWords() == 1);
    decoder.reset();
    CHECK(decoder.pendingWords() == 0 && decoder.badBlocks() == 0 && decoder.gateGaps() == 0);
}

void scopeTests() {
    // Appendix A, TestDecodeScopeRecords.vi: two records split across calls.
    diamond::ScopeDecoder decoder;
    auto records = decoder.feed({0x7b, UINT64_C(0x53434f5001020002), 0x10, 0,
        UINT64_C(0x0004000300020001), UINT64_C(0xfffffffe80007fff),
        UINT64_C(0x53434f5000030001), 0x24});
    CHECK(records.size() == 1 && decoder.badWords() == 1 && decoder.pendingWords() == 2);
    CHECK(records[0].channel == 1 && records[0].preTicks == 2 && records[0].recordTicks == 2);
    CHECK(records[0].timestamp == 16 && records[0].number == 0 && records[0].skipped == 0);
    CHECK(records[0].samples == std::vector<int16_t>({1, 2, 3, 4, 32767, -32768, -2, -1}));
    records = decoder.feed({UINT64_C(0x100000001), UINT64_C(0x0008000700060005)});
    CHECK(records.size() == 1 && decoder.pendingWords() == 0);
    CHECK(records[0].channel == 0 && records[0].preTicks == 3 && records[0].recordTicks == 1);
    CHECK(records[0].timestamp == 36 && records[0].number == 1 && records[0].skipped == 1);
    CHECK(records[0].samples == std::vector<int16_t>({5, 6, 7, 8}));
    CHECK(decoder.feed(nullptr, 0).empty());

    const std::vector<uint64_t> words = {UINT64_C(0x53434f5001020002), 0x10, 0,
        UINT64_C(0x0004000300020001), UINT64_C(0xfffffffe80007fff)};
    for (std::size_t split = 0; split <= words.size(); ++split) {
        diamond::ScopeDecoder d(1);
        auto output = d.feed(words.data(), split);
        const auto tail = d.feed(words.data() + split, words.size() - split);
        output.insert(output.end(), tail.begin(), tail.end());
        CHECK(output.size() == 1 && output[0].samples.size() == 8);
        CHECK(d.pendingWords() == 0 && d.badWords() == 0);
    }
    diamond::ScopeDecoder ch0(0);
    CHECK(ch0.feed(words).empty() && ch0.badWords() == words.size());
    decoder.reset();
    CHECK(decoder.feed({UINT64_C(0x53434f5002000001), // Invalid channel.
        UINT64_C(0x53434f5000f90001), // Pre > 248.
        UINT64_C(0x53434f5000000000), // Zero record length.
        UINT64_C(0x53434f500000fffd)}).empty()); // Length > 65532.
    CHECK(decoder.badWords() == 4 && decoder.pendingWords() == 0);
    CHECK(decoder.feed(words).size() == 1);

    // Accept firmware maximum, including fragments; not limited to LabVIEW's 4096.
    decoder.reset();
    std::vector<uint64_t> maximum(3 + diamond::MaxScopeRecordTicks, 0);
    maximum[0] = UINT64_C(0x53434f5000f8fffc);
    CHECK(decoder.feed(maximum.data(), maximum.size() - 1).empty());
    CHECK(decoder.pendingWords() == maximum.size() - 1);
    records = decoder.feed(maximum.data() + maximum.size() - 1, 1);
    CHECK(records.size() == 1 && records[0].samples.size() == 4u * diamond::MaxScopeRecordTicks);
    CHECK(decoder.pendingWords() == 0);
    decoder.feed({words[0]});
    decoder.reset();
    CHECK(decoder.pendingWords() == 0 && decoder.badWords() == 0);
}

void rawTests() {
    diamond::RawDecoder decoder;
    CHECK(decoder.feed({-32768, -1, 0}).empty() && decoder.pendingWords() == 3);
    CHECK(decoder.feed({32767, 1, 2}) == std::vector<int16_t>({-32768, -1, 0, 32767}));
    CHECK(decoder.pendingWords() == 2);
    CHECK(decoder.feed({3, 4}) == std::vector<int16_t>({1, 2, 3, 4}));
    CHECK(decoder.pendingWords() == 0 && decoder.feed(nullptr, 0).empty());
    decoder.feed({1});
    decoder.reset();
    CHECK(decoder.pendingWords() == 0);
}
}

int main() {
    try {
        pulseTests();
        rateTests();
        spectrumTests();
        scopeTests();
        rawTests();
        std::cout << "DiamondProtocol: all tests passed\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
