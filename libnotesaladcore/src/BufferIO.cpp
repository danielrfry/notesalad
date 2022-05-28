#include "BufferIO.h"

#include <algorithm>
#include <cstring>

BufferReader::BufferReader(const uint8_t* buffer, size_t size)
    : buffer(buffer), size(size), position(0)
{
}

bool BufferReader::eof()
{
    return position >= size;
}

size_t BufferReader::getSize()
{
    return size;
}

size_t BufferReader::getPosition()
{
    return position;
}

size_t BufferReader::read(uint8_t* dest, size_t count)
{
    size_t bytesToRead = std::min(this->size - this->position, count);
    memcpy(dest, this->buffer + position, bytesToRead);
    this->position += bytesToRead;
    return bytesToRead;
}

size_t BufferReader::seek(size_t pos)
{
    return position = std::min(pos, size);
}

BufferWriter::BufferWriter(uint8_t* buffer, size_t size)
    : buffer(buffer), size(size), position(0)
{
}

size_t BufferWriter::getSize()
{
    return size;
}

size_t BufferWriter::getPosition()
{
    return position;
}

size_t BufferWriter::write(const uint8_t* src, size_t count)
{
    size_t bytesToWrite = std::min(this->size - this->position, count);
    memcpy(this->buffer + position, src, bytesToWrite);
    this->position += bytesToWrite;
    return bytesToWrite;
}

size_t BufferWriter::seek(size_t pos)
{
    return position = std::min(pos, size);
}
