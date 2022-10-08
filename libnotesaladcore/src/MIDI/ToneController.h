#pragma once

#include "IToneGenerator.h"
#include "IVoiceAllocator.h"
#include "MIDICommon.h"
#include "Patch.h"
#include "PatchManagerBase.h"
#include "TimeSource.h"

#include <cstdio>

typedef struct ToneControllerChannelStatus {
    uint8_t bank = 0;
    uint8_t program = 0;
    uint8_t volume = 127;
    uint8_t pan = 63;
    uint16_t pitchWheelValue = 0x2000;
    uint16_t iPitchWheelRange = 0x100;
    float fPitchWheelRange = 2.0f;
    LFO lfos[NUM_LFOS];
} ToneControllerChannelStatus;

typedef struct ToneControllerVoiceStatus {
    uint8_t noteNum = 0;
    uint8_t velocity = 127;
    uint8_t noteVolume = 127;
    float pitch = 0;
    Milliseconds noteStartTime = 0;
    LFO lfos[NUM_LFOS];
} ToneControllerVoiceStatus;

template <class Tone, uint8_t voices>
class ToneController {
public:
    ToneController(IVoiceAllocator<Tone>* voiceAllocator,
        IToneGenerator<Tone>* toneGenerator,
        PatchManagerBase<Tone>* patchManager,
        TimeSource* timeSource)
        : voiceAllocator(voiceAllocator),
          toneGenerator(toneGenerator),
          patchManager(patchManager),
          timeSource(timeSource)
    {
        this->resetState();
    }
    void resetState()
    {
        for (uint8_t c = 0; c < 16; c++) {
            this->channelStatus[c] = ToneControllerChannelStatus();
            this->patchManager->getPatch(0, 0, 0xff, this->channelPatches[c]);
            this->clearParamMapCCValues(c);
        }
        for (uint8_t v = 0; v < voices; v++) {
            this->voiceStatus[v] = ToneControllerVoiceStatus();
        }
    }
    bool getNotePatch(uint8_t channel, uint8_t noteNum, Patch<Tone>& outPatch)
    {
        if (channel == DRUM_CHANNEL && this->patchManager->supportsDrums()) {
            auto& channelStatus = this->channelStatus[channel];
            return this->patchManager->getPatch(channelStatus.bank, channelStatus.program, noteNum, outPatch);
        } else {
            outPatch = this->channelPatches[channel];
            return true;
        }
    }
    Patch<Tone>* getChannelPatch(uint8_t channel)
    {
        return &this->channelPatches[channel];
    }
    uint8_t getChannelPolyMode(uint8_t channel)
    {
        if (channel == DRUM_CHANNEL) {
            return POLYMODE_POLY;
        } else {
            return this->channelPatches[channel].params.polyMode;
        }
    }
    void noteOn(uint8_t channel, uint8_t voice, uint8_t noteNum, float pitch, uint8_t velocity)
    {
        auto& voiceStatus = this->voiceStatus[voice];
        voiceStatus.noteNum = noteNum;
        voiceStatus.velocity = velocity;
        voiceStatus.pitch = pitch;
        voiceStatus.noteStartTime = this->timeSource->timeMS;
        this->resetVoiceLFOs(voice);
        auto patchParams = this->getVoiceParams(channel, voice);
        voiceStatus.noteVolume = this->getNoteVolume(velocity, patchParams.velocityDepth);
        this->updateVoiceTone(channel, voice, patchParams);
        pitch = this->getVoicePitch(channel, voice, patchParams);
        this->toneGenerator->noteOn(voice, pitch);
    }
    void setPitch(uint8_t channel, uint8_t voice, float pitch)
    {
        auto& status = this->voiceStatus[voice];
        status.pitch = pitch;
    }
    void noteOff(uint8_t voice)
    {
        this->toneGenerator->noteOff(voice);
    }
    void setBank(uint8_t channel, uint8_t bank)
    {
        auto& status = this->channelStatus[channel];
        status.bank = bank;
        this->clearParamMapCCValues(channel);
        this->patchManager->getPatch(status.bank, status.program, 0xff, this->channelPatches[channel]);
    }
    void setProgram(uint8_t channel, uint8_t program)
    {
        auto& status = this->channelStatus[channel];
        status.program = program;
        this->clearParamMapCCValues(channel);
        this->patchManager->getPatch(status.bank, status.program, 0xff, this->channelPatches[channel]);
    }
    void setPatch(uint8_t channel, Patch<Tone>& patch)
    {
        this->clearParamMapCCValues(channel);
        this->channelPatches[channel] = patch;
    }
    void setVolume(uint8_t channel, uint8_t volume)
    {
        this->channelStatus[channel].volume = volume;
        this->voiceAllocator->applyToChannelVoices(channel, [this, channel](uint8_t v) {
            this->toneGenerator->setVolume(v, this->getVolumeForVoice(channel, v));
        });
    }
    void setPan(uint8_t channel, uint8_t pan)
    {
        this->channelStatus[channel].pan = pan;
        this->voiceAllocator->applyToChannelVoices(channel, [this, pan](uint8_t v) {
            this->toneGenerator->setPan(v, pan);
        });
    }
    void setPitchWheel(uint8_t channel, uint16_t pwValue)
    {
        this->channelStatus[channel].pitchWheelValue = pwValue;
    }
    void setPitchWheelRange(uint8_t channel, uint16_t pwrValue)
    {
        auto& status = this->channelStatus[channel];
        if (status.iPitchWheelRange != pwrValue) {
            status.iPitchWheelRange = pwrValue;
            status.fPitchWheelRange = ((pwrValue >> 7) & 0x7f) + ((pwrValue & 0x7f) / 100.0f);
        }
    }
    void setParam(uint8_t channel, uint16_t paramID, uint16_t value)
    {
        Patch<Tone>& patch = this->channelPatches[channel];
        patch.setParam(paramID, value);
    }
    uint16_t getParam(uint8_t channel, uint16_t paramID)
    {
        Patch<Tone>& patch = this->channelPatches[channel];
        return patch.getParam(paramID);
    }
    void setController(uint8_t channel, uint8_t controller, uint8_t value)
    {
        auto& patch = this->channelPatches[channel];
        uint8_t baseIndex = channel * NUM_PARAM_MAPS;
        for (uint8_t i = 0; i < NUM_PARAM_MAPS; i++) {
            if (patch.paramMaps[i].src == controller) {
                if (this->paramMapCCValues[baseIndex + i] != value) {
                    this->paramMapCCValues[baseIndex + i] = value;
                }
            }
        }
    }
    void update()
    {
        for (uint8_t ch = 0; ch < 16; ch++) {
            this->updateChannelParams(ch);
        }
    }

private:
    IVoiceAllocator<Tone>* voiceAllocator;
    IToneGenerator<Tone>* toneGenerator;
    PatchManagerBase<Tone>* patchManager;
    TimeSource* timeSource;
    ToneControllerChannelStatus channelStatus[16];
    Patch<Tone> channelPatches[16];
    ToneControllerVoiceStatus voiceStatus[voices];
    uint8_t paramMapCCValues[NUM_PARAM_MAPS * 16];

