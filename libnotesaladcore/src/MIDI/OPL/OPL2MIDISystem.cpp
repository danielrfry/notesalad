#include "OPL2MIDISystem.h"
#include "../SysEx.h"
#include "OPLParamInfo.h"

OPL2MIDISystem::OPL2MIDISystem(BufferedOPLDevice* device, PatchManagerBase<OPLTone>* patchManager)
    : timeSource(), voiceAllocator(), toneGenerator(device, &timeSource),
      midiDriver(&voiceAllocator, &toneGenerator, patchManager, &timeSource, SYSEX_DEV_ID_OPL2, &OPL_PARAMS_INFO)
{
}