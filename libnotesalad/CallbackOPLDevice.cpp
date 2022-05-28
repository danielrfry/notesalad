#include "CallbackOPLDevice.h"

CallbackOPLDevice::CallbackOPLDevice(void* context,
    OPLWriteCallback writeCallback,
    OPLResetCallback resetCallback)
    : context(context), writeCallback(writeCallback), resetCallback(resetCallback)
{
}

CallbackOPLDevice::~CallbackOPLDevice()
{
}

void CallbackOPLDevice::hardReset()
{
    resetCallback(this->context);
}

void CallbackOPLDevice::write(uint16_t addr, uint8_t data)
{
    writeCallback(this->context, addr, data);
}
