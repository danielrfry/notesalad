#pragma once

#include <cstdint>

#include "CallbackOPMDevice.h"
#include "midi.h"

extern "C" {
    typedef void* OPMDeviceRef;
    typedef MIDIRef OPMMIDIRef;

    OPMDeviceRef ntsld_opm_emu_new(uint32_t sampleRate);
    OPMDeviceRef ntsld_opm_cbkdev_new(void* context, OPMWriteCallback writeCallback, OPMResetCallback resetCallback);

    void ntsld_opm_delete(OPMDeviceRef device);
    void ntsld_opm_reset(OPMDeviceRef device);
    void ntsld_opm_write(OPMDeviceRef device, uint8_t addr, uint8_t data);

    void ntsld_opm_emu_getsamples(OPMDeviceRef device, int16_t* output, int numSamples);
    void ntsld_opm_emu_getsamples_offset(OPMDeviceRef device, int16_t* output, int offsetSamples, int numSamples);

    OPMMIDIRef ntsld_opmmidi_new(void* context, OPMDeviceRef device);
}