    uint8_t getVolumeForVoice(uint8_t channel, uint8_t voice)
    {
        uint8_t channelVolume = this->channelStatus[channel].volume;
        uint8_t noteVolume = this->voiceStatus[voice].noteVolume;
        return (channelVolume * noteVolume) / 127;
    }

    PatchParams<Tone> getVoiceParams(uint8_t channel, uint8_t voice)
    {
        Patch<Tone> patch;
        if (channel == DRUM_CHANNEL && this->patchManager->supportsDrums()) {
            auto& voiceStatus = this->voiceStatus[voice];
            this->getNotePatch(channel, voiceStatus.noteNum, patch);
        } else {
            patch = this->channelPatches[channel];
        }
        this->applyParamMaps(channel, voice, patch);

        return patch.params;
    }

    void applyParamMaps(uint8_t channel, uint8_t voice, Patch<Tone>& patch)
    {
        auto& channelStatus = this->channelStatus[channel];
        auto& voiceStatus = this->voiceStatus[voice];

        for (uint8_t i = 0; i < NUM_PARAM_MAPS; i++) {
            auto& map = patch.paramMaps[i];
            if (map.src != PARAMMAP_SRC_NONE) {
                int32_t value = patch.getParam(map.destParam);
                float mapSrcValue = this->getMapSrcValue(channel, voice, patch, i);
                int32_t adjustment = (int32_t)(map.adjustmentAmount * mapSrcValue);
                value = clamp<int32_t>(value + adjustment, 0, UINT16_MAX);
                patch.setParam(map.destParam, value);
            }
        }

        for (uint8_t l = 0; l < NUM_LFOS; l++) {
            channelStatus.lfos[l].setWave(patch.lfoParams[l].wave);
            channelStatus.lfos[l].setOneShot(patch.lfoParams[l].oneShot);
            channelStatus.lfos[l].setPeriod(this->timeSource->timeMS, patch.lfoParams[l].period);
            voiceStatus.lfos[l].setWave(patch.lfoParams[l].wave);
            voiceStatus.lfos[l].setOneShot(patch.lfoParams[l].oneShot);
            voiceStatus.lfos[l].setPeriod(this->timeSource->timeMS - voiceStatus.noteStartTime,
                patch.lfoParams[l].period);
        }
    }

