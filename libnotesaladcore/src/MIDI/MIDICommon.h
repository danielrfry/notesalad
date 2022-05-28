#pragma once

#include <cstdint>

#define DRUM_CHANNEL 9

#define PRIORITY_ACTIVE 120
#define PRIORITY_SUSTAINED 110
#define PRIORITY_ACTIVE_MONO 130

typedef enum NoteStatus : uint8_t {
    INACTIVE,
    ACTIVE,
    SUSTAINED
} NoteStatus;
