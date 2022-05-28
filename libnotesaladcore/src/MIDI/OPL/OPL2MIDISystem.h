#pragma once

#include "../../OPL/OPLTone.h"
#include "../MIDIDriver.h"
#include "../PatchManagerBase.h"
#include "../TimeSource.h"
#include "OPL2VoiceAllocator.h"
#include "OPLToneGenerator.h"

class OPL2MIDISystem {
public:
    TimeSource timeSource;
    OPL2VoiceAllocator voiceAllocator;
    OPLToneGenerator toneGenerator;
    MIDIDriver<OPLTone, 9> midiDriver;
    OPL2MIDISystem(BufferedOPLDevice* device, PatchManagerBase<OPLTone>* patchManager);
};
