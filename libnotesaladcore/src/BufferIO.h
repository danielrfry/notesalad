#pragma once

#include <cstddef>
#include <cstdint>

class IReader {
public:
    virtual size_t read(uint8_t* dest, size_t count) = 0;
    virtual bool eof() = 0;

    template <typename T>
    bool read(T& item)
    {
        return this->read(&item, sizeof(item)) == sizeof(item);
    }
};

class IWriter {
public:
    virtual size_t write(const uint8_t* src, size_t count) = 0;

    template <typename T>
    bool write(T& item)
    {
        return this->write((const uint8_t*)&item, sizeof(item)) == sizeof(item);
    }
    bool write(uint8_t b)
    {
        return this->write(&b, 1);
    }
};

class BufferReader : public IReader {
public:
    const uint8_t* buffer;
    BufferReader(const uint8_t* buffer, size_t size);
    virtual bool eof() override;
    virtual size_t getSize();
    virtual size_t getPosition();
    virtual size_t read(uint8_t* dest, size_t count) override;
    virtual size_t seek(size_t pos);

private:
    size_t size;
    size_t position;
};

class BufferWriter : public IWriter {
public:
    uint8_t* buffer;
    BufferWriter(uint8_t* buffer, size_t size);
    size_t getSize();
    size_t getPosition();
    virtual size_t write(const uint8_t* src, size_t count) override;
    size_t seek(size_t pos);

private:
    size_t size;
    size_t position;
};

template <size_t arraySize>
class ArrayWriter : public BufferWriter {
public:
    ArrayWriter() : BufferWriter(nullptr, arraySize), backingStore()
    {
        buffer = &backingStore[0];
    }

private:
    uint8_t backingStore[arraySize];
};
