#pragma once

#include <cstdint>

#include "OPLReadWriteDeviceBase.h"

class OPLRegisterSet : public OPLReadWriteDeviceBase {
public:
    OPLRegisterSet();
    virtual void hardReset();
    virtual void write(uint16_t addr, uint8_t data);
    virtual uint8_t read(uint16_t addr);

private:
    uint8_t registers[288];
};
