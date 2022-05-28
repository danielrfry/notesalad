#include "7BitEncoding.h"

#include <cstdio>

Encoder7Bit::Encoder7Bit(IWriter* baseWriter) : baseWriter(baseWriter), buffer(), msbs(0), bufferPos(0)
{
}

size_t Encoder7Bit::write(const uint8_t* src, size_t count)
{
    size_t bytesWritten = 0;
    for (size_t pos = 0; pos < count; pos++) {
        msbs = (msbs << 1) | (src[pos] >> 7);
        buffer[bufferPos++] = src[pos] & 0x7f;
        if (bufferPos >= 7) {
            if (!flush())
                break;
        }
        bytesWritten++;
    }

    return bytesWritten;
}

bool Encoder7Bit::flush()
{
    if (bufferPos > 0) {
        msbs = msbs << (7 - bufferPos);
        if (!baseWriter->write(msbs))
            return false;
        if (baseWriter->write(&buffer[0], bufferPos) < bufferPos)
            return false;
        bufferPos = 0;
        msbs = 0;
    }
    return true;
}

Decoder7Bit::Decoder7Bit(IReader* baseReader) : baseReader(baseReader), msbs(0), msbsAvailable(0)
{
}

size_t Decoder7Bit::read(uint8_t* dest, size_t count)
{
    for (size_t pos = 0; pos < count; pos++) {
        if (msbsAvailable == 0) {
            if (!baseReader->read(msbs))
                return pos;
            msbsAvailable = 7;
        }

        if (!baseReader->read(dest[pos]))
            return pos;
        msbs = msbs << 1;
        dest[pos] |= msbs & 0x80;
        msbsAvailable--;
    }

    return count;
}

bool Decoder7Bit::eof()
{
    return baseReader->eof();
}

bool encodeLEB128(uint16_t v, IWriter* buffer)
{
    uint8_t b;
    do {
        b = v & 0x7f;
        v = v >> 7;
        if (v != 0) {
            b |= 0x80;
        }
        if (!buffer->write(b)) {
            return false;
        }
    } while (v != 0);

    return true;
}

bool decodeLEB128(uint16_t& outDecodedValue, IReader* buffer)
{
    uint8_t shift = 0;
    uint8_t b;
    outDecodedValue = 0;
    while (buffer->read(b)) {
        outDecodedValue |= ((b & 0x7f) << shift);
        shift += 7;
        if (!(b & 0x80)) {
            return true;
        }
    }
    return false;
}

size_t get7BitEncodedSize(size_t unencodedSize)
{
    if (unencodedSize == 0) {
        return 0;
    } else {
        return unencodedSize + ((unencodedSize + 6) / 7);
    }
}

size_t get7BitDecodedSize(size_t encodedSize)
{
    if (encodedSize == 0) {
        return 0;
    } else {
        return encodedSize - ((encodedSize + 7) / 8);
    }
}

bool encode7Bit(const uint8_t* input, size_t inputSize, uint8_t* output, size_t outputSize)
{
    BufferWriter writer(output, outputSize);
    Encoder7Bit encoder(&writer);
    return encoder.write(input, inputSize) == inputSize && encoder.flush();
}

bool decode7Bit(const uint8_t* input, size_t inputSize, uint8_t* output, size_t outputSize)
{
    BufferReader reader(input, inputSize);
    Decoder7Bit decoder(&reader);
    return decoder.read(output, outputSize) == outputSize;
}
