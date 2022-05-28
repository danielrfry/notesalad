#pragma once

#include "../../OPM/BufferedOPMDevice.h"
#include "../../OPM/OPMTone.h"
#include "../IToneGenerator.h"

class OPMToneGenerator : public IToneGenerator<OPMTone> {
public:
    OPMToneGenerator(BufferedOPMDevice* device);
    virtual void reset(bool hardReset);
    virtual void setTone(uint8_t voice, OPMTone& tone, uint8_t volume, uint8_t pan);
    virtual void setPitch(uint8_t voice, float note);
    virtual void setVolume(uint8_t voice, uint8_t volume);
    virtual void setPan(uint8_t voice, uint8_t pan);
    virtual void noteOn(uint8_t voice, float note);
    virtual void noteOff(uint8_t voice);

private:
    BufferedOPMDevice* device;
    OPMTone voiceTones[8];
    uint8_t voiceVolume[8];
    uint8_t voicePan[8];
    void resetState();
    void writeTone(uint8_t voice, OPMTone& tone);
    void updateTone(uint8_t voice);
};
