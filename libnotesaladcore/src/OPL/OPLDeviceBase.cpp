#include "OPLDeviceBase.h"
#include "OPLUtils.h"

OPLDeviceBase::~OPLDeviceBase()
{
}

void OPLDeviceBase::softReset()
{
    // Set decay rate to maximum to reset envelope generators
    for (uint8_t i = 0; i <= 0x15; i++) {
        this->write(0x60 + i, 0x0f);
        this->write(0x160 + i, 0x0f);
    }

    // Disable all channels to immediately stop all sound
    for (uint8_t ch = 0; ch <= 0x08; ch++) {
        this->write(0xc0 + ch, 0);
        this->write(0x1c0 + ch, 0);
    }

    // Zero other registers
    this->write(0x104, 0);
    this->write(0x08, 0);

    for (uint8_t i = 0; i <= 0x15; i++) {
        this->write(0x20 + i, 0);
        this->write(0x40 + i, 0);
        this->write(0x60 + i, 0);
        this->write(0x80 + i, 0);
        this->write(0xe0 + i, 0);
        this->write(0x120 + i, 0);
        this->write(0x140 + i, 0);
        this->write(0x160 + i, 0);
        this->write(0x180 + i, 0);
        this->write(0x1e0 + i, 0);
    }

    for (uint8_t i = 0; i <= 0x08; i++) {
        this->write(0xa0 + i, 0);
        this->write(0xb0 + i, 0);
        this->write(0xc0 + i, 0);
        this->write(0x1a0 + i, 0);
        this->write(0x1b0 + i, 0);
        this->write(0x1c0 + i, 0);
    }

    this->write(0xbd, 0);
}
