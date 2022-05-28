#pragma once

#include <cstdint>

class OPMDeviceBase {
public:
    virtual ~OPMDeviceBase();
    virtual void hardReset() = 0;
    virtual void write(uint8_t addr, uint8_t data) = 0;
};
