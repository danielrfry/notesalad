#include "opl.h"
#include "OPL2MIDIManager.h"
#include "OPL3MIDIManager.h"
#include "OPLEmulator.h"

OPLDeviceRef ntsld_opl_emu_new(uint32_t sampleRate)
{
    return dynamic_cast<OPLDeviceBase*>(new OPLEmulator(sampleRate));
}

OPLDeviceRef ntsld_opl_cbkdev_new(void* context, OPLWriteCallback writeCallback, OPLResetCallback resetCallback)
{
    return dynamic_cast<OPLDeviceBase*>(new CallbackOPLDevice(context, writeCallback, resetCallback));
}

void ntsld_opl_delete(OPLDeviceRef device)
{
    delete ((OPLDeviceBase*)device);
}

void ntsld_opl_reset(OPLDeviceRef device)
{
    ((OPLDeviceBase*)device)->hardReset();
}

void ntsld_opl_write(OPLDeviceRef device, uint16_t addr, uint8_t data)
{
    ((OPLDeviceBase*)device)->write(addr, data);
}

void ntsld_opl_emu_getsamples(OPLDeviceRef device, int16_t* output, int numSamples)
{
    dynamic_cast<OPLEmulator*>((OPLDeviceBase*)device)->getSamples(output, numSamples);
}

void ntsld_opl_emu_getsamples_offset(OPLDeviceRef device, int16_t* output, int offsetSamples, int numSamples)
{
    ntsld_opl_emu_getsamples(device, output + (offsetSamples * 2), numSamples);
}

OPL2MIDIRef ntsld_opl2midi_new(void* context, OPLDeviceRef device)
{
    return dynamic_cast<IMIDIManager*>(new OPL2MIDIManager(context, (OPLDeviceBase*)device));
}

OPL3MIDIRef ntsld_opl3midi_new(void* context, OPLDeviceRef device)
{
    return dynamic_cast<IMIDIManager*>(new OPL3MIDIManager(context, (OPLDeviceBase*)device));
}
