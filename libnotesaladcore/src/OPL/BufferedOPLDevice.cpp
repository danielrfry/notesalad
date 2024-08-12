#include "BufferedOPLDevice.h"

BufferedOPLDevice::BufferedOPLDevice(OPLDeviceBase* baseDevice) : baseDevice(baseDevice)
{
}

void BufferedOPLDevice::hardReset()
{
    this->baseDevice->hardReset();
    this->registers.hardReset();
}

void BufferedOPLDevice::softReset()
{
    this->baseDevice->softReset();
    this->registers.hardReset();
}

void BufferedOPLDevice::write(uint16_t addr, uint8_t data)
{
    if (this->registers.read(addr) != data) {
        this->registers.write(addr, data);
        this->baseDevice->write(addr, data);
    }
}

uint8_t BufferedOPLDevice::read(uint16_t addr)
{
    return this->registers.read(addr);
}
