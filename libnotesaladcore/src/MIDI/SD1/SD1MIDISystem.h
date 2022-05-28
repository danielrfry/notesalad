#pragma once

#include "../../SD1/SD1Tone.h"
#include "../BasicVoiceAllocator.h"
#include "../MIDIDriver.h"
#include "../PatchManagerBase.h"
#include "../TimeSource.h"
#include "SD1ToneGenerator.h"

class SD1MIDISystem {
public:
    BasicVoiceAllocator<SD1Tone, 16> voiceAllocator;
    SD1ToneGenerator toneGenerator;
    TimeSource timeSource;
    MIDIDriver<SD1Tone, 16> midiDriver;
    SD1MIDISystem(SD1DeviceBase* device, PatchManagerBase<SD1Tone>* patchManager);
};
