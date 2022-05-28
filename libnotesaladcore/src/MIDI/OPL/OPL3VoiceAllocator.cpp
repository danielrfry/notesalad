#include "OPL3VoiceAllocator.h"
#include "../../OPL/OPLUtils.h"

#include <algorithm>

void OPL3VoiceAllocator::reset()
{
    this->alloc2Op.reset();
    this->alloc4Op.reset();
    this->voice4OpModes.reset();
}

bool OPL3VoiceAllocator::reserve(uint8_t& outVoice, const VoiceStatus& status, OPLTone& tone, uint8_t priority)
{
    if (tone.toneData.flags & OPL_TONE_FLAG_4OP) {
        uint8_t voice4Op;
        if (this->alloc4Op.reserve(voice4Op, status, tone, priority)) {
            outVoice = voice4Op + 18;
            this->setPriority(outVoice, priority, true);
            return true;
        } else {
            return false;
        }
    } else {
        uint8_t voice2Op;
        if (this->alloc2Op.reserve(voice2Op, status, tone, priority)) {
            outVoice = voice2Op;
            this->setPriority(outVoice, priority, true);
            return true;
        } else {
            return false;
        }
    }
}

void OPL3VoiceAllocator::setPriority(uint8_t voice, uint8_t newPriority)
{
    this->setPriority(voice, newPriority, false);
}

uint8_t OPL3VoiceAllocator::getPriority(uint8_t voice)
{
    if (voice > 17) {
        return this->alloc4Op.getPriority(voice - 18);
    } else {
        return this->alloc2Op.getPriority(voice);
    }
}

void OPL3VoiceAllocator::setPriority(uint8_t voice, uint8_t newPriority, bool alreadyReserved)
{
    if (voice > 17) {
        uint8_t voice4Op = voice - 18;
        uint8_t voices2Op[2];
        OPL::get2OpChannelsFor4OpChannel(voice4Op, &voices2Op[0]);
        if (!alreadyReserved) {
            this->alloc4Op.setPriority(voice4Op, newPriority);
        }
        this->alloc2Op.setPriority(voices2Op[0], newPriority);
        this->alloc2Op.setPriority(voices2Op[1], newPriority);
        this->voice4OpModes[voice4Op] = true;
    } else {
        if (!alreadyReserved) {
            this->alloc2Op.setPriority(voice, newPriority);
        }
        uint8_t voice4Op = OPL::get4OpChannelFor2OpChannel(voice);
        uint8_t voices2Op[2];
        OPL::get2OpChannelsFor4OpChannel(voice4Op, &voices2Op[0]);
        if (voice4Op != 255) {
            uint8_t maxPriority = std::max(
                this->alloc2Op.getPriority(voices2Op[0]),
                this->alloc2Op.getPriority(voices2Op[1]));
            this->alloc4Op.setPriority(voice4Op, maxPriority);
            this->voice4OpModes[voice4Op] = false;
        }
    }
}

const VoiceStatus* OPL3VoiceAllocator::getVoiceStatus(uint8_t voice)
{
    if (voice > 17) {
        return this->alloc4Op.getVoiceStatus(voice - 18);
    } else {
        return this->alloc2Op.getVoiceStatus(voice);
    }
}

void OPL3VoiceAllocator::applyToChannelVoices(uint8_t channel, std::function<void(uint8_t, const VoiceStatus*)> fn)
{
    for (uint8_t v = 0; v < 24; v++) {
        if (this->getChannel(v) == channel) {
            auto status = this->getVoiceStatus(v);
            fn(v, status);
        }
    }
}

bool OPL3VoiceAllocator::isVoiceEnabled(uint8_t voice)
{
    if (voice > 17) {
        return this->voice4OpModes[voice - 18];
    } else {
        uint8_t voice4Op = OPL::get4OpChannelFor2OpChannel(voice);
        return !this->voice4OpModes[voice4Op];
    }
}

bool OPL3VoiceAllocator::isVoiceCompatible(uint8_t voice, OPLTone& tone)
{
    if (voice > 17) {
        return tone.is4Op();
    } else {
        return !tone.is4Op();
    }
}