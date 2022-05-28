#include <notesaladcore.h>

#include "../ParamInfo.h"

template <>
const ParamInfoList* getParamInfoList<OPMTone>()
{
    return &OPM_PARAMS_INFO;
}

INSTANTIATE_TYPED_TEST_SUITE_P(OPMParamInfoTest, ParamInfoTest, OPMTone);
