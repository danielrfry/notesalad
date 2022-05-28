#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "IVoiceAllocator.h"

template <class Tone, uint8_t voices>
class BasicVoiceAllocator : public IVoiceAllocator<Tone> {
public:
    BasicVoiceAllocator()
        : voiceQueue(voices)
    {
        this->reset();
    }

    virtual void reset()
    {
        voiceQueue.clear();
        for (uint8_t i = 0; i < voices; i++) {
            voicePriorities[i] = 0;
            voiceStatuses[i] = VoiceStatus();
            voiceQueue.push_back(i);
        }
    }

    virtual bool reserve(uint8_t& outVoice, const VoiceStatus& status, Tone& tone, uint8_t priority)
    {
        uint8_t nextVoice = this->voiceQueue[0];
        if (this->voicePriorities[nextVoice] > priority) {
            // No available voices at the same or lower priority
            return false;
        }
        this->setPriority(nextVoice, priority);
        this->voiceStatuses[nextVoice] = status;
        outVoice = nextVoice;
        return true;
    }

    virtual void setPriority(uint8_t voice, uint8_t newPriority)
    {
        this->voiceQueue.erase(std::remove(this->voiceQueue.begin(), this->voiceQueue.end(), voice),
            this->voiceQueue.end());
        auto insertPos = std::find_if(this->voiceQueue.begin(), this->voiceQueue.end(), [this, newPriority](uint8_t& voiceIndex) {
            return this->voicePriorities[voiceIndex] > newPriority;
        });
        this->voiceQueue.insert(insertPos, voice);
        this->voicePriorities[voice] = newPriority;
    }

    virtual uint8_t getPriority(uint8_t voice)
    {
        return this->voicePriorities[voice];
    }

    virtual const VoiceStatus* getVoiceStatus(uint8_t voice)
    {
        return &this->voiceStatuses[voice];
    }

    virtual void applyToChannelVoices(uint8_t channel, std::function<void(uint8_t, const VoiceStatus*)> fn)
    {
        for (uint8_t v = 0; v < voices; v++) {
            if (this->getChannel(v) == channel) {
                fn(v, &this->voiceStatuses[v]);
            }
        }
    }

private:
    uint8_t voicePriorities[voices];
    VoiceStatus voiceStatuses[voices];
    std::vector<uint8_t> voiceQueue;
};
