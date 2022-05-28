#include <cmath>

#include "../Utils.h"
#include "PatchParams.h"

float PatchParamUtils::pitchOffsetFromNRPNValue(uint16_t nrpnValue)
{
    uint8_t msb = ((nrpnValue >> 7) & 0x7f);
    uint8_t lsb = nrpnValue & 0x7f;
    return ((float)msb - 64) + (lsb / 128.0f);
}

uint16_t PatchParamUtils::nrpnValueFromPitchOffset(float pitch)
{
    pitch = clamp(pitch, -64.0f, 63.0f);
    uint8_t msb = (uint8_t)floor(pitch);
    uint8_t lsb = ((uint8_t)floor((pitch - msb) * 128.0f)) & 0x7f;
    return (msb << 7) | lsb;
}
