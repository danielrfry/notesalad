#pragma once

#include "../../OPL/OPLTone.h"
#include "../MIDIDriver.h"
#include "../PatchManagerBase.h"
#include "../TimeSource.h"
#include "OPL3VoiceAllocator.h"
#include "OPLToneGenerator.h"
#include "OPLParamInfo.h"

class OPL3MIDISystem {
public:
    TimeSource timeSource;
    OPL3VoiceAllocator voiceAllocator;
    OPLToneGenerator toneGenerator;
    MIDIDriver<OPLTone, 24> midiDriver;
    OPL3MIDISystem(OPLReadWriteDeviceBase* device, PatchManagerBase<OPLTone>* patchManager);
};
