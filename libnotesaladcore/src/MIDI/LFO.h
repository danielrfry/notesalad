#pragma once

#include "TimeSource.h"

#define LFO_WAVE_TRIANGLE 0
#define LFO_WAVE_SAW 1
#define LFO_WAVE_RAMP 2
#define LFO_WAVE_SQUARE 3
#define LFO_WAVE_RANDOM 4

#define LFO_WAVE_MAX 4

class LFO {
public:
    LFO();
    float getValue(Milliseconds time);
    void setPeriod(Milliseconds time, uint16_t newPeriod);
    void setWave(uint8_t newWave);
    bool getOneShot();
    void setOneShot(bool oneShot);
    void reset();

private:
    uint16_t period;
    uint8_t wave;
    uint16_t timeOffset;
    Milliseconds rndLastUpdate;
    float rndValue;
    uint8_t flags;
    uint16_t getPhase(Milliseconds time);
    float getRandomValue(Milliseconds time);
    void updateRandom();
};
