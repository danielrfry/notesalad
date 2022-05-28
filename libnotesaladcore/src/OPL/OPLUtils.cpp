#include "OPLUtils.h"

#include <algorithm>
#include <math.h>

#define MIDI_NOTES_LEN 43

static const uint16_t midiNotes[] = {
    0x0055, //  0: C-1
    0x005a, //  1: C#-1
    0x0060, //  2: D-1
    0x0065, //  3: D#-1
    0x006c, //  4: E-1
    0x0072, //  5: F-1
    0x0079, //  6: F#-1
    0x0080, //  7: G-1
    0x0088, //  8: G#-1
    0x0090, //  9: A-1
    0x0098, // 10: A#-1
    0x00a1, // 11: B-1
    0x00ab, // 12: C0
    0x00b5, // 13: C#0
    0x00c0, // 14: D0
    0x00cb, // 15: D#0
    0x00d8, // 16: E0
    0x00e4, // 17: F0
    0x00f2, // 18: F#0
    0x0100, // 19: G0
    0x0110, // 20: G#0
    0x0120, // 21: A0
    0x0131, // 22: A#0
    0x0143, // 23: B0
    0x0156, // 24: C1
    0x016b, // 25: C#1
    0x0180, // 26: D1
    0x0197, // 27: D#1
    0x01b0, // 28: E1
    0x01c9, // 29: F1
    0x01e4, // 30: F#1
    0x0201, // 31: G1
    0x0220, // 32: G#1
    0x0240, // 33: A1
    0x0263, // 34: A#1
    0x0287, // 35: B1
    0x02ad, // 36: C2
    0x02d6, // 37: C#2
    0x0301, // 38: D2
    0x032f, // 39: D#2
    0x0360, // 40: E2
    0x0393, // 41: F2
    0x03c9 // 42: F#2
    // 0x0403 // 43: G2
};

static const uint8_t midiVolumeCurve[] = {
    63, 61, 59, 57, 55, 53, 52, 50, 48, 47, 45, 44, 42, 41, 40, 38,
    37, 36, 35, 34, 33, 32, 30, 29, 29, 28, 27, 26, 25, 24, 23, 23,
    22, 21, 21, 20, 19, 19, 18, 17, 17, 16, 16, 15, 15, 14, 14, 13,
    13, 13, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8,
    8, 7, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
};

void OPL::get2OpChannelsFor4OpChannel(uint8_t ch4Op, uint8_t* ch2Op)
{
    ch2Op[0] = ((ch4Op / 3) * 9) + (ch4Op % 3);
    ch2Op[1] = ch2Op[0] + 3;
}

void OPL::get2OpRegOffsets(uint8_t ch2Op, uint16_t* regOffsets)
{
    uint16_t regBase = ch2Op >= 9 ? 0x100 : 0;
    ch2Op = ch2Op % 9;
    regBase += ((ch2Op / 3) * 8) + (ch2Op % 3);
    regOffsets[0] = regBase;
    regOffsets[1] = regBase + 3;
}

uint16_t OPL::get2OpChannelRegOffset(uint8_t ch2Op)
{
    return ch2Op > 8 ? (ch2Op % 9) | 0x100 : (ch2Op % 9);
}

uint8_t OPL::get4OpChannelFor2OpChannel(uint8_t ch2op)
{
    switch (ch2op) {
    case 0:
    case 3:
        return 0;
    case 1:
    case 4:
        return 1;
    case 2:
    case 5:
        return 2;
    case 9:
    case 12:
        return 3;
    case 10:
    case 13:
        return 4;
    case 11:
    case 14:
        return 5;
    default:
        return 255;
    }
}

static void getBlockFNumForIntNote(uint8_t note, uint8_t& block, uint16_t& fnum)
{
    if (note < MIDI_NOTES_LEN) {
        block = 1;
        fnum = *(midiNotes + note);
    } else {
        uint16_t index = (MIDI_NOTES_LEN - 12) + ((note - MIDI_NOTES_LEN) % 12);
        block = ((note - MIDI_NOTES_LEN) / 12) + 2;
        fnum = *(midiNotes + index);
    }
}

void OPL::getBlockFNum(float note, uint8_t& block, uint16_t& fnum)
{
    note = std::max(0.0f, std::min(note, 127.0f));
    uint8_t intNote = (uint8_t)note;
    float frac = note - intNote;
    uint8_t block1, block2;
    uint16_t fnum1, fnum2;
    getBlockFNumForIntNote(intNote, block1, fnum1);

    if (frac == 0.0f) {
        block = block1;
        fnum = fnum1;
    } else {
        getBlockFNumForIntNote(intNote + 1, block2, fnum2);
        if (block2 > block1) {
            block1 = block2;
            fnum1 = 0x1e4;
        }
        block = block1;
        fnum = fnum1 + (uint16_t)lround((fnum2 - fnum1) * frac);
    }
}

uint8_t OPL::getTLForMIDIVolume(uint8_t midiVolume)
{
    midiVolume = std::max((uint8_t)0, std::min(midiVolume, (uint8_t)127));
    if (midiVolume == 127) {
        return 0;
    }

    return midiVolumeCurve[midiVolume];
}

