#pragma once

#include <cstdint>

#include "ParamIDs.h"

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
        case PARAM_POLYMODE:
            return polyMode;
        case PARAM_FIXEDNOTENUM:
            return fixedNoteNum;
        case PARAM_PITCHOFFSET:
            return this->pitchOffset;
        case PARAM_GLIDEDURATION:
            return this->glideDurationMS;
        case PARAM_VELOCITYDEPTH:
            return this->velocityDepth;
        default:
            return tone.getParam(paramID);
        }
    }
    void setParam(uint16_t paramID, uint16_t value)
    {
        switch (paramID) {
        case PARAM_POLYMODE:
            polyMode = value & 0xff;
            break;
        case PARAM_FIXEDNOTENUM:
            fixedNoteNum = value & 0x7f;
            break;
        case PARAM_PITCHOFFSET:
            pitchOffset = clamp(value, (uint16_t)0, (uint16_t)0x3fff);
            break;
        case PARAM_GLIDEDURATION:
            this->glideDurationMS = value;
            break;
        case PARAM_VELOCITYDEPTH:
            this->velocityDepth = value;
        default:
            tone.setParam(paramID, value);
        }
    }

private:
};
