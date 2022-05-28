#include "OPMToneGenerator.h"
#include "../../OPM/OPMUtils.h"

#include <cmath>

OPMToneGenerator::OPMToneGenerator(BufferedOPMDevice* device) : device(device)
{
    this->resetState();
}

void OPMToneGenerator::reset(bool hardReset)
{
    this->resetState();
    if (hardReset) {
        this->device->hardReset();
    } else {
        // TODO: Soft reset
        this->device->hardReset();
    }
}

void OPMToneGenerator::resetState()
{
    for (uint8_t v = 0; v < 8; v++) {
        this->voiceTones[v] = OPMTone();
        this->voiceVolume[v] = 127;
        this->voicePan[v] = 63;
    }
}

void OPMToneGenerator::setTone(uint8_t voice, OPMTone& tone, uint8_t volume, uint8_t pan)
{
    this->voiceTones[voice] = tone;
    this->voiceVolume[voice] = volume;
    this->voicePan[voice] = pan;
    this->updateTone(voice);
}

void OPMToneGenerator::setPitch(uint8_t voice, float note)
{
    uint8_t intNote = (uint8_t)floor(note);
    uint8_t fraction = (uint8_t)floor((note - intNote) * 64);
    this->device->write(0x28 + voice, OPM::getKCForMIDINote(intNote));
    this->device->write(0x30 + voice, fraction << 2);
}

void OPMToneGenerator::setVolume(uint8_t voice, uint8_t volume)
{
    this->voiceVolume[voice] = volume;
    this->updateTone(voice);
}

void OPMToneGenerator::setPan(uint8_t voice, uint8_t pan)
{
    this->voicePan[voice] = pan;
    this->updateTone(voice);
}

void OPMToneGenerator::noteOn(uint8_t voice, float note)
{
    this->setPitch(voice, note);
    uint8_t snch = (this->voiceTones[voice].getOpsEnabled() << 3) | (voice & 0x07);
    this->device->write(0x08, snch);
}

void OPMToneGenerator::noteOff(uint8_t voice)
{
    this->device->write(0x08, voice & 0x07);
}

void OPMToneGenerator::writeTone(uint8_t voice, OPMTone& tone)
{
    this->device->write(0x20 + voice, tone.toneData.rl_fb_conect);
    this->device->write(0x38 + voice, tone.toneData.pms_ams);
    for (uint8_t op = 0; op < 4; op++) {
        uint8_t opOffset = (op * 8) + voice;
        OPMOperatorData& opData = tone.toneData.op[op];
        this->device->write(0x40 + opOffset, opData.dt1_mul);
        this->device->write(0x60 + opOffset, opData.tl);
        this->device->write(0x80 + opOffset, opData.ks_ar);
        this->device->write(0xa0 + opOffset, opData.amsen_d1r);
        this->device->write(0xc0 + opOffset, opData.dt2_d2r);
        this->device->write(0xe0 + opOffset, opData.d1l_rr);
    }
}

void OPMToneGenerator::updateTone(uint8_t voice)
{
    OPMTone tone = this->voiceTones[voice];
    tone.applyVolume(this->voiceVolume[voice]);
    tone.applyPan(this->voicePan[voice]);
    this->writeTone(voice, tone);
}