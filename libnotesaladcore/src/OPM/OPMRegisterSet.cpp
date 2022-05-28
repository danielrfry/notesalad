#include "OPMRegisterSet.h"

#include <cstring>

static int getArrayIndexForRegister(uint8_t addr)
{
    if (addr >= 0x20) {
        return addr - 0x20;
    } else {
        switch (addr) {
        case 0x01:
            return 224;
        case 0x08:
            return 225;
        case 0x0f:
            return 226;
        case 0x10:
            return 227;
        case 0x11:
            return 228;
        case 0x12:
            return 229;
        case 0x14:
            return 230;
        case 0x18:
            return 231;
        case 0x19:
            return 232;
        case 0x1b:
            return 233;
        }
    }

    return -1;
}

OPMRegisterSet::OPMRegisterSet()
{
    this->reset();
}

void OPMRegisterSet::reset()
{
    memset(&this->registers[0], 0, 234);
}

void OPMRegisterSet::set(uint8_t addr, uint8_t value)
{
    int index = getArrayIndexForRegister(addr);
    if (index >= 0) {
        this->registers[index] = value;
    }
}

uint8_t OPMRegisterSet::get(uint8_t addr)
{
    int index = getArrayIndexForRegister(addr);
    if (index < 0) {
        return 0;
    } else {
        return this->registers[index];
    }
}