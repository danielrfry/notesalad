#pragma once

#include <cstdint>

#include "../Utils.h"
#include "LFO.h"
#include "PatchParams.h"

#define NUM_PARAM_MAPS 4
#define NUM_LFOS 2

#define PARAMMAP_SRC_NONE 255

#define PARAMMAP_PARAM_START 0x3000
#define PARAMMAP_PARAM_END 0x30ff

#define PARAMMAP_PARAM_SRC 0
#define PARAMMAP_PARAM_DESTPARAM 1
#define PARAMMAP_PARAM_ADJUST_AMOUNT 2

#define LFO_PARAM_START 0x3100
#define LFO_PARAM_END 0x31ff

#define LFO_PARAM_WAVE 0
#define LFO_PARAM_PERIOD 1
#define LFO_PARAM_SYNC 2
#define LFO_PARAM_ONESHOT 3

typedef struct ParamMap {
    uint8_t src = PARAMMAP_SRC_NONE;
    uint16_t destParam = 0;
    int16_t adjustmentAmount = 0;
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
        return (paramID >= PARAMMAP_PARAM_START) && (paramID <= PARAMMAP_PARAM_END);
    }
    static bool isLFOParam(uint16_t paramID)
    {
        return (paramID >= LFO_PARAM_START) && (paramID <= LFO_PARAM_END);
    }

private:
    void getParamMapIndex(uint16_t paramID, uint8_t& paramMapIndex, uint8_t& paramMapItem)
    {
        paramMapIndex = (paramID - PARAMMAP_PARAM_START) / 0x10;
        paramMapItem = (paramID - PARAMMAP_PARAM_START) % 0x10;
    }

    void getLFOIndex(uint16_t paramID, uint8_t& lfoIndex, uint8_t& lfoItem)
    {
        lfoIndex = (paramID - LFO_PARAM_START) / 0x10;
        lfoItem = (paramID - LFO_PARAM_START) % 0x10;
    }

    uint16_t getParamMapParam(uint16_t paramID)
    {
        uint8_t paramMapIndex, paramMapItem;
        this->getParamMapIndex(paramID, paramMapIndex, paramMapItem);

        if (paramMapIndex >= NUM_PARAM_MAPS)
            return 0;

        switch (paramMapItem) {
        case PARAMMAP_PARAM_SRC:
            return this->paramMaps[paramMapIndex].src;
        case PARAMMAP_PARAM_DESTPARAM:
            return this->paramMaps[paramMapIndex].destParam;
        case PARAMMAP_PARAM_ADJUST_AMOUNT:
            return this->paramMaps[paramMapIndex].adjustmentAmount + 0x2000;
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
        case PARAMMAP_PARAM_SRC:
            this->paramMaps[paramMapIndex].src = clamp<uint16_t>(value, 0, 255);
            break;
        case PARAMMAP_PARAM_DESTPARAM:
            this->paramMaps[paramMapIndex].destParam = clamp<uint16_t>(value, 0, 0x3fff);
            break;
        case PARAMMAP_PARAM_ADJUST_AMOUNT:
            this->paramMaps[paramMapIndex].adjustmentAmount = clamp<uint16_t>(value, 0, 0x3fff) - 0x2000;
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
        case LFO_PARAM_WAVE:
            return this->lfoParams[lfoIndex].wave;
        case LFO_PARAM_PERIOD:
            return this->lfoParams[lfoIndex].period;
        case LFO_PARAM_SYNC:
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
        case LFO_PARAM_WAVE:
            this->lfoParams[lfoIndex].wave = clamp<uint16_t>(value, 0, LFO_WAVE_MAX);
            break;
        case LFO_PARAM_PERIOD:
            this->lfoParams[lfoIndex].period = value;
            break;
        case LFO_PARAM_SYNC:
            this->lfoParams[lfoIndex].sync = (value != 0);
            break;
        case LFO_PARAM_ONESHOT:
            this->lfoParams[lfoIndex].oneShot = (value != 0);
            break;
        }
    }
};
