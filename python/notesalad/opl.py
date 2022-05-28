from . import MIDIBase, OPLResetCallback, OPLWriteCallback, _lib, opl_emu_getsamples
from ctypes import c_void_p


DEFAULT_SAMPLE_RATE = 49716


class OPLDeviceBase:
    def __init__(self, h_device):
        self.h_device = h_device

    def __del__(self):
        _lib.ntsld_opl_delete(self.h_device)

    def reset(self):
        _lib.ntsld_opl_reset(self.h_device)

    def write(self, addr, data):
        _lib.ntsld_opl_write(self.h_device, addr, data)


class OPLEmulator(OPLDeviceBase):
    def __init__(self, sample_rate=DEFAULT_SAMPLE_RATE):
        super().__init__(_lib.ntsld_opl_emu_new(sample_rate))

    def get_samples(self, buffer, offset=0, sample_count=None):
        opl_emu_getsamples(self.h_device, buffer, offset, sample_count)


class OPLCallbackDevice(OPLDeviceBase):
    def __init__(self, write_callback, reset_callback):
        def call_write(_, reg, value):
            if write_callback is not None:
                write_callback(reg, value)

        def call_reset(_):
            if reset_callback is not None:
                reset_callback()

        self._c_call_write = OPLWriteCallback(call_write)
        self._c_call_reset = OPLResetCallback(call_reset)

        super().__init__(_lib.ntsld_opl_cbkdev_new(
            c_void_p(0), self._c_call_write, self._c_call_reset))


class OPL2MIDI(MIDIBase):
    def __init__(self, device):
        self.device = device
        super().__init__(_lib.ntsld_opl2midi_new(c_void_p(0), device.h_device))


class OPL3MIDI(MIDIBase):
    def __init__(self, device):
        self.device = device
        super().__init__(_lib.ntsld_opl3midi_new(c_void_p(0), device.h_device))
