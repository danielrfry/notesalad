#pragma once

#include "BufferIO.h"

#include <cstdint>
#include <sys/types.h>

#define PACKED __attribute__((__packed__))

template <uint8_t flag>
inline bool getFlag(uint8_t data)
{
    return (data & flag) != 0;
}

template <uint8_t flags>
inline void setFlags(uint8_t& data, bool value)
{
    data = (data & ~flags) | (value ? value & flags : 0);
}

template <uint8_t bit>
inline bool getBit(uint8_t data)
{
    return (data & (1 << bit)) != 0;
}

template <uint8_t start, uint8_t count>
inline uint8_t getBits(uint8_t data)
{
    return (data >> start) & ((1 << count) - 1);
}

template <uint8_t bit>
inline void setBit(uint8_t& data, bool value)
{
    setFlags<1 << bit>(data, value);
}

template <uint8_t start, uint8_t count>
inline void setBits(uint8_t& data, uint8_t value)
{
    uint8_t mask = ((1 << count) - 1) << start;
    data &= ~mask;
    data |= (value << start) & mask;
}

template <typename T>
inline T clamp(T value, T min, T max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

uint8_t lookupMIDIVolume(uint8_t volume);
