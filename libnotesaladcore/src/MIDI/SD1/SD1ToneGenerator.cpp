#include "SD1ToneGenerator.h"
#include "../../Utils.h"

#include <algorithm>
#include <cmath>

static const uint16_t fnum_data[] = {
    179, 190, 201, 212, 225, 238, 253, 267, 284, 301, 319, 336, // For notes 0-11
    357, 378, 401, 425, 450, 477, 505, 535, 567, 601, 636, 674, // For notes 12-107
    714, 757, 802, 850, 900, 954, 1010 // For notes 108-114
};

SD1ToneGenerator::SD1ToneGenerator(SD1DeviceBase* device) : device(device)
{
    for (uint8_t v = 0; v < 16; v++) {
        this->voiceVolumes[v] = 0;
        this->voiceNotes[v] = -1.0f;
        this->setVolume(v, 127);
        this->device->writeReg(0x0b, v);
        this->device->writeReg(0x10, 0x31); // ChVol
        this->device->writeReg(0x11, 0x01); // XVB
        this->device->writeReg(0x12, 0x08); // INT/FRAC
    }
}

void SD1ToneGenerator::reset(bool hardReset)
{
    // TODO: Implement reset for SD-1
}

void SD1ToneGenerator::setTone(uint8_t voice, SD1Tone& tone, uint8_t volume, uint8_t pan)
{
    if (this->voiceTones[voice] != tone) {
        this->voiceTones[voice] = tone;
        this->writeTones(voice);
    }
    this->setVolume(voice, volume);
}

static void getBlockFNum(float note, uint8_t& block, uint16_t& fnum)
{
    note = std::max(0.0f, std::min(note, 114.0f));
    uint8_t intNote = (uint8_t)note;
    float frac = note - intNote;
    uint8_t freqIndex;

    if (intNote < 12) {
        block = 0;
        freqIndex = intNote;
    } else if (intNote < 108) {
        block = (intNote - 12) / 12;
        freqIndex = (intNote % 12) + 12;
    } else {
        block = 7;
        freqIndex = (intNote % 12) + 24;
    }

    if (frac == 0.0f) {
        fnum = fnum_data[freqIndex];
    } else {
        uint16_t fnum1 = fnum_data[freqIndex];
        uint16_t fnum2 = fnum_data[freqIndex + 1];
        fnum = fnum1 + (uint16_t)lround((fnum2 - fnum1) * frac);
    }
}

void SD1ToneGenerator::setPitch(uint8_t voice, float note)
{
    if ((note < 0) || (note > 114.0f))
        return;

    if (this->voiceNotes[voice] != note) {
        this->voiceNotes[voice] = note;
        this->writePitch(voice, note);
    }
}

void SD1ToneGenerator::setVolume(uint8_t voice, uint8_t volume)
{
    if (this->voiceVolumes[voice] != volume) {
        this->voiceVolumes[voice] = volume;

        uint8_t fmVol = (volume * 31) / 127;
        this->device->writeReg(0x0b, voice & 0x0f); // Select voice
        this->device->writeReg(0x0c, fmVol << 2); // Set volume
    }
}

void SD1ToneGenerator::setPan(uint8_t voice, uint8_t pan)
{
    // SD-1 is mono
}

void SD1ToneGenerator::noteOn(uint8_t voice, float note)
{
    this->device->writeReg(0x0b, voice & 0x0f);
    this->device->writeReg(0x0f, 0x10 | voice);
    this->writePitch(voice, note);
    this->device->writeReg(0x0f, 0x40 | voice);
}

void SD1ToneGenerator::noteOff(uint8_t voice)
{
    this->device->writeReg(0x0b, voice & 0x0f);
    this->device->writeReg(0x0f, 0x00 | voice);
}

void SD1ToneGenerator::writeTones(uint8_t lastVoice)
{
    uint8_t buffer[486];
    buffer[0] = 0x07;
    buffer[1] = 0x81 + lastVoice;
    uint16_t offset = 2;
    for (uint8_t ch = 0; ch <= lastVoice; ch++) {
        SD1ToneData* toneData = (SD1ToneData*)((void*)&buffer[offset]);
        *toneData = this->voiceTones[ch].toneData;
        offset += sizeof(SD1ToneData);
    }
    buffer[offset++] = 0x80;
    buffer[offset++] = 0x03;
    buffer[offset++] = 0x81;
    buffer[offset++] = 0x80;

    this->device->writeReg(0x08, 0x16);
    this->device->delayMicroseconds(7);
    this->device->writeReg(0x08, 0x00);
    this->device->write(&buffer[0], offset);
}

void SD1ToneGenerator::writePitch(uint8_t voice, float note)
{
    uint8_t block;
    uint16_t fnum;
    getBlockFNum(note, block, fnum);

    this->device->writeReg(0x0b, voice & 0x0f); // Select voice
    // Set block/fnum
    this->device->writeReg(0x0d, ((fnum & 0x380) >> 4) | (block & 0x07));
    this->device->writeReg(0x0e, fnum & 0x7f);
}

void SD1ToneGenerator::setAnalogueGain(uint8_t gain)
{
    gain = clamp(gain, (uint8_t)0, (uint8_t)3);
    this->device->writeReg(0x03, gain); // Analog Gain
}

void SD1ToneGenerator::setMasterVolume(uint8_t vol)
{
    vol = clamp(vol, (uint8_t)0, (uint8_t)63);
    this->device->writeReg(0x19, vol << 2);
}

void SD1ToneGenerator::setMasterChannelVolume(uint8_t vol)
{
    vol = clamp(vol, (uint8_t)0, (uint8_t)31);
    for (uint8_t v = 0; v < 16; v++) {
        this->device->writeReg(0x0b, v);
        this->device->writeReg(0x10, 0x31); // ChVol
    }
}

void SD1ToneGenerator::setGlobalParam(uint16_t paramID, uint16_t value)
{
    switch (paramID) {
    case GLOBAL_PARAM_SD1_ANALOGUE_GAIN:
        this->setAnalogueGain(value);
        break;
    case GLOBAL_PARAM_SD1_MASTER_VOL:
        this->setMasterVolume(value);
        break;
    case GLOBAL_PARAM_SD1_MASTER_CH_VOL:
        this->setMasterChannelVolume(value);
        break;
    }
}