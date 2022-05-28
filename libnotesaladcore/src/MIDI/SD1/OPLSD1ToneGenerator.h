#pragma once

#include "../../OPL/OPLTone.h"
#include "../../SD1/SD1DeviceBase.h"
#include "../IToneGenerator.h"
#include "SD1ToneGenerator.h"

#include <cstdint>

class OPLSD1ToneGenerator : public IToneGenerator<OPLTone> {
public:
    OPLSD1ToneGenerator(SD1DeviceBase* device);
    virtual void reset(bool hardReset);
    virtual void setTone(uint8_t voice, OPLTone& tone, uint8_t volume, uint8_t pan);
    virtual void setPitch(uint8_t voice, float note);
    virtual void setVolume(uint8_t voice, uint8_t volume);
    virtual void setPan(uint8_t voice, uint8_t pan);
    virtual void noteOn(uint8_t voice, float note);
    virtual void noteOff(uint8_t voice);

private:
    SD1ToneGenerator sd1ToneGenerator;
    OPLTone voiceTones[16];
    uint8_t voiceVolumes[16];
    void updateTone(uint8_t voice);
};
