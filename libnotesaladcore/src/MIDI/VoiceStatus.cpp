#include "VoiceStatus.h"

VoiceStatus::VoiceStatus() : VoiceStatus(255, 255)
{
}

VoiceStatus::VoiceStatus(uint8_t channel, uint8_t noteNum) : channel(channel), noteNum(noteNum)
{
}
