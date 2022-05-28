#include "SD1ParamInfo.h"

const ParamInfo SD1_PARAMS_INFO_ITEMS[] = {
    { 0x0000, 1 }, // Octave
    { 0x0001, 0 }, // LFO freq
    { 0x0002, 0 }, // Algorithm
    { 0x0020, 0 }, // Op 1 sustain
    { 0x0021, 0 }, // Op 1 XOF
    { 0x0022, 0 }, // Op 1 KSR
    { 0x0023, 15 }, // Op 1 release
    { 0x0024, 0 }, // Op 1 decay
    { 0x0025, 15 }, // Op 1 attack
    { 0x0026, 15 }, // Op 1 sus level
    { 0x0027, 63 }, // Op 1 level
    { 0x0028, 0 }, // Op 1 KSL
    { 0x0029, 0 }, // Op 1 AM depth
    { 0x002a, 0 }, // Op 1 AM enable
    { 0x002b, 0 }, // Op 1 vib depth
    { 0x002c, 0 }, // Op 1 vib enable
    { 0x002d, 1 }, // Op 1 mult
    { 0x002e, 0 }, // Op 1 detune
    { 0x002f, 0 }, // Op 1 wave
    { 0x0030, 0 }, // Op 1 feedback
    { 0x0040, 0 }, // Op 2 sustain
    { 0x0041, 0 }, // Op 2 XOF
    { 0x0042, 0 }, // Op 2 KSR
    { 0x0043, 15 }, // Op 2 release
    { 0x0044, 0 }, // Op 2 decay
    { 0x0045, 15 }, // Op 2 attack
    { 0x0046, 15 }, // Op 2 sus level
    { 0x0047, 0 }, // Op 2 level
    { 0x0048, 0 }, // Op 2 KSL
    { 0x0049, 0 }, // Op 2 AM depth
    { 0x004a, 0 }, // Op 2 AM enable
    { 0x004b, 0 }, // Op 2 vib depth
    { 0x004c, 0 }, // Op 2 vib enable
    { 0x004d, 1 }, // Op 2 mult
    { 0x004e, 0 }, // Op 2 detune
    { 0x004f, 0 }, // Op 2 wave
    { 0x0050, 0 }, // Op 2 feedback
    { 0x0060, 0 }, // Op 3 sustain
    { 0x0061, 0 }, // Op 3 XOF
    { 0x0062, 0 }, // Op 3 KSR
    { 0x0063, 15 }, // Op 3 release
    { 0x0064, 0 }, // Op 3 decay
    { 0x0065, 15 }, // Op 3 attack
    { 0x0066, 15 }, // Op 3 sus level
    { 0x0067, 63 }, // Op 3 level
    { 0x0068, 0 }, // Op 3 KSL
    { 0x0069, 0 }, // Op 3 AM depth
    { 0x006a, 0 }, // Op 3 AM enable
    { 0x006b, 0 }, // Op 3 vib depth
    { 0x006c, 0 }, // Op 3 vib enable
    { 0x006d, 1 }, // Op 3 mult
    { 0x006e, 0 }, // Op 3 detune
    { 0x006f, 0 }, // Op 3 wave
    { 0x0070, 0 }, // Op 3 feedback
    { 0x0080, 0 }, // Op 4 sustain
    { 0x0081, 0 }, // Op 4 XOF
    { 0x0082, 0 }, // Op 4 KSR
    { 0x0083, 15 }, // Op 4 release
    { 0x0084, 0 }, // Op 4 decay
    { 0x0085, 15 }, // Op 4 attack
    { 0x0086, 15 }, // Op 4 sus level
    { 0x0087, 63 }, // Op 4 level
    { 0x0088, 0 }, // Op 4 KSL
    { 0x0089, 0 }, // Op 4 AM depth
    { 0x008a, 0 }, // Op 4 AM enable
    { 0x008b, 0 }, // Op 4 vib depth
    { 0x008c, 0 }, // Op 4 vib enable
    { 0x008d, 1 }, // Op 4 mult
    { 0x008e, 0 }, // Op 4 detune
    { 0x008f, 0 }, // Op 4 wave
    { 0x0090, 0 }, // Op 4 feedback
};

const ParamInfoList SD1_PARAMS_INFO = { 71, SD1_PARAMS_INFO_ITEMS };
