#include <nds3/nds.h>
#include <cstdint>
#include <array>

class DataGenerator
{
public:
    DataGenerator(nds::Factory &factory,
                  const std::string &deviceName,
                  const nds::namedParameters_t &parameters);

private:
    // 0 = Neutron Power, 1 = Neutron Profile
    enum class Mode : std::int16_t
    {
        NeutronPower   = 0,
        NeutronProfile = 1
    };

    Mode m_mode;
    bool m_generatedOnce;
    std::array<float, 24> m_generatedValues;
    std::array<bool, 24> m_pushedOnce;

    void Mode_Writer(const timespec& timestamp, const std::int32_t& value);
    void Publish_Writer(const timespec& timestamp, const std::int32_t& value);
    void generateOnce();
    void publishAllTemperatures();
    void publishOnce(std::size_t index, const char* pvName, nds::PVVariableIn<float>& pv, const timespec& timestamp, float value);

    nds::PVVariableIn<float> PV00;
    nds::PVVariableIn<float> PV01;
    nds::PVVariableIn<float> PV02;
    nds::PVVariableIn<float> PV03;
    nds::PVVariableIn<float> PV04;
    nds::PVVariableIn<float> PV05;
    nds::PVVariableIn<float> PV06;
    nds::PVVariableIn<float> PV07;
    nds::PVVariableIn<float> PV08;
    nds::PVVariableIn<float> PV09;
    nds::PVVariableIn<float> PV10;
    nds::PVVariableIn<float> PV11;
    nds::PVVariableIn<float> PV12;
    nds::PVVariableIn<float> PV13;
    nds::PVVariableIn<float> PV14;
    nds::PVVariableIn<float> PV15;
    nds::PVVariableIn<float> PV16;
    nds::PVVariableIn<float> PV17;
    nds::PVVariableIn<float> PV18;
    nds::PVVariableIn<float> PV19;
    nds::PVVariableIn<float> PV20;
    nds::PVVariableIn<float> PV21;
    nds::PVVariableIn<float> PV22;
    nds::PVVariableIn<float> PV23;
};