#pragma once

#include <cstdint>

#include "../Utils.h"
#include "LFO.h"
#include "PatchParams.h"

#define NUM_PARAM_MAPS 4
#define NUM_LFOS 2

#define PARAMMAP_FLAG_INVERT_SRC 1

typedef struct ParamMap {
    uint8_t src = PARAMMAP_SRC_NONE;
    uint16_t destParam = 0;
    int16_t adjustmentAmount = 0;
    uint8_t flags = 0;
} ParamMap;

typedef struct LFOParams {
    uint8_t wave = 0;
    uint16_t period = 0;
    bool sync = false;
    bool oneShot = false;
} LFOParams;

template <class Tone>
class Patch {
public:
    PatchParams<Tone> params;
    ParamMap paramMaps[NUM_PARAM_MAPS];
    LFOParams lfoParams[NUM_LFOS];
    uint16_t getParam(uint16_t paramID)
    {
        if (isParamMapParam(paramID)) {
            return this->getParamMapParam(paramID);
        } else if (isLFOParam(paramID)) {
            return this->getLFOParam(paramID);
        } else {
            return this->params.getParam(paramID);
        }
    }
    void setParam(uint16_t paramID, uint16_t value)
    {
        if (isParamMapParam(paramID)) {
            this->setParamMapParam(paramID, value);
        } else if (isLFOParam(paramID)) {
            this->setLFOParam(paramID, value);
        } else {
            this->params.setParam(paramID, value);
        }
    }
    static bool isParamMapParam(uint16_t paramID)
    {
        return (paramID >= PARAM_PARAMMAP_START) && (paramID <= PARAM_PARAMMAP_END);
    }
    static bool isLFOParam(uint16_t paramID)
    {
        return (paramID >= PARAM_LFO_START) && (paramID <= PARAM_LFO_END);
    }

private:
    void getParamMapIndex(uint16_t paramID, uint8_t& paramMapIndex, uint8_t& paramMapItem)
    {
        paramMapIndex = (paramID - PARAM_PARAMMAP_START) / 0x10;
        paramMapItem = (paramID - PARAM_PARAMMAP_START) % 0x10;
    }

    void getLFOIndex(uint16_t paramID, uint8_t& lfoIndex, uint8_t& lfoItem)
    {
        lfoIndex = (paramID - PARAM_LFO_START) / 0x10;
        lfoItem = (paramID - PARAM_LFO_START) % 0x10;
    }

    uint16_t getParamMapParam(uint16_t paramID)
    {
        uint8_t paramMapIndex, paramMapItem;
        this->getParamMapIndex(paramID, paramMapIndex, paramMapItem);

        if (paramMapIndex >= NUM_PARAM_MAPS)
            return 0;

        switch (paramMapItem) {
        case PARAM_PARAMMAP_SRC:
            return this->paramMaps[paramMapIndex].src;
        case PARAM_PARAMMAP_DESTPARAM:
            return this->paramMaps[paramMapIndex].destParam;
        case PARAM_PARAMMAP_ADJUST_AMOUNT:
            return this->paramMaps[paramMapIndex].adjustmentAmount + 0x2000;
        case PARAM_PARAMMAP_INVERT_SRC:
            return this->paramMaps[paramMapIndex].flags & PARAMMAP_FLAG_INVERT_SRC ? 1 : 0;
        default:
            return 0;
        }
    }
    void setParamMapParam(uint16_t paramID, uint16_t value)
    {
        uint8_t paramMapIndex, paramMapItem;
        this->getParamMapIndex(paramID, paramMapIndex, paramMapItem);

        if (paramMapIndex >= NUM_PARAM_MAPS)
            return;

        switch (paramMapItem) {
        case PARAM_PARAMMAP_SRC:
            this->paramMaps[paramMapIndex].src = clamp<uint16_t>(value, 0, 255);
            break;
        case PARAM_PARAMMAP_DESTPARAM:
            this->paramMaps[paramMapIndex].destParam = clamp<uint16_t>(value, 0, 0x3fff);
            break;
        case PARAM_PARAMMAP_ADJUST_AMOUNT:
            this->paramMaps[paramMapIndex].adjustmentAmount = clamp<uint16_t>(value, 0, 0x3fff) - 0x2000;
            break;
        case PARAM_PARAMMAP_INVERT_SRC:
            setBit<PARAMMAP_FLAG_INVERT_SRC>(this->paramMaps[paramMapIndex].flags, value != 0);
            break;
        }
    }
    uint16_t getLFOParam(uint16_t paramID)
    {
        uint8_t lfoIndex, lfoItem;
        this->getLFOIndex(paramID, lfoIndex, lfoItem);
        if (lfoIndex >= NUM_LFOS) {
            return 0;
        }

        switch (lfoItem) {
        case PARAM_LFO_WAVE:
            return this->lfoParams[lfoIndex].wave;
        case PARAM_LFO_PERIOD:
            return this->lfoParams[lfoIndex].period;
        case PARAM_LFO_SYNC:
            return this->lfoParams[lfoIndex].sync ? 1 : 0;
        }
        return 0;
    }
    void setLFOParam(uint16_t paramID, uint16_t value)
    {
        uint8_t lfoIndex, lfoItem;
        this->getLFOIndex(paramID, lfoIndex, lfoItem);

        if (lfoIndex >= NUM_LFOS) {
            return;
        }

        switch (lfoItem) {
        case PARAM_LFO_WAVE:
            this->lfoParams[lfoIndex].wave = clamp<uint16_t>(value, 0, LFO_WAVE_MAX);
            break;
        case PARAM_LFO_PERIOD:
            this->lfoParams[lfoIndex].period = value;
            break;
        case PARAM_LFO_SYNC:
            this->lfoParams[lfoIndex].sync = (value != 0);
            break;
        case PARAM_LFO_ONESHOT:
            this->lfoParams[lfoIndex].oneShot = (value != 0);
            break;
        }
    }
};
