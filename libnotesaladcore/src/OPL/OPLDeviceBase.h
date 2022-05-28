#pragma once

#include <cstdint>

class OPLDeviceBase {
public:
    virtual ~OPLDeviceBase();
    virtual void hardReset() = 0;
    virtual void write(uint16_t addr, uint8_t data) = 0;
    virtual void softReset();
};
