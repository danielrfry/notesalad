#pragma once

#include <cstdint>

class SD1DeviceBase {
public:
    virtual ~SD1DeviceBase();
    virtual void delayMicroseconds(uint32_t us) = 0;
    virtual void write(const uint8_t* data, const uint16_t len) = 0;
    void writeReg(uint8_t addr, uint8_t data);
    virtual void hardReset() = 0;
};
