#pragma once

#include "../../SD1/SD1DeviceBase.h"
#include "../../SD1/SD1Tone.h"
#include "../GlobalParams.h"
#include "../IToneGenerator.h"

class SD1ToneGenerator : public IToneGenerator<SD1Tone> {
public:
    SD1ToneGenerator(SD1DeviceBase* device);
    virtual void reset(bool hardReset);
    virtual void setTone(uint8_t voice, SD1Tone& tone, uint8_t volume, uint8_t pan);
    virtual void setPitch(uint8_t voice, float note);
    virtual void setVolume(uint8_t voice, uint8_t volume);
    virtual void setPan(uint8_t voice, uint8_t pan);
    virtual void noteOn(uint8_t voice, float note);
    virtual void noteOff(uint8_t voice);
    void setAnalogueGain(uint8_t gain);
    void setMasterVolume(uint8_t vol);
    void setMasterChannelVolume(uint8_t vol);
    void setGlobalParam(uint16_t param, uint16_t value);

private:
    SD1DeviceBase* device;
    SD1Tone voiceTones[16];
    uint8_t voiceVolumes[16];
    float voiceNotes[16];
    void writeTones(uint8_t lastVoice);
    void writePitch(uint8_t voice, float note);
};
