#include "oplsd1.h"
#include "SD1/SD1OPLAdaptor.h"

OPLDeviceRef ntsld_oplsd1_new(SD1DeviceRef sd1Device)
{
    return new SD1OPLAdaptor((SD1DeviceBase*)sd1Device);
}