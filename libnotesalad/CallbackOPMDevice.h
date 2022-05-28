#pragma once

#include "OPM/OPMDeviceBase.h"

extern "C" {
    typedef void (*OPMWriteCallback)(void* context, uint8_t addr, uint8_t data);
    typedef void (*OPMResetCallback)(void* context);
}

class CallbackOPMDevice : public OPMDeviceBase {
public:
    void* context;
    OPMWriteCallback writeCallback;
    OPMResetCallback resetCallback;
    CallbackOPMDevice(void* context, OPMWriteCallback writeCallback,
        OPMResetCallback resetCallback);
    virtual ~CallbackOPMDevice();
    virtual void hardReset();
    virtual void write(uint8_t addr, uint8_t data);
};
