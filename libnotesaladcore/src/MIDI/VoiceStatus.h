#pragma once

#include <cstdint>

class VoiceStatus {
public:
    uint8_t channel;
    uint8_t noteNum;
    VoiceStatus();
    VoiceStatus(uint8_t channel, uint8_t noteNum);
};
