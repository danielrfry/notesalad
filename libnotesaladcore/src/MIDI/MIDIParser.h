#pragma once

#include "IMIDIDriver.h"

#include <cstdint>

#define MIDI_BUFFER_SIZE 512

class MIDIParser {
public:
    MIDIParser(IMIDIDriver* driver);
    void putBuffer(uint8_t* data, uint32_t length);

private:
    uint8_t buffer[MIDI_BUFFER_SIZE];
    uint16_t bufferPos;
    IMIDIDriver* driver;
    bool parsePacket();
    void parseAndDeletePacket();
    void putByte(uint8_t b);
};
