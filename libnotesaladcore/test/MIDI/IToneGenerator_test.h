#pragma once

#include <cstdint>
#include <gmock/gmock.h>
#include <notesaladcore.h>

#include "TestTone.h"

class MockToneGenerator : public IToneGenerator<TestTone> {
public:
    MOCK_METHOD(void, reset, (bool hardReset), (override));
    MOCK_METHOD(void, setTone, (uint8_t voice, TestTone& tone, uint8_t volume, uint8_t pan), (override));
    MOCK_METHOD(void, setPitch, (uint8_t voice, float note), (override));
    MOCK_METHOD(void, setVolume, (uint8_t voice, uint8_t volume), (override));
    MOCK_METHOD(void, setPan, (uint8_t voice, uint8_t pan), (override));
    MOCK_METHOD(void, noteOn, (uint8_t voice, float note), (override));
    MOCK_METHOD(void, noteOff, (uint8_t voice), (override));
    MOCK_METHOD(bool, isNoteActive, (uint8_t voice), (override));
    MOCK_METHOD(void, setGlobalParam, (uint16_t paramID, uint16_t value), (override));
};
