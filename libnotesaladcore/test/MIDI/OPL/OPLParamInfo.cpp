#include <notesaladcore.h>

#include "../ParamInfo.h"

template <>
const ParamInfoList* getParamInfoList<OPLTone>()
{
    return &OPL_PARAMS_INFO;
}

INSTANTIATE_TYPED_TEST_SUITE_P(OPLParamInfoTest, ParamInfoTest, OPLTone);
