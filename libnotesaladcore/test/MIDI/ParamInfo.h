#pragma once

#include <gtest/gtest.h>
#include <iomanip>
#include <notesaladcore.h>

template <class ToneData>
const ParamInfoList* getParamInfoList();

template <class T>
class ParamInfoTest : public testing::Test {
};

TYPED_TEST_SUITE_P(ParamInfoTest);

TYPED_TEST_P(ParamInfoTest, MatchesDefaultToneData)
{
    TypeParam defaultTone;
    const ParamInfoList* paramInfoList = getParamInfoList<TypeParam>();
    for (uint16_t i = 0; i < paramInfoList->numParams; i++) {
        auto& paramInfo = paramInfoList->paramInfo[i];
        auto paramID = paramInfo.id;
        auto expectedValue = paramInfo.defaultValue;
        auto actualValue = defaultTone.getParam(paramID);
        EXPECT_EQ(expectedValue, actualValue) << "Expected: " << expectedValue << " Actual: " << actualValue << " for parameter: " << std::showbase << std::hex << paramID;
    }
}

TYPED_TEST_P(ParamInfoTest, GetsAndSetsParams)
{
    TypeParam tone;
    const ParamInfoList* paramInfoList = getParamInfoList<TypeParam>();
    for (uint16_t i = 0; i < paramInfoList->numParams; i++) {
        auto& paramInfo = paramInfoList->paramInfo[i];
        auto paramID = paramInfo.id;
        auto newValue = paramInfo.defaultValue == 0 ? 1 : paramInfo.defaultValue - 1;
        tone.setParam(paramID, newValue);
        auto readValue = tone.getParam(paramID);
        EXPECT_EQ(readValue, newValue) << "Failed to get/set parameter: " << std::showbase << std::hex << paramID;
    }
}

REGISTER_TYPED_TEST_SUITE_P(ParamInfoTest, MatchesDefaultToneData, GetsAndSetsParams);
