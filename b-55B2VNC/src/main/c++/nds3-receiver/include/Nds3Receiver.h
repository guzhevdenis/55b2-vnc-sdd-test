/**
 * @file Nds3Receiver.h
 * @brief Simple NDS3 Driver that receives CountRate value
 */

#ifndef NDS3_RECEIVER_H
#define NDS3_RECEIVER_H

#include <nds3/nds.h>

class Nds3Receiver
{
public:
    Nds3Receiver(nds::Factory& factory,
                 const std::string& deviceName,
                 const nds::namedParameters_t& parameters);

private:


    // PV write handler (receives data)
    void writeCountRate(const timespec& timestamp, const double& value);
};

#endif // NDS3_RECEIVER_H
