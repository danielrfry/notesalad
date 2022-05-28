#pragma once

#include "../../OPL/OPLTone.h"
#include "../BasicVoiceAllocator.h"
#include "../MIDIDriver.h"
#include "../PatchManagerBase.h"
#include "../TimeSource.h"
#include "OPLSD1ToneGenerator.h"

class OPLSD1MIDISystem {
public:
    BasicVoiceAllocator<OPLTone, 16> voiceAllocator;
    OPLSD1ToneGenerator toneGenerator;
    TimeSource timeSource;
    MIDIDriver<OPLTone, 16> midiDriver;
    OPLSD1MIDISystem(SD1DeviceBase* device, PatchManagerBase<OPLTone>* patchManager);
};
