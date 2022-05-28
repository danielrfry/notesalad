#include "OPLSD1ToneGenerator.h"
#include "../../SD1/SD1Utils.h"

OPLSD1ToneGenerator::OPLSD1ToneGenerator(SD1DeviceBase* device) : sd1ToneGenerator(device)
{
}

void OPLSD1ToneGenerator::reset(bool hardReset)
{
    this->sd1ToneGenerator.reset(hardReset);
}

void OPLSD1ToneGenerator::setTone(uint8_t voice, OPLTone& tone, uint8_t volume, uint8_t pan)
{
    this->voiceTones[voice] = tone;
    this->voiceVolumes[voice] = volume;
    this->updateTone(voice);
}

void OPLSD1ToneGenerator::setPitch(uint8_t voice, float note)
{
    this->sd1ToneGenerator.setPitch(voice, note);
}

void OPLSD1ToneGenerator::setVolume(uint8_t voice, uint8_t volume)
{
    this->voiceVolumes[voice] = volume;
    this->updateTone(voice);
}

void OPLSD1ToneGenerator::setPan(uint8_t voice, uint8_t pan)
{
    this->sd1ToneGenerator.setPan(voice, pan);
}

void OPLSD1ToneGenerator::noteOn(uint8_t voice, float note)
{
    this->sd1ToneGenerator.noteOn(voice, note);
}

void OPLSD1ToneGenerator::noteOff(uint8_t voice)
{
    this->sd1ToneGenerator.noteOff(voice);
}

void OPLSD1ToneGenerator::updateTone(uint8_t voice)
{
    OPLTone tone = this->voiceTones[voice];
    tone.applyVolume(this->voiceVolumes[voice]);
    SD1Tone sd1Tone;
    SD1::convertOPLTone(tone, sd1Tone);
    this->sd1ToneGenerator.setTone(voice, sd1Tone, 127, 64);
}
