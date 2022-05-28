#pragma once

#include "OPL/OPLDeviceBase.h"

extern "C" {
    typedef void (*OPLWriteCallback)(void* context, uint16_t addr, uint8_t data);
    typedef void (*OPLResetCallback)(void* context);
}

class CallbackOPLDevice : public OPLDeviceBase {
public:
    void* context;
    OPLWriteCallback writeCallback;
    OPLResetCallback resetCallback;
    CallbackOPLDevice(void* context, OPLWriteCallback writeCallback,
        OPLResetCallback resetCallback);
    virtual ~CallbackOPLDevice();
    virtual void hardReset();
    virtual void write(uint16_t addr, uint8_t data);
};
