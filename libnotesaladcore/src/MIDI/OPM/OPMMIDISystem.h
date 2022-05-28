#pragma once

#include "../../OPM/OPMTone.h"
#include "../BasicVoiceAllocator.h"
#include "../MIDIDriver.h"
#include "../PatchManagerBase.h"
#include "../TimeSource.h"
#include "OPMToneGenerator.h"

class OPMMIDISystem {
public:
    BasicVoiceAllocator<OPMTone, 8> voiceAllocator;
    OPMToneGenerator toneGenerator;
    TimeSource timeSource;
    MIDIDriver<OPMTone, 8> midiDriver;
    OPMMIDISystem(BufferedOPMDevice* device, PatchManagerBase<OPMTone>* patchManager);
};
