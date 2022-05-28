#include "EmulatorBase.h"

EmulatorBase::EmulatorBase(uint32_t nativeSampleRate, uint32_t outputSampleRate)
{
    this->nativeSampleRate = nativeSampleRate;
    this->outputSampleRate = outputSampleRate;
    this->resampler = ntsld_resampler_init(2, nativeSampleRate, outputSampleRate, 5, nullptr);
}

EmulatorBase::~EmulatorBase()
{
    ntsld_resampler_destroy(this->resampler);
}

uint32_t EmulatorBase::getNativeSampleRate()
{
    return this->nativeSampleRate;
}

void EmulatorBase::getSamples(int16_t* output, int numSamples)
{
    if (this->nativeSampleRate == this->outputSampleRate) {
        return this->getRawSamples(output, numSamples);
    }

    while (numSamples > 0) {
        if (this->resampleBufferPos < SAMPLE_BUFFER_SAMPLES) {
            uint32_t inLen = SAMPLE_BUFFER_SAMPLES - this->resampleBufferPos;
            uint32_t outLen = numSamples;
            ntsld_resampler_process_interleaved_int(
                this->resampler,
                &this->resampleBuffer[this->resampleBufferPos * 2],
                &inLen,
                output,
                &outLen);
            output += (outLen * 2);
            numSamples -= outLen;
            this->resampleBufferPos += inLen;
        } else {
            this->getRawSamples(&this->resampleBuffer[0], SAMPLE_BUFFER_SAMPLES);
            this->resampleBufferPos = 0;
        }
    }
}