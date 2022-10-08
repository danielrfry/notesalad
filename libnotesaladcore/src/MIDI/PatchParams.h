#pragma once

#include <cstdint>

#define PATCH_PARAM_POLYMODE 0x2000
#define PATCH_PARAM_FIXEDNOTENUM 0x2001
#define PATCH_PARAM_PITCHOFFSET 0x2002
#define PATCH_PARAM_GLIDEDURATION 0x2003
#define PATCH_PARAM_VELOCITYDEPTH 0x2004

#define POLYMODE_POLY 0x00
#define POLYMODE_MONO 0x01
#define POLYMODE_MONO_EXT 0x02
#define POLYMODE_LEGATO 0x03
#define POLYMODE_LEGATO_EXT 0x04

namespace PatchParamUtils {
float pitchOffsetFromNRPNValue(uint16_t nrpnValue);
uint16_t nrpnValueFromPitchOffset(float pitchOffset);
}

template <class Tone>
class PatchParams {
public:
    Tone tone;
    uint8_t polyMode = POLYMODE_POLY;
    uint8_t fixedNoteNum = 0;
    uint16_t pitchOffset = 0x2000;
    uint16_t glideDurationMS = 0;
    uint8_t velocityDepth = 0x7f;

    uint16_t getParam(uint16_t paramID)
    {
        switch (paramID) {
        case PATCH_PARAM_POLYMODE:
            return polyMode;
        case PATCH_PARAM_FIXEDNOTENUM:
            return fixedNoteNum;
        case PATCH_PARAM_PITCHOFFSET:
            return this->pitchOffset;
        case PATCH_PARAM_GLIDEDURATION:
            return this->glideDurationMS;
        case PATCH_PARAM_VELOCITYDEPTH:
            return this->velocityDepth;
        default:
            return tone.getParam(paramID);
        }
    }
    void setParam(uint16_t paramID, uint16_t value)
    {
        switch (paramID) {
        case PATCH_PARAM_POLYMODE:
            polyMode = value & 0xff;
            break;
        case PATCH_PARAM_FIXEDNOTENUM:
            fixedNoteNum = value & 0x7f;
            break;
        case PATCH_PARAM_PITCHOFFSET:
            pitchOffset = clamp(value, (uint16_t)0, (uint16_t)0x3fff);
            break;
        case PATCH_PARAM_GLIDEDURATION:
            this->glideDurationMS = value;
            break;
        case PATCH_PARAM_VELOCITYDEPTH:
            this->velocityDepth = value;
        default:
            tone.setParam(paramID, value);
        }
    }

private:
};
