#include "OPL2VoiceAllocator.h"

OPL2VoiceAllocator::OPL2VoiceAllocator() : BasicVoiceAllocator()
{
}

bool OPL2VoiceAllocator::reserve(uint8_t& outVoice, const VoiceStatus& status, OPLTone& tone, uint8_t priority)
{
    if (tone.is4Op()) {
        return false;
    } else {
        return BasicVoiceAllocator<OPLTone, 9>::reserve(outVoice, status, tone, priority);
    }
}
