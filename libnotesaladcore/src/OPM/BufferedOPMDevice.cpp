#include "BufferedOPMDevice.h"

BufferedOPMDevice::BufferedOPMDevice(OPMDeviceBase* baseDevice) : baseDevice(baseDevice)
{
}

void BufferedOPMDevice::hardReset()
{
    this->baseDevice->hardReset();
    this->registers.reset();
}

void BufferedOPMDevice::write(uint8_t addr, uint8_t data)
{
    if (this->registers.get(addr) != data) {
        this->registers.set(addr, data);
        this->baseDevice->write(addr, data);
    }
}

uint8_t BufferedOPMDevice::read(uint8_t addr)
{
    return this->registers.get(addr);
}
