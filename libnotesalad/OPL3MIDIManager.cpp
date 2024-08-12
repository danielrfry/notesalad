#include "OPL3MIDIManager.h"

OPL3MIDIManager::OPL3MIDIManager(void* context, OPLDeviceBase* device)
    : bufferedDevice(device), gmPatchManager(), patchManager(&gmPatchManager),
      midiSystem(&bufferedDevice, &patchManager), parser(&midiSystem.midiDriver)
{
}

void OPL3MIDIManager::reset()
{
    this->midiSystem.midiDriver.reset(true);
}

void OPL3MIDIManager::setReceiveCallback(std::function<void(void*, uint32_t)> callback)
{
    this->midiSystem.midiDriver.onSendMIDI = callback;
}

void OPL3MIDIManager::sendMIDI(uint8_t* msgData, uint32_t msgLength)
{
    this->parser.putBuffer(msgData, msgLength);
}

void OPL3MIDIManager::setTime(uint32_t timeMS)
{
    this->midiSystem.timeSource.timeMS = timeMS;
}

void OPL3MIDIManager::update()
{
    this->midiSystem.midiDriver.update();
}
