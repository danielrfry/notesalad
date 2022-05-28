#include "DefaultOPLPatchManager.h"
#include "OPLGMPatchData.h"

bool DefaultOPLPatchManager::getPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<OPLTone>& outPatch)
{
    if (noteNum < 0x80) {
        return OPL::getGMDrumPatch(noteNum, outPatch);
    } else {
        outPatch = OPL::getGMPatch(bank, program);
        return true;
    }
}

bool DefaultOPLPatchManager::supportsDrums()
{
    return true;
}
