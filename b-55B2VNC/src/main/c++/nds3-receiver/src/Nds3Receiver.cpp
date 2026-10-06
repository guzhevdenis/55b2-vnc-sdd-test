/**
 * @file Nds3Receiver.cpp
 * @brief Simple NDS3 Driver that receives CountRate value
 */

#include <nds3/nds.h>
#include <fstream>
#include <mutex>
#include "../include/Nds3Receiver.h"

namespace {
std::mutex g_receiverTsLogMutex;
const char* kReceiverTsLogPath = "../../../../src/main/c++/nds3-receiver/log/Nds3Receiver_timestamps.log";
const char* kReceiverTsFallbackLogPath = "Nds3Receiver_timestamps.log";
std::ofstream g_receiverTsLogFile;
bool g_receiverTsUsingFallback = false;
std::once_flag g_receiverTsLogInitOnce;

void ensureReceiverTsLogOpened() {
    std::call_once(g_receiverTsLogInitOnce, []() {
        g_receiverTsLogFile.open(kReceiverTsLogPath, std::ios::app);
        if (!g_receiverTsLogFile.is_open()) {
            g_receiverTsLogFile.clear();
            g_receiverTsLogFile.open(kReceiverTsFallbackLogPath, std::ios::app);
            g_receiverTsUsingFallback = g_receiverTsLogFile.is_open();
        }
    });
}

void logReceiverTimestamp(const char* eventName, const timespec& ts) {
    ensureReceiverTsLogOpened();
    if (!g_receiverTsLogFile.is_open()) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_receiverTsLogMutex);
    g_receiverTsLogFile << eventName << "," << ts.tv_sec << "." << ts.tv_nsec << std::endl;
}
}

Nds3Receiver::Nds3Receiver(nds::Factory& factory,
                           const std::string& deviceName,
                           const nds::namedParameters_t& parameters)

{
    (void)parameters;

    nds::Port rootNode(deviceName);

    rootNode.addChild(nds::PVDelegateOut<double>(
        "CountRate-received",
        std::bind(&Nds3Receiver::writeCountRate, this,
                  std::placeholders::_1, std::placeholders::_2)
    ));

    rootNode.initialize(this, factory);
}

void Nds3Receiver::writeCountRate(const timespec& timestamp, const double& value)
{
    (void)value;
    logReceiverTimestamp("CountRate_received", timestamp);
}

NDS_DEFINE_DRIVER(Nds3Receiver, Nds3Receiver)
