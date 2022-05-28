#pragma once

#include <cstdint>
#include <gmock/gmock.h>
#include <notesaladcore.h>

class MockMIDIDriver : public IMIDIDriver {
public:
    MOCK_METHOD(void, reset, (bool hardReset), (override));
    MOCK_METHOD(void, noteOn, (uint8_t channel, uint8_t note, uint8_t velocity), (override));
    MOCK_METHOD(void, noteOff, (uint8_t channel, uint8_t note, uint8_t velocity), (override));
    MOCK_METHOD(void, programChange, (uint8_t channel, uint8_t program), (override));
    MOCK_METHOD(void, controlChange, (uint8_t channel, uint8_t control, uint8_t value), (override));
    MOCK_METHOD(void, sysEx, (uint8_t * data, unsigned int length), (override));
    MOCK_METHOD(void, pitchWheel, (uint8_t channel, uint16_t value), (override));
    MOCK_METHOD(void, update, (), (override));
};
