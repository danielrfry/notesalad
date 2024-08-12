#include "OPL2MIDIManager.h"

OPL2MIDIManager::OPL2MIDIManager(void* context, OPLDeviceBase* device)
    : bufferedDevice(device), patchManager(),
      midiSystem(&bufferedDevice, &patchManager), parser(&midiSystem.midiDriver)
{
}

void OPL2MIDIManager::reset()
{
    this->midiSystem.midiDriver.reset(true);
}

void OPL2MIDIManager::setReceiveCallback(std::function<void(void*, uint32_t)> callback)
{
    this->midiSystem.midiDriver.onSendMIDI = callback;
}

void OPL2MIDIManager::sendMIDI(uint8_t* msgData, uint32_t msgLength)
{
    this->parser.putBuffer(msgData, msgLength);
}

void OPL2MIDIManager::setTime(uint32_t timeMS)
{
    this->midiSystem.timeSource.timeMS = timeMS;
}

void OPL2MIDIManager::update()
{
    this->midiSystem.midiDriver.update();
}
