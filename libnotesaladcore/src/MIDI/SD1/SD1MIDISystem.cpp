#include "SD1MIDISystem.h"
#include "../SysEx.h"
#include "SD1ParamInfo.h"

SD1MIDISystem::SD1MIDISystem(SD1DeviceBase* device, PatchManagerBase<SD1Tone>* patchManager)
    : voiceAllocator(), toneGenerator(device), timeSource(),
      midiDriver(&voiceAllocator, &toneGenerator, patchManager, &timeSource, SYSEX_DEV_ID_SD1, &SD1_PARAMS_INFO)
{
}
