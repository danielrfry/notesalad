#pragma once

#include <notesaladcore.h>

#define PARAM_ID_TEST_A 0
#define PARAM_ID_TEST_B 27
#define PARAM_ID_TEST_C 488

#define PARAM_DEFAULT_TEST_A 0
#define PARAM_DEFAULT_TEST_B 37
#define PARAM_DEFAULT_TEST_C 981

class TestTone {
public:
    uint16_t paramA = PARAM_DEFAULT_TEST_A;
    uint16_t paramB = PARAM_DEFAULT_TEST_B;
    uint16_t paramC = PARAM_DEFAULT_TEST_C;

    uint16_t getParam(uint16_t paramID);
    void setParam(uint16_t paramID, uint16_t paramValue);
};

extern const ParamInfo TEST_PARAMS_INFO_ITEMS[];

extern const ParamInfoList TEST_PARAMS_INFO;
