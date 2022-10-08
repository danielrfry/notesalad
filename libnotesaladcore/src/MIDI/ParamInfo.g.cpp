#include "ParamInfo.h"

const ParamInfo UNIVERSAL_PARAMS_INFO_ITEMS[] = {
    { 0x2000, 0 }, // Poly mode
    { 0x2001, 0 }, // Note num
    { 0x2002, 8192 }, // Pitch offset
    { 0x2003, 0 }, // Glide
    { 0x2004, 127 }, // Velocity depth
    { 0x3000, 255 }, // Map 1 src
    { 0x3001, 0 }, // Map 1 dest
    { 0x3002, 8192 }, // Map 1 amount
    { 0x3010, 255 }, // Map 2 src
    { 0x3011, 0 }, // Map 2 dest
    { 0x3012, 8192 }, // Map 2 amount
    { 0x3020, 255 }, // Map 3 src
    { 0x3021, 0 }, // Map 3 dest
    { 0x3022, 8192 }, // Map 3 amount
    { 0x3030, 255 }, // Map 4 src
    { 0x3031, 0 }, // Map 4 dest
    { 0x3032, 8192 }, // Map 4 amount
    { 0x3100, 0 }, // LFO 1 wave
    { 0x3101, 0 }, // LFO 1 period
    { 0x3102, 0 }, // LFO 1 sync
    { 0x3103, 0 }, // LFO 1 one shot
    { 0x3110, 0 }, // LFO 2 wave
    { 0x3111, 0 }, // LFO 2 period
    { 0x3112, 0 }, // LFO 2 sync
    { 0x3113, 0 }, // LFO 2 one shot
};

const ParamInfoList UNIVERSAL_PARAMS_INFO = { 25, UNIVERSAL_PARAMS_INFO_ITEMS };
