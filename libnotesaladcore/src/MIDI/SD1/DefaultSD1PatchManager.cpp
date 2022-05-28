#include "DefaultSD1PatchManager.h"
#include "SD1Patches.h"

bool DefaultSD1PatchManager::supportsDrums()
{
    return true;
}

bool DefaultSD1PatchManager::getPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<SD1Tone>& outPatch)
{
    if (noteNum < 0x80) {
        return SD1::getGMDrumPatch(noteNum, outPatch);
    } else {
        SD1::getGMPatch(bank, program, outPatch);
        return true;
    }
}
