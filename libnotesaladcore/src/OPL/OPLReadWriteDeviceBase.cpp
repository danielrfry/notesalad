#include "OPLReadWriteDeviceBase.h"
#include "OPLTone.h"
#include "OPLUtils.h"

bool OPLReadWriteDeviceBase::isVoiceEnabled(uint8_t voice)
{
    if ((voice >= 6 && voice <= 8) || (voice >= 15 && voice <= 17)) {
        // These voices cannot be assigned to 4-op channels and are therefore always enabled
        return true;
    } else {
        uint8_t ch4Op = OPL::getVoice4OpChannel(voice);
        return ch4Op != 255 && (OPL::isVoice4Op(voice) == this->is4OpChannelEnabled(ch4Op));
    }
}

bool OPLReadWriteDeviceBase::is4OpChannelEnabled(uint8_t ch4Op)
{
    if (ch4Op < 6) {
        uint8_t bit = (1 << ch4Op);
        uint8_t value = this->read(0x104);
        return (value & bit) != 0;
    } else {
        return false;
    }
}

void OPLReadWriteDeviceBase::set4OpChannelEnabled(uint8_t ch4Op, bool enabled)
{
    if (ch4Op < 6) {
        uint8_t bit = (1 << ch4Op);
        uint8_t value = this->read(0x104);
        if (enabled) {
            value |= bit;
        } else {
            value &= ~bit;
        }
        this->write(0x104, value);
    }
}

void OPLReadWriteDeviceBase::setChannelKeyOn(uint8_t ch2Op, bool keyOn)
{
    uint16_t reg = OPL::get2OpChannelRegOffset(ch2Op) + 0xb0;
    uint8_t value = this->read(reg);
    if (keyOn) {
        value |= 0x20;
    } else {
        value &= ~0x20;
    }
    this->write(reg, value);
}

void OPLReadWriteDeviceBase::readTone(uint8_t voice, OPLTone& tone)
{
    uint16_t chRegOffsets[2];
    uint8_t numChannels;
    OPL::getChannelRegOffsetsForVoice(voice, &chRegOffsets[0], numChannels);
    uint16_t opRegOffsets[4];
    uint8_t numOperators;
    OPL::getOperatorRegOffsetsForVoice(voice, &opRegOffsets[0], numOperators);

    bool opl3Enabled = this->read(0x105) & 1;

    tone = OPLTone();
    if (numOperators == 4) {
        tone.toneData.flags = OPL_TONE_FLAG_4OP;
    } else {
        tone.toneData.flags = 0;
    }

    for (uint8_t ch = 0; ch < numChannels; ch++) {
        tone.toneData.fbcnt[ch] = this->read(0xc0 + chRegOffsets[ch]) & 0x0f;
    }
    for (uint8_t op = 0; op < numOperators; op++) {
        OPLOperatorData& opData = tone.toneData.op[op];
        opData.avekm = this->read(0x20 + opRegOffsets[op]);
        opData.ksl_tl = this->read(0x40 + opRegOffsets[op]);
        opData.ar_dr = this->read(0x60 + opRegOffsets[op]);
        opData.sl_rr = this->read(0x80 + opRegOffsets[op]);
        opData.ws = this->read(0xe0 + opRegOffsets[op]) & (opl3Enabled ? 0x07 : 0x03);
    }
}

void OPLReadWriteDeviceBase::writeTone(uint8_t voice, OPLTone& tone)
{
    uint8_t channels[2];
    uint8_t numChannels;
    OPL::getVoice2OpChannels(voice, &channels[0], numChannels);

    for (uint8_t c = 0; c < numChannels; c++) {
        uint16_t regOffsets[2];
        OPL::get2OpRegOffsets(channels[c], &regOffsets[0]);
        for (uint8_t op = 0; op < 2; op++) {
            OPLOperatorData& opData = tone.toneData.op[(c * 2) + op];
            uint16_t offset = regOffsets[op];
            this->write(0x20 + offset, opData.avekm);
            this->write(0x40 + offset, opData.ksl_tl);
            this->write(0x60 + offset, opData.ar_dr);
            this->write(0x80 + offset, opData.sl_rr);
            this->write(0xe0 + offset, opData.ws);
        }

        this->write(0xc0 + OPL::get2OpChannelRegOffset(channels[c]), tone.toneData.fbcnt[c]);
    }
}

