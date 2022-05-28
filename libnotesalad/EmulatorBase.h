#pragma once

#include <cstdint>

#include "resampler/speex_resampler.h"

class EmulatorBase {
public:
    EmulatorBase(uint32_t nativeSampleRate, uint32_t outputSampleRate);
    virtual ~EmulatorBase();
    uint32_t getNativeSampleRate();
    void getSamples(int16_t* output, int numSamples);

protected:
    virtual void getRawSamples(int16_t* output, int numSamples) = 0;

private:
    static const int SAMPLE_BUFFER_SAMPLES = 50;
    static const int SAMPLE_BUFFER_SIZE = SAMPLE_BUFFER_SAMPLES * 2;

    uint32_t nativeSampleRate;
    uint32_t outputSampleRate;
    SpeexResamplerState* resampler;
    int16_t resampleBuffer[SAMPLE_BUFFER_SIZE];
    int resampleBufferPos = SAMPLE_BUFFER_SAMPLES;
};
