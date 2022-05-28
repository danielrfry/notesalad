#pragma once

#include "../../OPL/OPLTone.h"
#include "../BasicVoiceAllocator.h"

class OPL2VoiceAllocator : public BasicVoiceAllocator<OPLTone, 9> {
public:
    OPL2VoiceAllocator();
    virtual bool reserve(uint8_t& outVoice, const VoiceStatus& status, OPLTone& tone, uint8_t priority);
};
