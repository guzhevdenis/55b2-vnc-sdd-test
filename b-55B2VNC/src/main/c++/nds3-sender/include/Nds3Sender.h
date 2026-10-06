/**
 * @file Nds3Sender.h
 * @brief Simple NDS3 Driver with one read-only variable
 */

#ifndef NDS3_SENDER_H
#define NDS3_SENDER_H

#include <nds3/nds.h>
#include <cstdint>

class Nds3Sender
{
public:
    Nds3Sender(nds::Factory& factory,
               const std::string& deviceName,
               const nds::namedParameters_t& parameters);

private:
    void sendWriter(const timespec& timestamp, const std::int32_t& value);

    nds::PVVariableIn<double> m_countRatePV;
};

#endif // NDS3_SENDER_H
