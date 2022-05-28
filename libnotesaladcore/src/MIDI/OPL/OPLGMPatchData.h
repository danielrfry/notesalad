#pragma once

#include "../../OPL/OPLTone.h"
#include "../Patch.h"

#include <cstdint>

namespace OPL {
Patch<OPLTone> getGMPatch(uint8_t bank, uint8_t program);
bool getGMDrumPatch(uint8_t note, Patch<OPLTone>& patch);
}
