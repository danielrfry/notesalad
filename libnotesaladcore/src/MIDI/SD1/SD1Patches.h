#pragma once

#include "../../OPL/OPLTone.h"
#include "../../SD1/SD1Tone.h"
#include "../Patch.h"

#include <cstdint>

namespace SD1 {
void getGMPatch(uint8_t bank, uint8_t program, Patch<SD1Tone>& patch);
bool getGMDrumPatch(uint8_t note, Patch<SD1Tone>& patch);
}
