#include "opm.h"
#include "OPMEmulator.h"
#include "OPMMIDIManager.h"

OPMDeviceRef ntsld_opm_emu_new(uint32_t sampleRate)
{
    return dynamic_cast<OPMDeviceBase*>(new OPMEmulator(sampleRate));
}

OPMDeviceRef ntsld_opm_cbkdev_new(void* context, OPMWriteCallback writeCallback, OPMResetCallback resetCallback)
{
    return dynamic_cast<OPMDeviceBase*>(new CallbackOPMDevice(context, writeCallback, resetCallback));
}

void ntsld_opm_delete(OPMDeviceRef device)
{
    delete ((OPMDeviceBase*)device);
}

void ntsld_opm_reset(OPMDeviceRef device)
{
    ((OPMDeviceBase*)device)->hardReset();
}

void ntsld_opm_write(OPMDeviceRef device, uint8_t addr, uint8_t data)
{
    ((OPMDeviceBase*)device)->write(addr, data);
}

void ntsld_opm_emu_getsamples(OPMDeviceRef device, int16_t* output, int numSamples)
{
    dynamic_cast<OPMEmulator*>((OPMDeviceBase*)device)->getSamples(output, numSamples);
}

void ntsld_opm_emu_getsamples_offset(OPMDeviceRef device, int16_t* output, int offsetSamples, int numSamples)
{
    ntsld_opm_emu_getsamples(device, output + (offsetSamples * 2), numSamples);
}

OPMMIDIRef ntsld_opmmidi_new(void* context, OPMDeviceRef device)
{
    return dynamic_cast<IMIDIManager*>(new OPMMIDIManager(context, (OPMDeviceBase*)device));
}
