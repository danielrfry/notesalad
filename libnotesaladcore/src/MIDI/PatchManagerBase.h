#pragma once

#include "Patch.h"

template <class Tone>
class PatchManagerBase {
public:
    virtual bool supportsDrums()
    {
        return false;
    }

    // noteNum should be 255 for melodic patches
    virtual bool getPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<Tone>& outPatch)
    {
        return false;
    }

    virtual bool setPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<Tone>& patch)
    {
        return false;
    }

    virtual bool deletePatch(uint8_t bank, uint8_t program, uint8_t noteNum)
    {
        return false;
    }
};
