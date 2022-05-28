#include "CallbackSD1Device.h"

CallbackSD1Device::CallbackSD1Device(void* context, SD1WriteCallback writeCbk,
    SD1DelayCallback delayCbk) : context(context),
                                 writeCbk(writeCbk), delayCbk(delayCbk)
{
}

CallbackSD1Device::~CallbackSD1Device()
{
}

void CallbackSD1Device::delayMicroseconds(uint32_t us)
{
    this->delayCbk(context, us);
}

void CallbackSD1Device::write(const uint8_t* data, const uint16_t len)
{
    this->writeCbk(context, data, len);
}

void CallbackSD1Device::hardReset()
{
}