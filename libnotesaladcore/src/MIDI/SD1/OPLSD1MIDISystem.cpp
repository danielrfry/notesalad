#include "OPLSD1MIDISystem.h"
#include "../OPL/OPLParamInfo.h"

OPLSD1MIDISystem::OPLSD1MIDISystem(SD1DeviceBase* device, PatchManagerBase<OPLTone>* patchManager)
    : voiceAllocator(), toneGenerator(device), timeSource(),
      midiDriver(&voiceAllocator, &toneGenerator, patchManager, &timeSource, SYSEX_DEV_ID_OPL3, &OPL_PARAMS_INFO)
{
}
