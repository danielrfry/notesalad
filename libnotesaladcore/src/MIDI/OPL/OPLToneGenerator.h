#pragma once

#include "../../OPL/BufferedOPLDevice.h"
#include "../../OPL/OPLTone.h"
#include "../IToneGenerator.h"
#include "../TimeSource.h"

class OPLToneGenerator : public IToneGenerator<OPLTone> {
public:
    OPLToneGenerator(OPLReadWriteDeviceBase* device, TimeSource* timeSource);
    virtual void reset(bool hardReset);
    virtual void setTone(uint8_t voice, OPLTone& tone, uint8_t volume, uint8_t pan);
    virtual void setPitch(uint8_t voice, float note);
    virtual void setVolume(uint8_t voice, uint8_t volume);
    virtual void setPan(uint8_t voice, uint8_t pan);
    virtual void noteOn(uint8_t voice, float note);
    virtual void noteOff(uint8_t voice);
    virtual bool isNoteActive(uint8_t voice);

private:
    OPLReadWriteDeviceBase* device;
    TimeSource* timeSource;
    OPLTone voiceTones[24];
    uint8_t voiceVolume[24];
    uint8_t voicePan[24];
    Milliseconds voiceReleaseTime[24];
    uint8_t voiceStates[24];
    void resetState();
    void updateTone(uint8_t voice);
    void setPitch(uint8_t voice, float note, bool keyOn);
    Milliseconds getReleaseDuration(uint8_t releaseRate);
    Milliseconds getReleaseRate(OPLTone& tone);
    Milliseconds getReleaseTime(uint8_t releaseRate);
};
