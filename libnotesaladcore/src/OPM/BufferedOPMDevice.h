#pragma once

#include "OPMDeviceBase.h"
#include "OPMRegisterSet.h"

class BufferedOPMDevice : public OPMDeviceBase {
public:
    BufferedOPMDevice(OPMDeviceBase* baseDevice);
    virtual void hardReset();
    virtual void write(uint8_t addr, uint8_t data);
    uint8_t read(uint8_t addr);

private:
    OPMDeviceBase* baseDevice;
    OPMRegisterSet registers;
};
