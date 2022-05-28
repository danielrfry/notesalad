#pragma once

#include <cstdint>

#include "../MIDI/Patch.h"
#include "OPMTone.h"

namespace OPM {
uint8_t getKCForMIDINote(uint8_t note);
}
