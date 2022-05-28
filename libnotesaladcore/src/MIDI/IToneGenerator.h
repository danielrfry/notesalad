#pragma once

#include <cstdint>

template <class Tone>
class IToneGenerator {
public:
    virtual void reset(bool hardReset) = 0;
    virtual void setTone(uint8_t voice, Tone& tone, uint8_t volume, uint8_t pan) = 0;
    virtual void setPitch(uint8_t voice, float note) = 0;
    virtual void setVolume(uint8_t voice, uint8_t volume) = 0;
    virtual void setPan(uint8_t voice, uint8_t pan) = 0;
    virtual void noteOn(uint8_t voice, float note) = 0;
    virtual void noteOff(uint8_t voice) = 0;
    // Returns true if a note is playing (including during release phase).
    // Used to determine whether to apply modulation, for example.
    virtual bool isNoteActive(uint8_t voice)
    {
        return true;
    };
    virtual void setGlobalParam(uint16_t paramID, uint16_t value) { }
};
