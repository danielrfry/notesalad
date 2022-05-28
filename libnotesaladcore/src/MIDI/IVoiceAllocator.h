#pragma once

#include <cstdint>
#include <functional>

#include "VoiceStatus.h"

template <class Tone>
class IVoiceAllocator {
public:
    virtual void reset() = 0;
    virtual bool reserve(uint8_t& outVoice, const VoiceStatus& status, Tone& tone, uint8_t priority) = 0;
    virtual void setPriority(uint8_t voice, uint8_t newPriority) = 0;
    inline void release(uint8_t voice)
    {
        this->setPriority(voice, 0);
    }
    virtual uint8_t getPriority(uint8_t voice) = 0;
    virtual const VoiceStatus* getVoiceStatus(uint8_t voice) = 0;
    uint8_t getChannel(uint8_t voice)
    {
        return this->getVoiceStatus(voice)->channel;
    }
    virtual void applyToChannelVoices(uint8_t channel, std::function<void(uint8_t, const VoiceStatus*)> fn) = 0;
    void applyToChannelVoices(uint8_t channel, std::function<void(uint8_t)> fn)
    {
        this->applyToChannelVoices(channel, [fn](uint8_t v, const VoiceStatus* status) { fn(v); });
    }
    virtual bool isVoiceEnabled(uint8_t voice)
    {
        return true;
    }
    virtual bool isVoiceCompatible(uint8_t voice, Tone& tone)
    {
        return true;
    }
};
