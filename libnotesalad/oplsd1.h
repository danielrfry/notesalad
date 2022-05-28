#pragma once

#include "opl.h"
#include "sd1.h"

extern "C" {
    OPLDeviceRef ntsld_oplsd1_new(SD1DeviceRef sd1Device);
}