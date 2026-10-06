#include <nds3/nds.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <mutex>
#include <ctime>
#include "DataGenerator.h"

// This class declares our device in the contructor and supplies the functionalities to support it
#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

namespace {
std::mutex g_timestampLogMutex;
const char* kTimestampLogPath = "../../../../src/main/c++/DataGenerator/log/DataGenerator_timestamps.log";
const char* kFallbackLogPath = "DataGenerator_timestamps.log";
std::ofstream g_timestampLogFile;
bool g_logFileInitialized = false;
bool g_usingFallbackLogPath = false;

void ensureLogFileOpened() {
    if (g_logFileInitialized) {
        return;
    }

    g_timestampLogFile.open(kTimestampLogPath, std::ios::app);
    if (!g_timestampLogFile.is_open()) {
        g_timestampLogFile.clear();
        g_timestampLogFile.open(kFallbackLogPath, std::ios::app);
        if (g_timestampLogFile.is_open()) {
            g_usingFallbackLogPath = true;
            std::cerr << "DataGenerator: primary log path unavailable, using current directory: "
                      << kFallbackLogPath << std::endl;
        }
    }
    g_logFileInitialized = true;
}

void logTimestamp(const char* pvName, timespec* pTimestamp) {
    ensureLogFileOpened();
    if (!g_timestampLogFile.is_open()) {
        static bool s_reported = false;
        if (!s_reported) {
            std::cerr << "DataGenerator: cannot open timestamp log file: " << kTimestampLogPath;
            if (g_usingFallbackLogPath) {
                std::cerr << " or fallback: " << kFallbackLogPath;
            }
            std::cerr << std::endl;
            s_reported = true;
        }
        return;
    }
    if (pTimestamp == nullptr) {
        return;
    }
    clock_gettime(CLOCK_REALTIME, pTimestamp);

    std::lock_guard<std::mutex> lock(g_timestampLogMutex);
    g_timestampLogFile << pvName << "," << pTimestamp->tv_sec << "." << pTimestamp->tv_nsec << std::endl;
}
}

// Constructor. Here we setup the device structure
DataGenerator::DataGenerator(nds::Factory &factory,
                             const std::string &deviceName,
                             const nds::namedParameters_t &parameters)
{
    (void)parameters;
    m_generatedOnce = false;
    m_generatedValues.fill(0.0f);
    m_pushedOnce.fill(false);
    nds::Port rootNode(deviceName);

    // --- Mode PV: 0 = Neutron Power, 1 = Neutron Profile ---
    m_mode = Mode::NeutronProfile; // default

    rootNode.addChild(
        nds::PVDelegateOut<std::int32_t>(
            "Mode",
            std::bind(&DataGenerator::Mode_Writer,
                      this,
                      std::placeholders::_1,
                      std::placeholders::_2)));
    rootNode.addChild(
        nds::PVDelegateOut<std::int32_t>(
            "Publish",
            std::bind(&DataGenerator::Publish_Writer,
                      this,
                      std::placeholders::_1,
                      std::placeholders::_2)));

    PV00 = rootNode.addChild(nds::PVVariableIn<float>("Temperature00"));
    PV01 = rootNode.addChild(nds::PVVariableIn<float>("Temperature01"));
    PV02 = rootNode.addChild(nds::PVVariableIn<float>("Temperature02"));
    PV03 = rootNode.addChild(nds::PVVariableIn<float>("Temperature03"));
    PV04 = rootNode.addChild(nds::PVVariableIn<float>("Temperature04"));
    PV05 = rootNode.addChild(nds::PVVariableIn<float>("Temperature05"));
    PV06 = rootNode.addChild(nds::PVVariableIn<float>("Temperature06"));
    PV07 = rootNode.addChild(nds::PVVariableIn<float>("Temperature07"));
    PV08 = rootNode.addChild(nds::PVVariableIn<float>("Temperature08"));
    PV09 = rootNode.addChild(nds::PVVariableIn<float>("Temperature09"));
    PV10 = rootNode.addChild(nds::PVVariableIn<float>("Temperature10"));
    PV11 = rootNode.addChild(nds::PVVariableIn<float>("Temperature11"));
    PV12 = rootNode.addChild(nds::PVVariableIn<float>("Temperature12"));
    PV13 = rootNode.addChild(nds::PVVariableIn<float>("Temperature13"));
    PV14 = rootNode.addChild(nds::PVVariableIn<float>("Temperature14"));
    PV15 = rootNode.addChild(nds::PVVariableIn<float>("Temperature15"));
    PV16 = rootNode.addChild(nds::PVVariableIn<float>("Temperature16"));
    PV17 = rootNode.addChild(nds::PVVariableIn<float>("Temperature17"));
    PV18 = rootNode.addChild(nds::PVVariableIn<float>("Temperature18"));
    PV19 = rootNode.addChild(nds::PVVariableIn<float>("Temperature19"));
    PV20 = rootNode.addChild(nds::PVVariableIn<float>("Temperature20"));
    PV21 = rootNode.addChild(nds::PVVariableIn<float>("Temperature21"));
    PV22 = rootNode.addChild(nds::PVVariableIn<float>("Temperature22"));
    PV23 = rootNode.addChild(nds::PVVariableIn<float>("Temperature23"));
    rootNode.initialize(this, factory);
}

