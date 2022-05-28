#include "CallbackOPMDevice.h"

CallbackOPMDevice::CallbackOPMDevice(void* context,
    OPMWriteCallback writeCallback,
    OPMResetCallback resetCallback)
    : context(context), writeCallback(writeCallback), resetCallback(resetCallback)
{
}

CallbackOPMDevice::~CallbackOPMDevice()
{
}

void CallbackOPMDevice::hardReset()
{
    resetCallback(this->context);
}

void CallbackOPMDevice::write(uint8_t addr, uint8_t data)
{
    writeCallback(this->context, addr, data);
}
