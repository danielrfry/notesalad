#include "LFO.h"

#include <math.h>
#include <stdlib.h>

#define LFO_PRIV_FLAG_ONESHOT 1

LFO::LFO()
    : period(0), wave(0), timeOffset(0), rndLastUpdate(0), rndValue(0.0f), flags(0)
{
}

inline float getTriangleValue(float phase)
{
    if (phase > 0.5f) {
        return 2.0f - (phase * 2);
    } else {
        return (phase * 2);
    }
}

float LFO::getValue(Milliseconds time)
{
    if (this->period < 2) {
        return 0.0f;
    } else {
        uint16_t phaseMS = this->getPhase(time);
        float phase = phaseMS / (float)this->period;
        switch (this->wave) {
        case LFO_WAVE_TRIANGLE:
            return getTriangleValue(phase);
        case LFO_WAVE_SAW:
            return 1.0f - phase;
        case LFO_WAVE_RAMP:
            return phase;
        case LFO_WAVE_SQUARE:
            return (phase >= 0.5f) ? 1.0f : 0.0f;
        case LFO_WAVE_RANDOM:
            return this->getRandomValue(time);
        default:
            return 0.0f;
        }
    }
}

uint16_t LFO::getPhase(Milliseconds time)
{
    if (this->period == 0) {
        return 0;
    } else if (this->getOneShot() && time >= this->period) {
        return this->period;
    } else {
        Milliseconds offsetTime = time + this->timeOffset;
        return offsetTime % this->period;
    }
}

void LFO::setPeriod(Milliseconds time, uint16_t newPeriod)
{
    if (newPeriod != this->period) {
        if (this->period == 0) {
            this->timeOffset = newPeriod - (time % newPeriod);
        } else if (newPeriod == 0) {
            this->timeOffset = 0;
        } else {
            uint16_t oldPhase = this->getPhase(time);
            Milliseconds newPhase = time % newPeriod;
            uint32_t newOffset = newPeriod - newPhase;
            newOffset += (oldPhase * newPeriod) / this->period;
            newOffset = newOffset % newPeriod;
            this->timeOffset = newOffset;
        }
        this->period = newPeriod;
    }
}

void LFO::setWave(uint8_t newWave)
{
    this->wave = newWave;
}

bool LFO::getOneShot()
{
    return (this->flags & LFO_PRIV_FLAG_ONESHOT) != 0;
}

void LFO::setOneShot(bool oneShot)
{
    this->flags = oneShot ? this->flags | LFO_PRIV_FLAG_ONESHOT : this->flags & ~LFO_PRIV_FLAG_ONESHOT;
}

void LFO::reset()
{
    this->rndLastUpdate = 0;
    this->updateRandom();
    this->timeOffset = 0;
    this->period = 0;
}

float LFO::getRandomValue(Milliseconds time)
{
    if ((time - this->rndLastUpdate) >= this->period) {
        this->rndLastUpdate = time;
        this->updateRandom();
    }
    return this->rndValue;
}

void LFO::updateRandom()
{
    this->rndValue = rand() / (float)RAND_MAX;
}