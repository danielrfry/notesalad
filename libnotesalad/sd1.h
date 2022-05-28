#pragma once

#include "CallbackSD1Device.h"

extern "C" {
    typedef void* SD1DeviceRef;
    typedef void* SD1MIDIRef;

    SD1DeviceRef ntsld_sd1_cbkdev_new(void* context, SD1WriteCallback writeCallback, SD1DelayCallback delayCallback);

    void ntsld_sd1_delete(SD1DeviceRef device);

    SD1MIDIRef ntsld_sd1midi_new(void* context, SD1DeviceRef device);
}
