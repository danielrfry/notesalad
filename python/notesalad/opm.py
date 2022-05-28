from ctypes import c_void_p
from . import MIDIBase, OPMResetCallback, OPMWriteCallback, _lib, opm_emu_getsamples


DEFAULT_SAMPLE_RATE = 55930


class OPMDeviceBase:
    def __init__(self, h_device):
        self.h_device = h_device

    def __del__(self):
        _lib.ntsld_opm_delete(self.h_device)

    def reset(self):
        _lib.ntsld_opm_reset(self.h_device)

    def write(self, addr, data):
        _lib.ntsld_opm_write(self.h_device, addr, data)


class OPMEmulator(OPMDeviceBase):
    def __init__(self, sample_rate=DEFAULT_SAMPLE_RATE):
        super().__init__(_lib.ntsld_opm_emu_new(sample_rate))

    def get_samples(self, buffer, offset=0, sample_count=None):
        opm_emu_getsamples(self.h_device, buffer, offset, sample_count)


class OPMCallbackDevice(OPMDeviceBase):
    def __init__(self, write_callback, reset_callback):
        def call_write(_, reg, value):
            if write_callback is not None:
                write_callback(reg, value)

        def call_reset(_):
            if reset_callback is not None:
                reset_callback()

        self._c_call_write = OPMWriteCallback(call_write)
        self._c_call_reset = OPMResetCallback(call_reset)

        super().__init__(_lib.ntsld_opm_cbkdev_new(
            c_void_p(0), self._c_call_write, self._c_call_reset))


class OPMMIDI(MIDIBase):
    def __init__(self, device):
        self.device = device
        super().__init__(_lib.ntsld_opmmidi_new(c_void_p(0), device.h_device))
