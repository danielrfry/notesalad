#include "OPLToneGenerator.h"
#include "../../OPL/OPLUtils.h"

#include <algorithm>

#define VOICE_STATE_INACTIVE 0
#define VOICE_STATE_KEYON 1
#define VOICE_STATE_RELEASE 2

OPLToneGenerator::OPLToneGenerator(OPLReadWriteDeviceBase* device, TimeSource* timeSource) : device(device), timeSource(timeSource)
{
    this->resetState();
}

void OPLToneGenerator::reset(bool hardReset)
{
    this->resetState();
    if (hardReset) {
        this->device->hardReset();
        this->device->write(0x105, 0x01);
    } else {
        this->device->softReset();
    }
}

void OPLToneGenerator::resetState()
{
    for (uint8_t voice = 0; voice < 24; voice++) {
        this->voiceTones[voice] = OPLTone();
        this->voiceVolume[voice] = 127;
        this->voicePan[voice] = 63;
        this->voiceReleaseTime[voice] = 0;
        this->voiceStates[voice] = VOICE_STATE_INACTIVE;
    }
}

void OPLToneGenerator::setTone(uint8_t voice, OPLTone& tone, uint8_t volume, uint8_t pan)
{
    if (tone.is4Op() != OPL::isVoice4Op(voice)) {
        return;
    }

    bool inRelease = this->voiceStates[voice] == VOICE_STATE_RELEASE;
    if (inRelease) {
        auto oldReleaseRate = this->getReleaseRate(this->voiceTones[voice]);
        auto newReleaseRate = this->getReleaseRate(tone);
        if (oldReleaseRate != newReleaseRate) {
            this->voiceReleaseTime[voice] = this->getReleaseTime(newReleaseRate);
        }
    }

    this->voiceTones[voice] = tone;
    this->voiceVolume[voice] = volume;
    this->voicePan[voice] = pan;
    this->updateTone(voice);
}

void OPLToneGenerator::updateTone(uint8_t voice)
{
    if (!this->device->isVoiceEnabled(voice))
        return;

    OPLTone adjustedTone = this->voiceTones[voice];
    adjustedTone.applyVolume(this->voiceVolume[voice]);
    adjustedTone.applyPan(this->voicePan[voice]);
    this->device->writeTone(voice, adjustedTone);
}

void OPLToneGenerator::setPitch(uint8_t voice, float note)
{
    if (!this->device->isVoiceEnabled(voice))
        return;

    uint8_t channel = OPL::getVoice2OpChannel(voice);
    uint16_t offset = OPL::get2OpChannelRegOffset(channel);
    bool keyOn = (this->device->read(offset + 0xb0) & 0x20) != 0;
    this->setPitch(voice, note, keyOn);
}

void OPLToneGenerator::noteOn(uint8_t voice, float note)
{
    this->device->enableVoice(voice);
    this->noteOff(voice);
    this->updateTone(voice);
    this->setPitch(voice, note, true);
    this->voiceReleaseTime[voice] = 0;
    this->voiceStates[voice] = VOICE_STATE_KEYON;
}

void OPLToneGenerator::noteOff(uint8_t voice)
{
    if (!this->device->isVoiceEnabled(voice))
        return;

    this->device->setVoiceKeyOn(voice, false);
    this->voiceReleaseTime[voice] = this->getReleaseTime(this->getReleaseRate(this->voiceTones[voice]));
    this->voiceStates[voice] = VOICE_STATE_RELEASE;
}

bool OPLToneGenerator::isNoteActive(uint8_t voice)
{
    auto& voiceState = this->voiceStates[voice];

    if (!this->device->isVoiceEnabled(voice)) {
        voiceState = VOICE_STATE_INACTIVE;
        return false;
    } else if (voiceState == VOICE_STATE_KEYON) {
        return true;
    } else if (voiceState == VOICE_STATE_INACTIVE) {
        return false;
    } else if (this->voiceReleaseTime[voice] == 0) {
        return true;
    } else if (this->timeSource->timeMS >= this->voiceReleaseTime[voice]) {
        this->voiceStates[voice] = VOICE_STATE_INACTIVE;
        return false;
    } else {
        return true;
    }
}

void OPLToneGenerator::setPitch(uint8_t voice, float note, bool keyOn)
{
    uint8_t block;
    uint16_t fNum;
    OPL::getBlockFNum(note, block, fNum);
    uint8_t konBlockFNumH = (keyOn ? 0x20 : 0x00) | ((block & 0x07) << 2) | ((fNum & 0x300) >> 8);
    uint8_t fnumL = (fNum & 0xff);

    uint8_t channel = OPL::getVoice2OpChannel(voice);
    uint16_t offset = OPL::get2OpChannelRegOffset(channel);
    this->device->write(offset + 0xa0, fnumL);
    this->device->write(offset + 0xb0, konBlockFNumH);
}

void OPLToneGenerator::setVolume(uint8_t voice, uint8_t volume)
{
    this->voiceVolume[voice] = volume;
    this->updateTone(voice);
}

void OPLToneGenerator::setPan(uint8_t voice, uint8_t pan)
{
    this->voicePan[voice] = pan;
    this->updateTone(voice);
}

Milliseconds OPLToneGenerator::getReleaseDuration(uint8_t releaseRate)
{
    if (releaseRate == 0) {
        return MILLISECONDS_MAX;
    } else if (releaseRate < 12) {
        return 25 * (1 << (11 - releaseRate));
    } else {
        switch (releaseRate) {
        case 12:
            return 13;
        case 13:
            return 7;
        case 14:
            return 4;
        case 15:
            return 2;
        default:
            return 0;
        }
    }
}

Milliseconds OPLToneGenerator::getReleaseRate(OPLTone& tone)
{
    uint8_t carriers[3];
    uint8_t numCarriers;
    numCarriers = tone.getCarriers(carriers);
    uint8_t rate = 15;
    for (uint8_t i = 0; i < numCarriers; i++) {
        uint8_t opRate = tone.getRR(carriers[i]);
        rate = std::min(rate, opRate);
    }
    return rate;
}

Milliseconds OPLToneGenerator::getReleaseTime(uint8_t releaseRate)
{
    if (releaseRate == 0) {
        return 0;
    } else {
        return this->timeSource->timeMS + this->getReleaseDuration(releaseRate);
    }
}