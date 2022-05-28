#pragma once

#include "EmulatorBase.h"
#include "OPL/OPLDeviceBase.h"
#include "ymfm/src/ymfm_opl.h"

#include <utility>
#include <vector>

class OPLEmulator : public EmulatorBase, public OPLDeviceBase {
public:
    OPLEmulator(uint32_t outputSampleRate);
    virtual ~OPLEmulator();
    virtual void hardReset();
    virtual void write(uint16_t addr, uint8_t data);

protected:
    virtual void getRawSamples(int16_t* output, int numSamples);

private:
    ymfm::ymfm_interface chipInterface;
    ymfm::ymf262 chip;
    std::vector<std::pair<uint16_t, uint8_t>> writeQueue;
};
