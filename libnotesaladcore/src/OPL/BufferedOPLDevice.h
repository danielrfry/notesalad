#pragma once

#include "OPLDeviceBase.h"
#include "OPLRegisterSet.h"

class BufferedOPLDevice : public OPLReadWriteDeviceBase {
public:
    BufferedOPLDevice(OPLDeviceBase* baseDevice);
    virtual void hardReset();
    virtual void softReset();
    virtual void write(uint16_t addr, uint8_t data);
    virtual uint8_t read(uint16_t addr);

private:
    OPLDeviceBase* baseDevice;
    OPLRegisterSet registers;
};
