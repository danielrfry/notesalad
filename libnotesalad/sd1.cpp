#include "sd1.h"
#include "SD1MIDIManager.h"

SD1DeviceRef ntsld_sd1_cbkdev_new(void* context, SD1WriteCallback writeCallback, SD1DelayCallback delayCallback)
{
    return new CallbackSD1Device(context, writeCallback, delayCallback);
}

void ntsld_sd1_delete(SD1DeviceRef device)
{
    delete ((SD1DeviceBase*)device);
}

SD1MIDIRef ntsld_sd1midi_new(void* context, SD1DeviceRef device)
{
    return new SD1MIDIManager(context, (SD1DeviceBase*)device);
}
