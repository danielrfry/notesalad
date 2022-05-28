#pragma once

#include <cstdint>

#include "CallbackSD1Device.h"
#include "IMIDIManager.h"
#include "MIDI/MIDIParser.h"
#include "MIDI/SD1/DefaultSD1PatchManager.h"
#include "MIDI/SD1/SD1MIDISystem.h"

class SD1MIDIManager : public IMIDIManager {
public:
    SD1MIDIManager(void* context, SD1DeviceBase* device);
    virtual void reset();
    virtual void setReceiveCallback(std::function<void(void*, uint32_t)> callback);
    virtual void sendMIDI(uint8_t* msgData, uint32_t msgLength);
    virtual void setTime(uint32_t timeMS);
    virtual void update();

private:
    DefaultSD1PatchManager patchManager;
    SD1MIDISystem midiSystem;
    MIDIParser parser;
};