// Data generator with selectable mode (Neutron Power / Neutron Profile)
void DataGenerator::Mode_Writer(const timespec& /*timestamp*/,
   const std::int32_t& value)
{
      if (value == 0)
      m_mode = Mode::NeutronPower;
      else
      m_mode = Mode::NeutronProfile;
}

void DataGenerator::Publish_Writer(const timespec& /*timestamp*/,
   const std::int32_t& value)
{
   if (value != 0) {
      publishAllTemperatures();
   }
}

void DataGenerator::generateOnce() {
   if (m_generatedOnce) {
      return;
   }
   const std::array<float, 24> neutronPower = {
      10.31028511f, 10.02734101f, 9.37763289f, 8.53562233f, 7.64873979f, 7.11232744f,
      6.90274274f, 6.48310735f, 6.68835471f, 6.85435450f, 6.77764659f, 5.47646355f,
      7.16472038f, 6.95844839f, 6.39191711f, 5.42053500f, 4.67282883f, 3.78418963f,
      2.83321334f, 3.04452244f, 3.58351894f, 3.46573590f, 4.00733319f, 2.19722458f
   };
   const std::array<float, 24> neutronProfile = {
      0.8074f, 0.7828f, 0.7346f, 0.6846f, 0.6365f, 0.6104f,
      0.6229f, 0.6086f, 0.6173f, 0.6242f, 0.6167f, 0.5394f,
      0.6378f, 0.6135f, 0.5674f, 0.5120f, 0.4612f, 0.4221f,
      0.4182f, 0.4242f, 0.4451f, 0.4476f, 0.4439f, 0.3756f
   };
   m_generatedValues = (m_mode == Mode::NeutronPower) ? neutronPower : neutronProfile;
   m_generatedOnce = true;
}

void DataGenerator::publishOnce(std::size_t index,
                                const char* pvName,
                                nds::PVVariableIn<float>& pv,
                                const timespec& timestamp,
                                float value) {
   if (!m_pushedOnce[index]) {
      timespec logTs = timestamp;
      logTimestamp(pvName, &logTs);
      pv.push(timestamp, value);
      m_pushedOnce[index] = true;
   }
}

void DataGenerator::publishAllTemperatures() {
   generateOnce();
   timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   publishOnce(0,  "Temperature00", PV00, ts, m_generatedValues[0]);
   publishOnce(1,  "Temperature01", PV01, ts, m_generatedValues[1]);
   publishOnce(2,  "Temperature02", PV02, ts, m_generatedValues[2]);
   publishOnce(3,  "Temperature03", PV03, ts, m_generatedValues[3]);
   publishOnce(4,  "Temperature04", PV04, ts, m_generatedValues[4]);
   publishOnce(5,  "Temperature05", PV05, ts, m_generatedValues[5]);
   publishOnce(6,  "Temperature06", PV06, ts, m_generatedValues[6]);
   publishOnce(7,  "Temperature07", PV07, ts, m_generatedValues[7]);
   publishOnce(8,  "Temperature08", PV08, ts, m_generatedValues[8]);
   publishOnce(9,  "Temperature09", PV09, ts, m_generatedValues[9]);
   publishOnce(10, "Temperature10", PV10, ts, m_generatedValues[10]);
   publishOnce(11, "Temperature11", PV11, ts, m_generatedValues[11]);
   publishOnce(12, "Temperature12", PV12, ts, m_generatedValues[12]);
   publishOnce(13, "Temperature13", PV13, ts, m_generatedValues[13]);
   publishOnce(14, "Temperature14", PV14, ts, m_generatedValues[14]);
   publishOnce(15, "Temperature15", PV15, ts, m_generatedValues[15]);
   publishOnce(16, "Temperature16", PV16, ts, m_generatedValues[16]);
   publishOnce(17, "Temperature17", PV17, ts, m_generatedValues[17]);
   publishOnce(18, "Temperature18", PV18, ts, m_generatedValues[18]);
   publishOnce(19, "Temperature19", PV19, ts, m_generatedValues[19]);
   publishOnce(20, "Temperature20", PV20, ts, m_generatedValues[20]);
   publishOnce(21, "Temperature21", PV21, ts, m_generatedValues[21]);
   publishOnce(22, "Temperature22", PV22, ts, m_generatedValues[22]);
   publishOnce(23, "Temperature23", PV23, ts, m_generatedValues[23]);
}

NDS_DEFINE_DRIVER(DataGenerator, DataGenerator)