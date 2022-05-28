#include "OPMMIDIManager.h"

OPMMIDIManager::OPMMIDIManager(void* context, OPMDeviceBase* device)
    : bufferedDevice(device), patchManager(),
      midiSystem(&bufferedDevice, &patchManager), parser(&midiSystem.midiDriver)
{
}

void OPMMIDIManager::reset()
{
}

void OPMMIDIManager::setReceiveCallback(std::function<void(void*, uint32_t)> callback)
{
    this->midiSystem.midiDriver.onSendMIDI = callback;
}

void OPMMIDIManager::sendMIDI(uint8_t* msgData, uint32_t msgLength)
{
    this->parser.putBuffer(msgData, msgLength);
}

void OPMMIDIManager::setTime(uint32_t timeMS)
{
    this->midiSystem.timeSource.timeMS = timeMS;
}

void OPMMIDIManager::update()
{
    this->midiSystem.midiDriver.update();
}
