#include "OPLParamInfo.h"

const ParamInfo OPL_PARAMS_INFO_ITEMS[] = {
    { 0x0000, 0 }, // 4-op mode
    { 0x0001, 0 }, // Feedback
    { 0x0002, 0 }, // Connection
    { 0x0010, 0 }, // Op 1 AM
    { 0x0011, 0 }, // Op 1 VIB
    { 0x0012, 1 }, // Op 1 EGT
    { 0x0013, 0 }, // Op 1 KSR
    { 0x0014, 1 }, // Op 1 MULT
    { 0x0015, 0 }, // Op 1 KSL
    { 0x0016, 63 }, // Op 1 level
    { 0x0017, 15 }, // Op 1 attack
    { 0x0018, 0 }, // Op 1 decay
    { 0x0019, 15 }, // Op 1 sustain
    { 0x001a, 15 }, // Op 1 release
    { 0x001b, 0 }, // Op 1 wave
    { 0x0020, 0 }, // Op 2 AM
    { 0x0021, 0 }, // Op 2 VIB
    { 0x0022, 1 }, // Op 2 EGT
    { 0x0023, 0 }, // Op 2 KSR
    { 0x0024, 1 }, // Op 2 MULT
    { 0x0025, 0 }, // Op 2 KSL
    { 0x0026, 0 }, // Op 2 level
    { 0x0027, 15 }, // Op 2 attack
    { 0x0028, 0 }, // Op 2 decay
    { 0x0029, 15 }, // Op 2 sustain
    { 0x002a, 15 }, // Op 2 release
    { 0x002b, 0 }, // Op 2 wave
    { 0x0030, 0 }, // Op 3 AM
    { 0x0031, 0 }, // Op 3 VIB
    { 0x0032, 1 }, // Op 3 EGT
    { 0x0033, 0 }, // Op 3 KSR
    { 0x0034, 1 }, // Op 3 MULT
    { 0x0035, 0 }, // Op 3 KSL
    { 0x0036, 63 }, // Op 3 level
    { 0x0037, 15 }, // Op 3 attack
    { 0x0038, 0 }, // Op 3 decay
    { 0x0039, 15 }, // Op 3 sustain
    { 0x003a, 15 }, // Op 3 release
    { 0x003b, 0 }, // Op 3 wave
    { 0x0040, 0 }, // Op 4 AM
    { 0x0041, 0 }, // Op 4 VIB
    { 0x0042, 1 }, // Op 4 EGT
    { 0x0043, 0 }, // Op 4 KSR
    { 0x0044, 1 }, // Op 4 MULT
    { 0x0045, 0 }, // Op 4 KSL
    { 0x0046, 0 }, // Op 4 level
    { 0x0047, 15 }, // Op 4 attack
    { 0x0048, 0 }, // Op 4 decay
    { 0x0049, 15 }, // Op 4 sustain
    { 0x004a, 15 }, // Op 4 release
    { 0x004b, 0 }, // Op 4 wave
};

const ParamInfoList OPL_PARAMS_INFO = { 51, OPL_PARAMS_INFO_ITEMS };
