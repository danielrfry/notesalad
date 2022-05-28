#include "OPL3MIDISystem.h"
#include "../SysEx.h"
#include "OPLParamInfo.h"

OPL3MIDISystem::OPL3MIDISystem(OPLReadWriteDeviceBase* device, PatchManagerBase<OPLTone>* patchManager)
    : timeSource(), voiceAllocator(), toneGenerator(device, &timeSource),
      midiDriver(&voiceAllocator, &toneGenerator, patchManager, &timeSource, SYSEX_DEV_ID_OPL3, &OPL_PARAMS_INFO)
{
}
