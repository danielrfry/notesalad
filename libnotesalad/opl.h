#pragma once

#include <cstdint>

#include "CallbackOPLDevice.h"
#include "midi.h"

extern "C" {
    typedef void* OPLDeviceRef;
    typedef MIDIRef OPL2MIDIRef;
    typedef MIDIRef OPL3MIDIRef;

    OPLDeviceRef ntsld_opl_emu_new(uint32_t sampleRate);
    OPLDeviceRef ntsld_opl_cbkdev_new(void* context, OPLWriteCallback writeCallback, OPLResetCallback resetCallback);

    void ntsld_opl_delete(OPLDeviceRef device);
    void ntsld_opl_reset(OPLDeviceRef device);
    void ntsld_opl_write(OPLDeviceRef device, uint16_t addr, uint8_t data);

    void ntsld_opl_emu_getsamples(OPLDeviceRef device, int16_t* output, int numSamples);

    OPL2MIDIRef ntsld_opl2midi_new(void* context, OPLDeviceRef device);
    OPL3MIDIRef ntsld_opl3midi_new(void* context, OPLDeviceRef device);
}