void OPL::getOperatorRegOffsetsForVoice(uint8_t voice, uint16_t* regOffsets, uint8_t& numOffsets)
{
    if (voice > 17) {
        // 4-op channel
        numOffsets = 4;
        uint8_t ch4Op = voice - 18;
        uint8_t ch2Op[2];
        OPL::get2OpChannelsFor4OpChannel(ch4Op, &ch2Op[0]);
        OPL::get2OpRegOffsets(ch2Op[0], &regOffsets[0]);
        OPL::get2OpRegOffsets(ch2Op[1], &regOffsets[2]);
    } else {
        // 2-op channel
        numOffsets = 2;
        OPL::get2OpRegOffsets(voice, &regOffsets[0]);
    }
}

void OPL::getChannelRegOffsetsForVoice(uint8_t voice, uint16_t* regOffsets, uint8_t& numOffsets)
{
    if (voice > 17) {
        // 4-op channel
        numOffsets = 2;
        uint8_t ch2Op[2];
        OPL::get2OpChannelsFor4OpChannel(voice - 18, &ch2Op[0]);
        regOffsets[0] = OPL::get2OpChannelRegOffset(ch2Op[0]);
        regOffsets[1] = OPL::get2OpChannelRegOffset(ch2Op[1]);
    } else {
        // 2-op channel
        numOffsets = 1;
        regOffsets[0] = OPL::get2OpChannelRegOffset(voice);
    }
}

uint8_t OPL::get2OpChannelForOperatorRegOffset(uint16_t offset)
{
    uint8_t offsetLSB = offset & 0xff;
    uint8_t voice = (offsetLSB / 8) * 3;
    voice += (offsetLSB % 8) % 3;
    if (offset & 0x100) {
        voice += 9;
    }
    return voice;
}

uint8_t OPL::getVoiceForOperatorRegOffset(uint16_t offset, uint8_t connection)
{
    uint8_t ch2Op = OPL::get2OpChannelForOperatorRegOffset(offset);
    uint8_t ch4Op = OPL::get4OpChannelFor2OpChannel(ch2Op);
    if (ch4Op == 255) {
        return ch2Op;
    } else {
        uint8_t chBit = 1 << ch4Op;
        if (connection & chBit) {
            return 18 + ch4Op;
        } else {
            return ch2Op;
        }
    }
}

uint8_t OPL::getVoiceForChannelRegOffset(uint16_t offset, uint8_t connection)
{
    uint8_t ch2Op = (offset & 0xff);
    if (offset & 0x100) {
        ch2Op += 9;
    }
    uint8_t ch4Op = OPL::get4OpChannelFor2OpChannel(ch2Op);
    if (ch4Op == 255) {
        return ch2Op;
    } else {
        uint8_t chBit = 1 << ch4Op;
        if (connection & chBit) {
            return 18 + ch4Op;
        } else {
            return ch2Op;
        }
    }
}

uint8_t OPL::getVoiceForRegister(uint16_t reg, uint8_t connection)
{
    uint8_t regLSB = reg & 0xff;
    if (regLSB >= 0x20 && regLSB <= 0x35) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x20, connection);
    } else if (regLSB >= 0x40 && regLSB <= 0x55) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x40, connection);
    } else if (regLSB >= 0x60 && regLSB <= 0x75) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x60, connection);
    } else if (regLSB >= 0x80 && regLSB <= 0x95) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x80, connection);
    } else if (regLSB >= 0xa0 && regLSB <= 0xa8) {
        return OPL::getVoiceForChannelRegOffset(reg - 0xa0, connection);
    } else if (regLSB >= 0xb0 && regLSB <= 0xb8) {
        return OPL::getVoiceForChannelRegOffset(reg - 0xb0, connection);
    } else if (regLSB >= 0xc0 && regLSB <= 0xc8) {
        return OPL::getVoiceForChannelRegOffset(reg - 0xc0, connection);
    } else if (regLSB >= 0xe0 && regLSB <= 0xf5) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0xe0, connection);
    } else {
        return 255;
    }
}

bool OPL::isVoice4Op(uint8_t voice)
{
    return voice > 17;
}

void OPL::getVoice2OpChannels(uint8_t voice, uint8_t* channels, uint8_t& numChannels)
{
    if (voice > 17) {
        numChannels = 2;
        OPL::get2OpChannelsFor4OpChannel(voice - 18, channels);
    } else {
        numChannels = 1;
        channels[0] = voice;
    }
}

uint8_t OPL::getVoice2OpChannel(uint8_t voice)
{
    uint8_t numChannels;
    uint8_t channels[2];
    OPL::getVoice2OpChannels(voice, &channels[0], numChannels);
    return channels[0];
}

uint8_t OPL::getVoice4OpChannel(uint8_t voice)
{
    if (voice > 17) {
        return voice - 18;
    } else {
        return OPL::get4OpChannelFor2OpChannel(voice);
    }
}
