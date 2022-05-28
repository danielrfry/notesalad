#pragma once

#include <cstdint>

#include "IToneGenerator.h"
#include "IVoiceAllocator.h"
#include "MIDICommon.h"
#include "ToneController.h"

typedef struct PolyNoteInfo {
    float pitch; // The pitch of the note before pitch wheel is applied
    NoteStatus noteStatus;
} PolyNoteInfo;

typedef struct PolyChannelStatus {
    bool sustainActive;
} PolyChannelStatus;

template <class Tone, uint8_t voices>
class PolyNoteManager : public INoteManager {
public:
    PolyNoteManager(IVoiceAllocator<Tone>* voiceAllocator,
        ToneController<Tone, voices>* toneController)
        : voiceAllocator(voiceAllocator),
          toneController(toneController)
    {
        this->resetState();
    }
    virtual void resetState()
    {
        for (uint8_t c = 0; c < 16; c++) {
            auto& status = this->channelStatuses[c];
            status.sustainActive = false;
        }
        for (uint8_t v = 0; v < voices; v++) {
            auto& noteInfo = this->voiceNoteInfo[v];
            noteInfo.pitch = 0.0f;
            noteInfo.noteStatus = INACTIVE;
        }
    }
    virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        this->noteOff(channel, note, velocity);

        Patch<Tone> patch;
        if (!this->toneController->getNotePatch(channel, note, patch))
            return;

        uint8_t voice;
        if (this->voiceAllocator->reserve(voice, VoiceStatus(channel, note), patch.params.tone, PRIORITY_ACTIVE)) {
            uint8_t fixedNoteNum = patch.params.fixedNoteNum;
            float pitch = fixedNoteNum == 0 ? note : fixedNoteNum;
            PolyNoteInfo newNoteInfo;
            newNoteInfo.pitch = pitch;
            newNoteInfo.noteStatus = ACTIVE;
            this->voiceNoteInfo[voice] = newNoteInfo;
            this->toneController->noteOn(channel, voice, note, pitch, velocity);
        }
    }
    virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        this->voiceAllocator->applyToChannelVoices(channel, [this, channel, note](uint8_t v, const VoiceStatus* voiceStatus) {
            auto& noteInfo = this->voiceNoteInfo[v];
            if ((voiceStatus->noteNum == note) && (noteInfo.noteStatus != INACTIVE)) {
                if (this->channelStatuses[channel].sustainActive) {
                    if (noteInfo.noteStatus == ACTIVE) {
                        this->voiceAllocator->setPriority(v, PRIORITY_SUSTAINED);
                        noteInfo.noteStatus = SUSTAINED;
                    }
                } else {
                    this->voiceOff(v);
                }
            }
        });
    }
    virtual void setSustain(uint8_t channel, bool sustainActive)
    {
        auto& status = this->channelStatuses[channel];
        status.sustainActive = sustainActive;
        if (!sustainActive) {
            this->clearSustain(channel);
        }
    }
    virtual void allNotesOff(uint8_t channel)
    {
        this->voiceAllocator->applyToChannelVoices(channel, [this](uint8_t v, const VoiceStatus* voiceStatus) {
            this->voiceOff(v);
        });
    }

private:
    IVoiceAllocator<Tone>* voiceAllocator;
    ToneController<Tone, voices>* toneController;
    PolyChannelStatus channelStatuses[16];
    PolyNoteInfo voiceNoteInfo[voices];
    void clearSustain(uint8_t channel)
    {
        this->voiceAllocator->applyToChannelVoices(channel, [this](uint8_t v) {
            auto& noteInfo = this->voiceNoteInfo[v];
            if (noteInfo.noteStatus == SUSTAINED) {
                this->voiceOff(v);
            }
        });
    }
    void voiceOff(uint8_t voice)
    {
        this->voiceAllocator->release(voice);
        this->toneController->noteOff(voice);
        this->voiceNoteInfo[voice].noteStatus = INACTIVE;
    }
};
