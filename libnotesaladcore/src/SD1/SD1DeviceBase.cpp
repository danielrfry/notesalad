#include "SD1DeviceBase.h"

SD1DeviceBase::~SD1DeviceBase()
{
}

void SD1DeviceBase::writeReg(uint8_t addr, uint8_t data)
{
    uint8_t buffer[2] = { addr, data };
    this->write(&buffer[0], 2);
}