void OPLReadWriteDeviceBase::enableVoice(uint8_t voice)
{
    if (!this->isVoiceEnabled(voice)) {
        uint8_t ch4Op = OPL::getVoice4OpChannel(voice);
        uint8_t channels[2];
        OPL::get2OpChannelsFor4OpChannel(ch4Op, &channels[0]);
        this->setChannelKeyOn(channels[0], false);
        this->setChannelKeyOn(channels[1], false);

        this->set4OpChannelEnabled(ch4Op, OPL::isVoice4Op(voice));
    }
}

void OPLReadWriteDeviceBase::setVoiceKeyOn(uint8_t voice, bool keyOn)
{
    if (!this->isVoiceEnabled(voice))
        return;

    if (keyOn) {
        this->setChannelKeyOn(OPL::getVoice2OpChannel(voice), true);
    } else {
        uint8_t channels[2];
        uint8_t numChannels;
        OPL::getVoice2OpChannels(voice, &channels[0], numChannels);
        for (uint8_t i = 0; i < numChannels; i++) {
            uint8_t ch2Op = channels[i];
            this->setChannelKeyOn(ch2Op, false);
        }
    }
}

void OPLReadWriteDeviceBase::allNotesOff()
{
    for (uint8_t ch = 0; ch < 18; ch++) {
        uint16_t offset = OPL::get2OpChannelRegOffset(ch);
        uint8_t value = this->read(0xb0 + offset);
        this->write(0xb0 + offset, value & 0xdf);
    }
}

void OPLReadWriteDeviceBase::setMinimumReleaseRate(uint8_t rr)
{
    for (uint8_t v = 0; v < OPL_NUM_VOICES; v++) {
        if (this->isVoiceEnabled(v)) {
            uint8_t carriers[4];
            uint8_t numCarriers;
            OPLTone tone;
            this->readTone(v, tone);
            numCarriers = tone.getCarriers(carriers);
            for (uint8_t i = 0; i < numCarriers; i++) {
                uint8_t opRR = tone.getRR(carriers[i]);
                if (opRR < rr) {
                    tone.setRR(carriers[i], rr);
                }
            }
            this->writeTone(v, tone);
        }
    }
}

void OPLReadWriteDeviceBase::pauseEnvelope(uint8_t voice)
{
    if (this->isVoiceEnabled(voice)) {
        uint16_t offsets[4];
        uint8_t numOffsets;
        OPL::getOperatorRegOffsetsForVoice(voice, offsets, numOffsets);
        for (uint8_t i = 0; i < numOffsets; i++) {
            uint8_t offset = offsets[i];
            this->write(0x60 + offset, 0);
            this->write(0x80 + offset, this->read(0x80 + offset) & 0xf0);
        }
    }
}

void OPLReadWriteDeviceBase::pauseEnvelope()
{
    for (uint8_t v = 0; v < OPL_NUM_VOICES; v++) {
        this->pauseEnvelope(v);
    }
}

void OPLReadWriteDeviceBase::copyTo(OPLDeviceBase& dest)
{
    dest.write(0x105, this->read(0x105));
    dest.write(0x08, this->read(0x08));

    for (uint8_t i = 0; i <= 0x15; i++) {
        dest.write(0x20 + i, this->read(0x20 + i));
        dest.write(0x40 + i, this->read(0x40 + i));
        dest.write(0x60 + i, this->read(0x60 + i));
        dest.write(0x80 + i, this->read(0x80 + i));
        dest.write(0xe0 + i, this->read(0xe0 + i));
        dest.write(0x120 + i, this->read(0x120 + i));
        dest.write(0x140 + i, this->read(0x140 + i));
        dest.write(0x160 + i, this->read(0x160 + i));
        dest.write(0x180 + i, this->read(0x180 + i));
        dest.write(0x1e0 + i, this->read(0x1e0 + i));
    }

    for (uint8_t i = 0; i < 0x08; i++) {
        dest.write(0xc0 + i, this->read(0xc0 + i));
        dest.write(0xa0 + i, this->read(0xa0 + i));
        dest.write(0xb0 + i, this->read(0xb0 + i));
        dest.write(0x1c0 + i, this->read(0x1c0 + i));
        dest.write(0x1a0 + i, this->read(0x1a0 + i));
        dest.write(0x1b0 + i, this->read(0x1b0 + i));
    }

    dest.write(0xbd, this->read(0xbd));
}