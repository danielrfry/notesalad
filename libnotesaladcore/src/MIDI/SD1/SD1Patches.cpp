#include "SD1Patches.h"
#include "../../SD1/SD1Tone.h"
#include "../../SD1/SD1Utils.h"
#include "../OPL/OPLGMPatchData.h"

#include <algorithm>
#include <cstring>

void SD1::getGMPatch(uint8_t bank, uint8_t program, Patch<SD1Tone>& patch)
{
    if (bank == 0 || bank == 1) {
        patch = Patch<SD1Tone>();
        Patch<OPLTone> oplPatch = OPL::getGMPatch(bank, program);
        SD1::convertOPLTone(oplPatch.params.tone, patch.params.tone);
    } else {
        patch = Patch<SD1Tone>();
    }
}

bool SD1::getGMDrumPatch(uint8_t note, Patch<SD1Tone>& patch)
{
    Patch<OPLTone> oplPatch;
    if (OPL::getGMDrumPatch(note, oplPatch)) {
        patch = Patch<SD1Tone>();
        SD1::convertOPLTone(oplPatch.params.tone, patch.params.tone);
        patch.params.fixedNoteNum = oplPatch.params.fixedNoteNum;
        return true;
    } else {
        return false;
    }
}
