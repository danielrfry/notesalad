#include "OPMParamInfo.h"

const ParamInfo OPM_PARAMS_INFO_ITEMS[] = {
    { 0x0001, 0 }, // Feedback
    { 0x0002, 0 }, // Connection
    { 0x0003, 0 }, // PM
    { 0x0004, 0 }, // AM
    { 0x0005, 15 }, // Operators
    { 0x0010, 0 }, // Op 1 detune 1
    { 0x0011, 1 }, // Op 1 mult
    { 0x0012, 127 }, // Op 1 level
    { 0x0013, 0 }, // Op 1 KS
    { 0x0014, 31 }, // Op 1 attack
    { 0x0015, 0 }, // Op 1 AM
    { 0x0016, 0 }, // Op 1 D1R
    { 0x0017, 0 }, // Op 1 detune 2
    { 0x0018, 0 }, // Op 1 D2R
    { 0x0019, 0 }, // Op 1 D1L
    { 0x001a, 15 }, // Op 1 RR
    { 0x0020, 0 }, // Op 2 detune 1
    { 0x0021, 1 }, // Op 2 mult
    { 0x0022, 127 }, // Op 2 level
    { 0x0023, 0 }, // Op 2 KS
    { 0x0024, 31 }, // Op 2 attack
    { 0x0025, 0 }, // Op 2 AM
    { 0x0026, 0 }, // Op 2 D1R
    { 0x0027, 0 }, // Op 2 detune 2
    { 0x0028, 0 }, // Op 2 D2R
    { 0x0029, 0 }, // Op 2 D1L
    { 0x002a, 15 }, // Op 2 RR
    { 0x0030, 0 }, // Op 3 detune 1
    { 0x0031, 1 }, // Op 3 mult
    { 0x0032, 127 }, // Op 3 level
    { 0x0033, 0 }, // Op 3 KS
    { 0x0034, 31 }, // Op 3 attack
    { 0x0035, 0 }, // Op 3 AM
    { 0x0036, 0 }, // Op 3 D1R
    { 0x0037, 0 }, // Op 3 detune 2
    { 0x0038, 0 }, // Op 3 D2R
    { 0x0039, 0 }, // Op 3 D1L
    { 0x003a, 15 }, // Op 3 RR
    { 0x0040, 0 }, // Op 4 detune 1
    { 0x0041, 1 }, // Op 4 mult
    { 0x0042, 0 }, // Op 4 level
    { 0x0043, 0 }, // Op 4 KS
    { 0x0044, 31 }, // Op 4 attack
    { 0x0045, 0 }, // Op 4 AM
    { 0x0046, 0 }, // Op 4 D1R
    { 0x0047, 0 }, // Op 4 detune 2
    { 0x0048, 0 }, // Op 4 D2R
    { 0x0049, 0 }, // Op 4 D1L
    { 0x004a, 15 }, // Op 4 RR
};

const ParamInfoList OPM_PARAMS_INFO = { 49, OPM_PARAMS_INFO_ITEMS };
