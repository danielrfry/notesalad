#include "MIDIParser.h"

MIDIParser::MIDIParser(IMIDIDriver* driver) : buffer(), bufferPos(0), driver(driver)
{
}

void MIDIParser::putBuffer(uint8_t* data, uint32_t length)
{
    while (length > 0) {
        if (data[0] & 0x80) {
            if ((this->bufferPos > 0) && (this->buffer[0] == 0xf0)) {
                if (data[0] == 0xf7) {
                    // End of a SysEx message - store the end byte
                    this->putByte(0xf7);
                    data++;
                    length--;
                    this->parseAndDeletePacket();
                    continue;
                }
            }
            this->parseAndDeletePacket();
            this->bufferPos = 0;
            this->putByte(data[0]);
            data++;
            length--;
        } else {
            if (this->bufferPos > 0) {
                this->putByte(data[0]);
            }
            data++;
            length--;
        }
    }

    this->parseAndDeletePacket();
}

void MIDIParser::putByte(uint8_t b)
{
    if (this->bufferPos >= MIDI_BUFFER_SIZE) {
        this->bufferPos = 0;
    } else {
        this->buffer[this->bufferPos++] = b;
    }
}

#define CHECK_LENGTH(l) \
    if (length < l)     \
    return false

bool MIDIParser::parsePacket()
{
    uint8_t* msgData = &this->buffer[0];
    uint32_t length = this->bufferPos;

    if (length == 0)
        return false;

    uint8_t msgType = msgData[0] & 0xf0;
    uint8_t channel = msgData[0] & 0x0f;

    if (msgType == 0x80) {
        CHECK_LENGTH(3);
        this->driver->noteOff(channel, msgData[1] & 0x7f, msgData[2] & 0x7f);
        return true;
    } else if (msgType == 0x90) {
        CHECK_LENGTH(3);
        this->driver->noteOn(channel, msgData[1] & 0x7f, msgData[2] & 0x7f);
        return true;
    } else if (msgType == 0xb0) {
        CHECK_LENGTH(3);
        this->driver->controlChange(channel, msgData[1] & 0x7f, msgData[2] & 0x7f);
        return true;
    } else if (msgType == 0xc0) {
        CHECK_LENGTH(2);
        this->driver->programChange(channel, msgData[1] & 0x7f);
        return true;
    } else if (msgData[0] == 0xf0) {
        if (msgData[length - 1] == 0xf7) {
            this->driver->sysEx(msgData, length);
            return true;
        }
    } else if (msgType == 0xe0) {
        CHECK_LENGTH(3);
        this->driver->pitchWheel(channel, msgData[1] | (msgData[2] << 7));
        return true;
    } else {
        // Unknown message, skip over it
        this->bufferPos = 0;
    }

    return false;
}

void MIDIParser::parseAndDeletePacket()
{
    if (this->parsePacket()) {
        this->bufferPos = 0;
    }
}