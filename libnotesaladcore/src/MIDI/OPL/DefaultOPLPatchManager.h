#pragma once

#include "../../OPL/OPLTone.h"
#include "../PatchManagerBase.h"

class DefaultOPLPatchManager : public PatchManagerBase<OPLTone> {
public:
    virtual bool supportsDrums() override;
    virtual bool getPatch(uint8_t bank, uint8_t program, uint8_t noteNum, Patch<OPLTone>& outPatch) override;
};
