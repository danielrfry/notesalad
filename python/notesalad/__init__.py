"""Device-independent MIDI implementation"""

__version__ = "0.5.0"

from ctypes import c_int, cdll, CFUNCTYPE, c_bool, c_void_p, c_ushort, c_ubyte, c_uint
import sys

if sys.platform == 'darwin':
    _lib = cdll.LoadLibrary('libnotesalad.dylib')
else:
    _lib = cdll.LoadLibrary('libnotesalad.so')

# Integer types
c_uint32_t = c_uint
c_uint16_t = c_ushort
c_uint8_t = c_ubyte

# MIDI
MIDIRef = c_void_p
MIDIReceiveCallback = CFUNCTYPE(None, c_void_p, c_void_p, c_uint32_t)

_lib.ntsld_midi_set_recv_cbk.argtypes = (
    MIDIRef, c_void_p, MIDIReceiveCallback)
_lib.ntsld_midi_set_recv_cbk.restype = None
_lib.ntsld_midi_send.argtypes = (MIDIRef, c_void_p, c_uint32_t)
_lib.ntsld_midi_send.restype = None
_lib.ntsld_midi_set_time.argtypes = (MIDIRef, c_uint32_t)
_lib.ntsld_midi_set_time.restype = None
_lib.ntsld_midi_update.argtypes = (MIDIRef,)
_lib.ntsld_midi_update.restype = None
_lib.ntsld_midi_reset.argtypes = (MIDIRef,)
_lib.ntsld_midi_reset.restype = None
_lib.ntsld_midi_delete.argtypes = (MIDIRef,)
_lib.ntsld_midi_delete.restype = None

# OPL
OPLDeviceRef = c_void_p
OPL2MIDIRef = MIDIRef
OPL3MIDIRef = MIDIRef
OPLWriteCallback = CFUNCTYPE(None, c_void_p, c_uint16_t, c_uint8_t)
OPLResetCallback = CFUNCTYPE(None, c_void_p)

_lib.ntsld_opl_emu_new.argtypes = (c_uint32_t,)
_lib.ntsld_opl_emu_new.restype = OPLDeviceRef
_lib.ntsld_opl_cbkdev_new.argtypes = (
    c_void_p, OPLWriteCallback, OPLResetCallback)
_lib.ntsld_opl_cbkdev_new.restype = OPLDeviceRef

_lib.ntsld_opl_delete.argtypes = (OPLDeviceRef,)
_lib.ntsld_opl_delete.restype = None
_lib.ntsld_opl_reset.argtypes = (OPLDeviceRef,)
_lib.ntsld_opl_reset.restype = None
_lib.ntsld_opl_write.argtypes = (OPLDeviceRef, c_uint16_t, c_uint8_t)
_lib.ntsld_opl_write.restype = None

_lib.ntsld_opl_emu_getsamples.argtypes = (OPLDeviceRef, c_void_p, c_int)
_lib.ntsld_opl_emu_getsamples.restype = None

_lib.ntsld_opl2midi_new.argtypes = (c_void_p, OPLDeviceRef)
_lib.ntsld_opl2midi_new.restype = MIDIRef
_lib.ntsld_opl3midi_new.argtypes = (c_void_p, OPLDeviceRef)
_lib.ntsld_opl3midi_new.restype = MIDIRef

# SD-1
SD1DeviceRef = c_void_p

# OPL/SD-1 convertor
_lib.ntsld_oplsd1_new.argtypes = (SD1DeviceRef,)
_lib.ntsld_oplsd1_new.restype = OPLDeviceRef

# OPM
OPMDeviceRef = c_void_p
OPMMIDIRef = MIDIRef
OPMWriteCallback = CFUNCTYPE(None, c_void_p, c_ushort, c_ubyte)
OPMResetCallback = CFUNCTYPE(None, c_void_p)

_lib.ntsld_opm_emu_new.argtypes = (c_uint32_t,)
_lib.ntsld_opm_emu_new.restype = OPMDeviceRef
_lib.ntsld_opm_cbkdev_new.argtypes = (
    c_void_p, OPMWriteCallback, OPMResetCallback)
_lib.ntsld_opm_cbkdev_new.restype = OPMDeviceRef

_lib.ntsld_opm_delete.argtypes = (OPMDeviceRef,)
_lib.ntsld_opm_delete.restype = None
_lib.ntsld_opm_reset.argtypes = (OPMDeviceRef,)
_lib.ntsld_opm_reset.restype = None
_lib.ntsld_opm_write.argtypes = (OPMDeviceRef, c_uint8_t, c_uint8_t)
_lib.ntsld_opm_write.restype = None

_lib.ntsld_opm_emu_getsamples.argtypes = (OPMDeviceRef, c_void_p, c_int)
_lib.ntsld_opm_emu_getsamples.restype = None

_lib.ntsld_opmmidi_new.argtypes = (c_void_p, OPMDeviceRef)
_lib.ntsld_opmmidi_new.restype = OPMMIDIRef

# SD-1
SD1DeviceRef = c_void_p
SD1MIDIRef = MIDIRef
SD1WriteCallback = CFUNCTYPE(None, c_void_p, c_void_p, c_uint16_t)
SD1DelayCallback = CFUNCTYPE(None, c_void_p, c_uint32_t)

_lib.ntsld_sd1_cbkdev_new.argtypes = (
    c_void_p, SD1WriteCallback, SD1DelayCallback)
_lib.ntsld_sd1_cbkdev_new.restype = SD1DeviceRef
_lib.ntsld_sd1_delete.argtypes = (SD1DeviceRef,)
_lib.ntsld_sd1_delete.restype = None
_lib.ntsld_sd1midi_new.argtypes = (c_void_p, SD1DeviceRef)
_lib.ntsld_sd1midi_new.restype = SD1MIDIRef


class MIDIBase:
    def __init__(self, hMIDI):
        self._hMIDI = hMIDI

    def __del__(self):
        self.close()

    def reset(self):
        _lib.ntsld_midi_reset(self._hMIDI)

    def send(self, message):
        BUFTYPE = c_ubyte * len(message)
        buffer = BUFTYPE(*message)
        _lib.ntsld_midi_send(self._hMIDI, buffer, len(message))

    def set_time(self, time_ms):
        _lib.ntsld_midi_set_time(self._hMIDI, time_ms)

    def update(self):
        _lib.ntsld_midi_update(self._hMIDI)

    def close(self):
        _lib.ntsld_midi_delete(self._hMIDI)
        self._hMIDI = None


def _emu_get_samples(getsamples_fn, h_device, buffer, offset=0, sample_count=None):
    if sample_count is None:
        sample_count = (len(buffer) // 4) - offset
    byte_array = c_uint8_t * (sample_count * 4)
    getsamples_fn(h_device, byte_array.from_buffer(
        buffer, offset * 4), sample_count)


def opl_emu_getsamples(h_device, buffer, offset=0, sample_count=None):
    _emu_get_samples(_lib.ntsld_opl_emu_getsamples,
                     h_device, buffer, offset, sample_count)


def opm_emu_getsamples(h_device, buffer, offset=0, sample_count=None):
    _emu_get_samples(_lib.ntsld_opm_emu_getsamples,
                     h_device, buffer, offset, sample_count)
