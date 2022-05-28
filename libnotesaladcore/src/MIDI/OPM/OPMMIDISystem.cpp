#include "OPMMIDISystem.h"
#include "OPMParamInfo.h"

OPMMIDISystem::OPMMIDISystem(BufferedOPMDevice* device, PatchManagerBase<OPMTone>* patchManager)
    : voiceAllocator(), toneGenerator(device), timeSource(),
      midiDriver(&voiceAllocator, &toneGenerator, patchManager, &timeSource, SYSEX_DEV_ID_OPM, &OPM_PARAMS_INFO)
{
}