    float getMapSrcValue(uint8_t channel, uint8_t voice, Patch<Tone>& patch, uint8_t mapIndex)
    {
        auto& map = patch.paramMaps[mapIndex];

        if (map.src < 128) {
            return this->paramMapCCValues[(channel * NUM_PARAM_MAPS) + mapIndex] / 127.0f;
        }

        uint8_t lfoIndex = map.src - 128;
        if (lfoIndex < NUM_LFOS) {
            if (patch.lfoParams[lfoIndex].sync) {
                auto& voiceStatus = this->voiceStatus[voice];
                return voiceStatus.lfos[lfoIndex].getValue(this->timeSource->timeMS - voiceStatus.noteStartTime);
            } else {
                return this->channelStatus[channel].lfos[lfoIndex].getValue(this->timeSource->timeMS);
            }
        }

        return 0;
    }

    void updateVoiceTone(uint8_t channel, uint8_t voice, PatchParams<Tone>& patchParams)
    {
        auto volume = this->getVolumeForVoice(channel, voice);
        auto pan = this->channelStatus[channel].pan;
        this->toneGenerator->setTone(voice, patchParams.tone, volume, pan);
    }

    float getVoicePitch(uint8_t channel, uint8_t voice, PatchParams<Tone>& patchParams)
    {
        auto& voiceStatus = this->voiceStatus[voice];
        auto pitch = voiceStatus.pitch;
        if (patchParams.fixedNoteNum != 0) {
            pitch = patchParams.fixedNoteNum;
        }
        if (patchParams.pitchOffset != 0x2000) {
            pitch += PatchParamUtils::pitchOffsetFromNRPNValue(patchParams.pitchOffset);
        }
        auto& status = this->channelStatus[channel];
        pitch = this->applyPitchWheel(pitch, status.fPitchWheelRange, status.pitchWheelValue);

        return pitch;
    }

    void updateVoicePitch(uint8_t channel, uint8_t voice, PatchParams<Tone>& patchParams)
    {
        this->toneGenerator->setPitch(voice, this->getVoicePitch(channel, voice, patchParams));
    }

    void updateVoiceParams(uint8_t channel, uint8_t voice)
    {
        if (this->toneGenerator->isNoteActive(voice)) {
            auto patchParams = this->getVoiceParams(channel, voice);
            this->updateVoiceTone(channel, voice, patchParams);
            this->updateVoicePitch(channel, voice, patchParams);
        }
    }

    void updateChannelParams(uint8_t channel)
    {
        this->voiceAllocator->applyToChannelVoices(channel, [this, channel](uint8_t v) {
            this->updateVoiceParams(channel, v);
        });
    }

    float applyPitchWheel(float pitch, float pwSemitones, uint16_t pwValue)
    {
        if (pwValue == 0x2000) {
            return pitch;
        } else {
            float fPWValue = ((pwValue / (float)0x3fff) * 2.0f) - 1.0f;
            return pitch + (fPWValue * pwSemitones);
        }
    }

    void clearParamMapCCValues(uint8_t channel)
    {
        uint16_t base = NUM_PARAM_MAPS * channel;
        for (uint8_t i = 0; i < NUM_PARAM_MAPS; i++) {
            this->paramMapCCValues[base + i] = 0;
        }
    }

    void resetVoiceLFOs(uint8_t voice)
    {
        for (uint8_t i = 0; i < NUM_LFOS; i++) {
            this->voiceStatus[voice].lfos[i].reset();
        }
    }

    uint8_t getNoteVolume(uint8_t velocity, uint8_t velocityDepth)
    {
        return 127 - (((127 - velocity) * velocityDepth) / 127);
    }
};
