from ctypes import c_void_p

from .opl import OPLDeviceBase
from . import MIDIBase, SD1DelayCallback, SD1WriteCallback, _lib, c_uint8_t


class SD1DeviceBase:
    def __init__(self, h_device):
        self.h_device = h_device

    def __del__(self):
        _lib.ntsld_sd1_delete(self.h_device)


class SD1CallbackDevice(SD1DeviceBase):
    def __init__(self, write_callback, delay_callback):
        def call_write(_, data, data_len):
            byte_array = c_uint8_t * data_len
            write_callback(byte_array.from_address(data))

        def call_delay(_, ms):
            delay_callback(ms)

        self._c_call_write = SD1WriteCallback(call_write)
        self._c_call_delay = SD1DelayCallback(call_delay)

        super().__init__(_lib.ntsld_sd1_cbkdev_new(
            c_void_p(0), self._c_call_write, self._c_call_delay))


class SD1OPLDevice(OPLDeviceBase):
    def __init__(self, sd1_device):
        self.sd1_device = sd1_device
        super().__init__(_lib.ntsld_oplsd1_new(sd1_device.h_device))


class SD1MIDI(MIDIBase):
    def __init__(self, device):
        self.device = device
        super().__init__(_lib.ntsld_sd1midi_new(c_void_p(0), device.h_device))
