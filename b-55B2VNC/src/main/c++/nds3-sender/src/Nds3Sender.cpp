/**
 * @file Nds3Sender.cpp
 * @brief Simple NDS3 Driver with one read-only variable
 */

#include <nds3/nds.h>
#include <fstream>
#include <mutex>
#include <ctime>
#include "../include/Nds3Sender.h"

namespace {
std::mutex g_senderTsLogMutex;
const char* kSenderTsLogPath = "../../../../src/main/c++/nds3-sender/log/Nds3Sender_timestamps.log";
const char* kSenderTsFallbackLogPath = "Nds3Sender_timestamps.log";
std::ofstream g_senderTsLogFile;
bool g_senderTsUsingFallback = false;
std::once_flag g_senderTsLogInitOnce;

void ensureSenderTsLogOpened() {
    std::call_once(g_senderTsLogInitOnce, []() {
        g_senderTsLogFile.open(kSenderTsLogPath, std::ios::app);
        if (!g_senderTsLogFile.is_open()) {
            g_senderTsLogFile.clear();
            g_senderTsLogFile.open(kSenderTsFallbackLogPath, std::ios::app);
            g_senderTsUsingFallback = g_senderTsLogFile.is_open();
        }
    });
}

void logSenderTimestamp(const char* eventName, const timespec& ts) {
    ensureSenderTsLogOpened();
    if (!g_senderTsLogFile.is_open()) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_senderTsLogMutex);
    g_senderTsLogFile << eventName << "," << ts.tv_sec << "." << ts.tv_nsec << std::endl;
}
}

Nds3Sender::Nds3Sender(nds::Factory& factory,
                       const std::string& deviceName,
                       const nds::namedParameters_t& parameters)
{
    (void)parameters;

    nds::Port rootNode(deviceName);

    m_countRatePV = rootNode.addChild(nds::PVVariableIn<double>("CountRate"));
    rootNode.addChild(nds::PVDelegateOut<std::int32_t>(
        "Send",
        std::bind(&Nds3Sender::sendWriter, this,
                  std::placeholders::_1, std::placeholders::_2)
    ));

    rootNode.initialize(this, factory);

}

void Nds3Sender::sendWriter(const timespec& timestamp, const std::int32_t& value)
{
    if (value == 0) {
        return;
    }
    constexpr double kCountRate = 10.0;
    m_countRatePV.push(timestamp, kCountRate);
    m_countRatePV.setValue(kCountRate);
    logSenderTimestamp("CountRate_sent", timestamp);
}

NDS_DEFINE_DRIVER(Nds3Sender, Nds3Sender)
