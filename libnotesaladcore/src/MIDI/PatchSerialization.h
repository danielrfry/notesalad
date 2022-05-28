#pragma once

#include "../7BitEncoding.h"
#include "../Utils.h"
#include "ParamInfo.h"
#include "Patch.h"

#include <cstdint>

namespace PatchSerialization {

template <class Tone>
bool serialize(const ParamInfoList& chipParamsInfo, Patch<Tone>& patch, IWriter* dest)
{
    const ParamInfoList* paramInfoLists[] = {
        &chipParamsInfo, &UNIVERSAL_PARAMS_INFO
    };

    uint16_t lastParamID = 0;
    for (uint16_t l = 0; l < 2; l++) {
        auto& paramInfoList = *(paramInfoLists[l]);
        for (uint16_t i = 0; i < paramInfoList.numParams; i++) {
            auto& paramInfo = paramInfoList.paramInfo[i];
            auto paramID = paramInfo.id;
            auto paramValue = patch.getParam(paramID);
            if (paramValue != paramInfo.defaultValue) {
                auto paramIDDiff = paramID - lastParamID;
                if (!encodeLEB128(paramIDDiff, dest)) {
                    return false;
                }
                if (!encodeLEB128(paramValue, dest)) {
                    return false;
                }
                lastParamID = paramID;
            }
        }
    }

    return true;
}

template <class Tone>
bool deserialize(Patch<Tone>& outPatch, IReader* src)
{
    Patch<Tone> patch;

    uint16_t lastParamID = 0;
    while (!src->eof()) {
        uint16_t paramID;
        if (!decodeLEB128(paramID, src)) {
            return false;
        }
        paramID += lastParamID;

        uint16_t paramValue;
        if (!decodeLEB128(paramValue, src)) {
            return false;
        }

        patch.setParam(paramID, paramValue);

        lastParamID = paramID;
    }

    outPatch = patch;

    return true;
}

};
