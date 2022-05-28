#include <notesaladcore.h>

#include "../ParamInfo.h"

template <>
const ParamInfoList* getParamInfoList<SD1Tone>()
{
    return &SD1_PARAMS_INFO;
}

INSTANTIATE_TYPED_TEST_SUITE_P(SD1ParamInfoTest, ParamInfoTest, SD1Tone);
