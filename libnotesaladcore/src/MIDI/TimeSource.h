#pragma once

#include <cstdint>

#define MILLISECONDS_MAX UINT32_MAX

typedef uint32_t Milliseconds;

class TimeSource {
public:
    Milliseconds timeMS;
};
