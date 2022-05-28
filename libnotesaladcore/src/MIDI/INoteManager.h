#pragma once

#include <cstdint>

class INoteManager {
public:
    virtual void resetState() = 0;
    virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity) = 0;
    virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity) = 0;
    virtual void setSustain(uint8_t channel, bool sustainActive) = 0;
    virtual void allNotesOff(uint8_t channel) = 0;
};
