import EmulatorBase from './EmulatorBase';

export default class OPMEmulator extends EmulatorBase {
    constructor(lib) {
        const hChip = lib._ntsld_opm_emu_new(sampleRate);
        super(lib, hChip);
    }
    reset() {
        this._lib._ntsld_opm_reset(this._hChip, reg, value);
    }
    write(reg, value) {
        this._lib._ntsld_opm_write(this._hChip, reg, value);
    }
    _chipInit() {
        this._hChip = this._lib._ntsld_opm_emu_new(sampleRate);
    }
    _chipGetSamples(buffer, sampleCount) {
        this._lib._ntsld_opm_emu_getsamples(this._hChip, buffer, sampleCount);
    }
    _chipClose() {
        this._lib._ntsld_opm_delete(this._hChip);
    }
}
