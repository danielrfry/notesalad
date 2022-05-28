#pragma once

#include <cstdint>
#include <functional>

class IMIDIDriver {
public:
    std::function<void(void*, uint32_t)> onSendMIDI;

    virtual void reset(bool hardReset) = 0;
    virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity) = 0;
    virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity) = 0;
    virtual void programChange(uint8_t channel, uint8_t program) = 0;
    virtual void controlChange(uint8_t channel, uint8_t control, uint8_t value) = 0;
    virtual void sysEx(uint8_t* data, unsigned int length) = 0;
    virtual void pitchWheel(uint8_t channel, uint16_t value) = 0;
    virtual void update() = 0;
};
