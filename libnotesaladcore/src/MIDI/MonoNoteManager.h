#pragma once

#include <cstdint>

#include "INoteManager.h"
#include "IToneGenerator.h"
#include "IVoiceAllocator.h"
#include "Patch.h"
#include "TimeSource.h"
#include "ToneController.h"

#define MONO_MAX_NOTES 5

#define MONO_FLAG_SUSTAINACTIVE 1
#define MONO_FLAG_NOTE_PLAYING 2

typedef struct MonoChannelStatus {
    uint8_t voice;
    uint8_t flags;
    uint8_t notes[MONO_MAX_NOTES];
    uint8_t nextNoteIndex;
    float currentPitch;
    float initialPitch;
    float targetPitch;
    Milliseconds glideStartTime;
    Milliseconds glideDuration;
    bool isSustainActive();
    void setSustainActive(bool newSustainActive);
    bool isNotePlaying();
    void setNotePlaying(bool newNotePlaying);
} MonoChannelStatus;

bool shouldGlideFromLastNote(uint8_t polyMode);
bool isLegato(uint8_t polyMode);

template <class Tone, uint8_t voices>
class MonoNoteManager : public INoteManager {
public:
    MonoNoteManager(IVoiceAllocator<Tone>* voiceAllocator,
        ToneController<Tone, voices>* toneController,
        TimeSource* timeSource)
        : voiceAllocator(voiceAllocator),
          toneController(toneController),
          timeSource(timeSource)
    {
        this->resetState();
    }
    virtual void resetState()
    {
        for (uint8_t c = 0; c < 16; c++) {
            auto& status = this->channelStatuses[c];
            status.voice = 255;
            status.flags = 0;
            status.nextNoteIndex = 0;
            status.currentPitch = 0;
            status.initialPitch = 0;
            status.targetPitch = 0;
            status.glideStartTime = 0;
            status.glideDuration = 0;
        }
    }
    virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        auto& status = this->channelStatuses[channel];
        Patch<Tone> patch;
        if (!this->toneController->getNotePatch(channel, note, patch))
            return;

        if (status.isNotePlaying()) {
            // Existing note
            if (isLegato(patch.params.polyMode)) {
                this->removeNote(this->channelStatuses[channel], note);
                this->addNote(this->channelStatuses[channel], note);
            } else {
                this->setNote(this->channelStatuses[channel], note);
                this->toneController->noteOff(status.voice);
                this->toneController->noteOn(channel, status.voice, note, status.currentPitch, velocity);
            }
            this->updateNote(channel);
        } else {
            // New note
            uint8_t voice;
            if (!this->reserveVoice(channel, voice, patch.params.tone)) {
                return;
            }
            status.voice = voice;
            status.setNotePlaying(true);

            uint8_t fixedNoteNum = patch.params.fixedNoteNum;
            float pitch = fixedNoteNum == 0 ? note : fixedNoteNum;

            if (shouldGlideFromLastNote(patch.params.polyMode) && status.currentPitch != 0.0f) {
                status.initialPitch = status.currentPitch;
                status.glideDuration = patch.params.glideDurationMS;
            } else {
                status.initialPitch = pitch;
                status.currentPitch = pitch;
                status.glideDuration = 0;
            }
            status.targetPitch = pitch;
            status.glideStartTime = this->timeSource->timeMS;

            this->addNote(this->channelStatuses[channel], note);
            this->toneController->noteOn(channel, voice, note, status.currentPitch, velocity);
        }
    }
    virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        auto& status = this->channelStatuses[channel];
        this->removeNote(status, note);
        this->updateNote(channel);
    }
    virtual void setSustain(uint8_t channel, bool sustainActive)
    {
        auto& status = this->channelStatuses[channel];
        if (status.isSustainActive() != sustainActive) {
            status.setSustainActive(sustainActive);
            this->updateNote(channel);
        }
    }
    virtual void allNotesOff(uint8_t channel)
    {
        auto& status = this->channelStatuses[channel];
        if (status.isNotePlaying()) {
            this->stopNote(channel, status.voice);
        }
    }
    void update()
    {
        for (uint8_t ch = 0; ch < 16; ch++) {
            this->updateChannelPitch(ch);
        }
    }

