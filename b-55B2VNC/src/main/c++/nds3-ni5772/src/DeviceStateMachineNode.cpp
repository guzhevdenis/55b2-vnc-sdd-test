//State machine functions

#include "./../include/Ni5772Digitizer.h"

void Ni5772Digitizer::switchOn_Device() {}

void Ni5772Digitizer::switchOff_Device() {}

void Ni5772Digitizer::start_Device() {}

void Ni5772Digitizer::stop_Device() {}

void Ni5772Digitizer::recover_Device() {}

bool Ni5772Digitizer::allow_Device_Change(const nds::state_t,
  const nds::state_t,
    const nds::state_t) {
  return true;
}