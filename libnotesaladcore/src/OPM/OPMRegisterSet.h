#pragma once

#include <cstdint>

class OPMRegisterSet {
public:
    OPMRegisterSet();
    void set(uint8_t addr, uint8_t value);
    uint8_t get(uint8_t addr);
    void reset();

private:
    uint8_t registers[234];
};
