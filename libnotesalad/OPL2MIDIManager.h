#pragma once

#include "CallbackOPLDevice.h"
#include "IMIDIManager.h"
#include "MIDI/MIDIParser.h"
#include "MIDI/OPL/DefaultOPLPatchManager.h"
#include "MIDI/OPL/OPL2MIDISystem.h"

class OPL2MIDIManager : public IMIDIManager {
public:
    OPL2MIDIManager(void* context, OPLDeviceBase* device);
    virtual void reset();
    virtual void setReceiveCallback(std::function<void(void*, uint32_t)> callback);
    virtual void sendMIDI(uint8_t* msgData, uint32_t msgLength);
    virtual void setTime(uint32_t timeMS);
    virtual void update();

private:
    BufferedOPLDevice bufferedDevice;
    DefaultOPLPatchManager patchManager;
    OPL2MIDISystem midiSystem;
    MIDIParser parser;
};
