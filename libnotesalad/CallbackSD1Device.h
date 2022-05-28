#pragma once

#include "SD1/SD1DeviceBase.h"

#include <cstdint>

extern "C" {
    typedef void (*SD1WriteCallback)(void* context, const uint8_t* data,
        const uint16_t len);
    typedef void (*SD1DelayCallback)(void* context, uint32_t microseconds);
}

class CallbackSD1Device : public SD1DeviceBase {
public:
    CallbackSD1Device(void* context, SD1WriteCallback writeCbk,
        SD1DelayCallback delayCbk);
    virtual ~CallbackSD1Device();
    virtual void delayMicroseconds(uint32_t us);
    virtual void write(const uint8_t* data, const uint16_t len);
    virtual void hardReset();

private:
    void* context;
    SD1WriteCallback writeCbk;
    SD1DelayCallback delayCbk;
};
