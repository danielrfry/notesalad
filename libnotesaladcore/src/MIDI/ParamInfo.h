#pragma once

#include <cstdint>

struct ParamInfo {
    uint16_t id;
    uint16_t defaultValue;
};

struct ParamInfoList {
    uint16_t numParams;
    const ParamInfo* paramInfo;
};

extern const ParamInfoList UNIVERSAL_PARAMS_INFO;
