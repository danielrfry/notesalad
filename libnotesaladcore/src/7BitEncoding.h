#pragma once

#include "BufferIO.h"

class Encoder7Bit : public IWriter {
public:
    Encoder7Bit(IWriter* baseWriter);
    virtual size_t write(const uint8_t* src, size_t count) override;
    bool flush();

private:
    IWriter* baseWriter;
    uint8_t buffer[7];
    uint8_t msbs;
    uint8_t bufferPos;
};

class Decoder7Bit : public IReader {
public:
    Decoder7Bit(IReader* baseReader);
    virtual size_t read(uint8_t* dest, size_t count) override;
    virtual bool eof() override;

private:
    IReader* baseReader;
    uint8_t msbs;
    uint8_t msbsAvailable;
};

bool encodeLEB128(uint16_t v, IWriter* buffer);
bool decodeLEB128(uint16_t& outDecodedValue, IReader* buffer);

size_t get7BitEncodedSize(size_t unencodedSize);
size_t get7BitDecodedSize(size_t encodedSize);
bool encode7Bit(const uint8_t* input, size_t inputSize, uint8_t* output, size_t outputSize);
bool decode7Bit(const uint8_t* input, size_t inputSize, uint8_t* output, size_t outputSize);
