#include "TestTone.h"

uint16_t TestTone::getParam(uint16_t paramID)
{
    switch (paramID) {
    case PARAM_ID_TEST_A:
        return paramA;
    case PARAM_ID_TEST_B:
        return paramB;
    case PARAM_ID_TEST_C:
        return paramC;
    }
    return 0;
}

void TestTone::setParam(uint16_t paramID, uint16_t paramValue)
{
    switch (paramID) {
    case PARAM_ID_TEST_A:
        paramA = paramValue;
        break;
    case PARAM_ID_TEST_B:
        paramB = paramValue;
        break;
    case PARAM_ID_TEST_C:
        paramC = paramValue;
        break;
    }
}

const ParamInfo TEST_PARAMS_INFO_ITEMS[] = {
    { PARAM_ID_TEST_A, PARAM_DEFAULT_TEST_A },
    { PARAM_ID_TEST_B, PARAM_DEFAULT_TEST_B },
    { PARAM_ID_TEST_C, PARAM_DEFAULT_TEST_C }
};

const ParamInfoList TEST_PARAMS_INFO = { 3, TEST_PARAMS_INFO_ITEMS };
