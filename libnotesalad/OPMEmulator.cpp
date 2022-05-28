#include "OPMEmulator.h"

#define OPM_NATIVE_SAMPLE_RATE 55930

OPMEmulator::OPMEmulator(uint32_t outputSampleRate)
    : EmulatorBase(OPM_NATIVE_SAMPLE_RATE, outputSampleRate), chipInterface(), chip(chipInterface)
{
    chip.reset();
}

OPMEmulator::~OPMEmulator()
{
}

void OPMEmulator::hardReset()
{
    chip.reset();
}

void OPMEmulator::write(uint8_t addr, uint8_t data)
{
    this->writeQueue.push_back(std::make_pair(addr, data));
}

void OPMEmulator::getRawSamples(int16_t* output, int numSamples)
{
    for (int i = 0; i < numSamples; i++) {
        if (!this->writeQueue.empty()) {
            auto nextWrite = this->writeQueue.front();
            auto addr = nextWrite.first;
            auto data = nextWrite.second;
            this->writeQueue.erase(this->writeQueue.begin());

            chip.write_address(addr);
            chip.write_data(data);
        }
        ymfm::ym2151::output_data chipOutput;
        chip.generate(&chipOutput);
        output[i * 2] = chipOutput.data[0];
        output[i * 2 + 1] = chipOutput.data[1];
    }
}
