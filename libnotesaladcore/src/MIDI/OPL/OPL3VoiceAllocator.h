#pragma once

#include "../../OPL/OPLTone.h"
#include "../BasicVoiceAllocator.h"
#include "../IVoiceAllocator.h"

#include <bitset>

class OPL3VoiceAllocator : public IVoiceAllocator<OPLTone> {
public:
    virtual void reset();
    virtual bool reserve(uint8_t& outVoice, const VoiceStatus& status, OPLTone& tone, uint8_t priority);
    virtual void setPriority(uint8_t voice, uint8_t newPriority);
    virtual uint8_t getPriority(uint8_t voice);
    virtual const VoiceStatus* getVoiceStatus(uint8_t voice);
    virtual void applyToChannelVoices(uint8_t channel, std::function<void(uint8_t, const VoiceStatus*)> fn);
    virtual bool isVoiceEnabled(uint8_t voice);
    virtual bool isVoiceCompatible(uint8_t voice, OPLTone& tone);

private:
    BasicVoiceAllocator<OPLTone, 18> alloc2Op;
    BasicVoiceAllocator<OPLTone, 6> alloc4Op;
    std::bitset<6> voice4OpModes;
    void setPriority(uint8_t voice, uint8_t newPriority, bool alreadyReserved);
};
