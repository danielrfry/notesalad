#include "OPLEmulator.h"

#include <iostream>

#define OPL_NATIVE_SAMPLE_RATE 49716

OPLEmulator::OPLEmulator(uint32_t outputSampleRate)
    : EmulatorBase(OPL_NATIVE_SAMPLE_RATE, outputSampleRate), chipInterface(), chip(chipInterface)
{
    chip.reset();
}

OPLEmulator::~OPLEmulator()
{
}

void OPLEmulator::hardReset()
{
    chip.reset();
}

void OPLEmulator::write(uint16_t addr, uint8_t data)
{
    this->writeQueue.push_back(std::make_pair(addr, data));
}

void OPLEmulator::getRawSamples(int16_t* output, int numSamples)
{
    for (int i = 0; i < numSamples; i++) {
        if (!this->writeQueue.empty()) {
            auto nextWrite = this->writeQueue.front();
            auto addr = nextWrite.first;
            auto data = nextWrite.second;
            this->writeQueue.erase(this->writeQueue.begin());

            if (addr & 0x100) {
                chip.write_address_hi(addr & 0xff);
            } else {
                chip.write_address(addr & 0xff);
            }
            chip.write_data(data);
        }

        ymfm::ymf262::output_data chipOutput;
        chip.generate(&chipOutput);
        output[i * 2] = chipOutput.data[0];
        output[i * 2 + 1] = chipOutput.data[1];
    }
}