private:
    IVoiceAllocator<Tone>* voiceAllocator;
    ToneController<Tone, voices>* toneController;
    TimeSource* timeSource;
    MonoChannelStatus channelStatuses[16];
    void addNote(MonoChannelStatus& status, uint8_t noteNum)
    {
        if (status.nextNoteIndex == MONO_MAX_NOTES) {
            for (uint8_t i = 0; i < MONO_MAX_NOTES - 1; i++) {
                status.notes[i] = status.notes[i + 1];
            }
            status.notes[MONO_MAX_NOTES - 1] = noteNum;
        } else {
            status.notes[status.nextNoteIndex++] = noteNum;
        }
    }
    void removeNote(MonoChannelStatus& status, uint8_t noteNum)
    {
        uint8_t dest = 0;
        for (uint8_t src = 0; src < status.nextNoteIndex; src++) {
            if (status.notes[src] != noteNum) {
                status.notes[dest++] = status.notes[src];
            }
        }
        status.nextNoteIndex = dest;
    }
    void setNote(MonoChannelStatus& status, uint8_t noteNum)
    {
        status.notes[0] = noteNum;
        status.nextNoteIndex = 1;
    }
    bool getVoice(uint8_t channel, uint8_t& outVoice)
    {
        auto alloc = this->voiceAllocator;
        outVoice = this->channelStatuses[channel].voice;
        if (outVoice != 255) {
            auto voiceStatus = alloc->getVoiceStatus(outVoice);
            if (voiceStatus->channel == channel && alloc->isVoiceEnabled(outVoice)) {
                return true;
            }
        }
        return false;
    }
    bool reserveVoice(uint8_t channel, uint8_t& outVoice, Tone& tone)
    {
        if (this->getVoice(channel, outVoice)) {
            if (this->voiceAllocator->isVoiceCompatible(outVoice, tone)) {
                this->voiceAllocator->setPriority(outVoice, PRIORITY_ACTIVE_MONO);
                return true;
            } else {
                // The current voice cannot play the current tone - release & reserve a compatible voice
                this->voiceAllocator->release(outVoice);
            }
        }

        return this->voiceAllocator->reserve(outVoice, VoiceStatus(channel, 0), tone, PRIORITY_ACTIVE_MONO);
    }
    void stopNote(uint8_t channel, uint8_t voice)
    {
        if (this->voiceAllocator->getChannel(voice) == channel) {
            auto& status = this->channelStatuses[channel];
            this->toneController->noteOff(voice);
            this->voiceAllocator->release(voice);
            status.setNotePlaying(false);
        }
    }
    void updateNote(uint8_t channel)
    {
        auto& status = this->channelStatuses[channel];
        if (status.nextNoteIndex == 0) {
            if (!status.isSustainActive()) {
                this->stopNote(channel, status.voice);
            }
        } else {
            uint8_t noteNum = status.notes[status.nextNoteIndex - 1];
            if (noteNum != status.targetPitch) {
                status.initialPitch = status.currentPitch;
                status.targetPitch = noteNum;
                status.glideStartTime = this->timeSource->timeMS;
                status.glideDuration = this->toneController->getChannelPatch(channel)->params.glideDurationMS;
                this->updateChannelPitch(channel);
            }
        }
    }
    float getChannelPitch(uint8_t channel)
    {
        auto& status = this->channelStatuses[channel];
        auto timeSinceGlideStart = this->timeSource->timeMS - status.glideStartTime;
        if (timeSinceGlideStart >= status.glideDuration) {
            return status.targetPitch;
        } else if (timeSinceGlideStart <= 0) {
            return status.initialPitch;
        } else {
            return (timeSinceGlideStart / (float)status.glideDuration)
                * (status.targetPitch - status.initialPitch)
                + status.initialPitch;
        }
    }
    void updateChannelPitch(uint8_t channel)
    {
        auto& status = this->channelStatuses[channel];
        float prevPitch = status.currentPitch;
        float newPitch = this->getChannelPitch(channel);
        status.currentPitch = newPitch;

        uint8_t voice;
        if (this->getVoice(channel, voice) && prevPitch != newPitch) {
            this->toneController->setPitch(channel, voice, newPitch);
        }
    }
};
