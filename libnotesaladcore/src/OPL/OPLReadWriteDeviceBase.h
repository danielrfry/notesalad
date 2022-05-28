#pragma once

#include "OPLDeviceBase.h"
#include "OPLTone.h"

class OPLReadWriteDeviceBase : public OPLDeviceBase {
public:
    virtual uint8_t read(uint16_t addr) = 0;

    bool is4OpChannelEnabled(uint8_t ch4Op);
    void set4OpChannelEnabled(uint8_t ch4Op, bool enabled);
    void setChannelKeyOn(uint8_t channel, bool keyOn);

    void allNotesOff();
    void setMinimumReleaseRate(uint8_t rr);

    void pauseEnvelope(uint8_t voice);
    void pauseEnvelope();

    void readTone(uint8_t voice, OPLTone& tone);
    void writeTone(uint8_t voice, OPLTone& tone);
    bool isVoiceEnabled(uint8_t voice);
    void enableVoice(uint8_t voice);
    void setVoiceKeyOn(uint8_t voice, bool keyOn);

    void copyTo(OPLDeviceBase& dest);
};
