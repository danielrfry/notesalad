#pragma once

#include "../../SD1/SD1Tone.h"
#include "../PatchManagerBase.h"

class DefaultSD1PatchManager : public PatchManagerBase<SD1Tone> {
public:
    virtual bool supportsDrums() override;
    virtual bool getPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<SD1Tone>& outPatch) override;
};
