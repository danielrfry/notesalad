#include "SD1MIDIManager.h"

SD1MIDIManager::SD1MIDIManager(void* context, SD1DeviceBase* device)
    : patchManager(), midiSystem(device, &patchManager),
      parser(&midiSystem.midiDriver)
{
}

void SD1MIDIManager::reset()
{
    // Not currently implemented
}

void SD1MIDIManager::setReceiveCallback(std::function<void(void*, uint32_t)> callback)
{
    this->midiSystem.midiDriver.onSendMIDI = callback;
}

void SD1MIDIManager::sendMIDI(uint8_t* msgData, uint32_t msgLength)
{
    this->parser.putBuffer(msgData, msgLength);
}

void SD1MIDIManager::setTime(uint32_t timeMS)
{
    this->midiSystem.timeSource.timeMS = timeMS;
}

void SD1MIDIManager::update()
{
    this->midiSystem.midiDriver.update();
}
