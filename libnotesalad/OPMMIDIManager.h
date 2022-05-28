#pragma once

#include <cstdint>

#include "CallbackOPMDevice.h"
#include "IMIDIManager.h"
#include "MIDI/MIDIParser.h"
#include "MIDI/OPM/OPMMIDISystem.h"

class OPMMIDIManager : public IMIDIManager {
public:
    OPMMIDIManager(void* context, OPMDeviceBase* device);
    virtual void reset();
    virtual void setReceiveCallback(std::function<void(void*, uint32_t)> callback);
    virtual void sendMIDI(uint8_t* msgData, uint32_t msgLength);
    virtual void setTime(uint32_t timeMS);
    virtual void update();

private:
    BufferedOPMDevice bufferedDevice;
    PatchManagerBase<OPMTone> patchManager;
    OPMMIDISystem midiSystem;
    MIDIParser parser;
};
