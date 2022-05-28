#pragma once

#include "EmulatorBase.h"
#include "OPM/OPMDeviceBase.h"
#include "ymfm/src/ymfm_opm.h"

#include <utility>
#include <vector>

class OPMEmulator : public EmulatorBase, public OPMDeviceBase {
public:
    OPMEmulator(uint32_t outputSampleRate);
    virtual ~OPMEmulator();
    virtual void hardReset();
    virtual void write(uint8_t addr, uint8_t data);

protected:
    virtual void getRawSamples(int16_t* output, int numSamples);

private:
    ymfm::ymfm_interface chipInterface;
    ymfm::ym2151 chip;
    std::vector<std::pair<uint8_t, uint8_t>> writeQueue;
};
