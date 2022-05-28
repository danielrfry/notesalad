#pragma once

#include <cstdint>

#include "CallbackOPLDevice.h"
#include "IMIDIManager.h"
#include "MIDI/MIDIParser.h"
#include "MIDI/OPL/DefaultOPLPatchManager.h"
#include "MIDI/OPL/OPL3MIDISystem.h"
#include "MIDI/RAMPatchManager.h"

class OPL3MIDIManager : public IMIDIManager {
public:
    OPL3MIDIManager(void* context, OPLDeviceBase* device);
    virtual void reset();
    virtual void setReceiveCallback(std::function<void(void*, uint32_t)> callback);
    virtual void sendMIDI(uint8_t* msgData, uint32_t msgLength);
    virtual void setTime(uint32_t timeMS);
    virtual void update();

private:
    BufferedOPLDevice bufferedDevice;
    DefaultOPLPatchManager gmPatchManager;
    RAMPatchManager<OPLTone, 256> patchManager;
    OPL3MIDISystem midiSystem;
    MIDIParser parser;
};
