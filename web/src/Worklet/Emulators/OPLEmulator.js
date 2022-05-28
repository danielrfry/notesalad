import EmulatorBase from './EmulatorBase';

export default class OPLEmulator extends EmulatorBase {
    constructor(lib) {
        const hChip = lib._ntsld_opl_emu_new(sampleRate);
        super(lib, hChip);
        this.write(0x105, 1);
    }
    reset() {
        this._lib._ntsld_opl_reset(this._hChip, reg, value);
    }
    write(reg, value) {
        this._lib._ntsld_opl_write(this._hChip, reg, value);
    }
    _chipInit() {
        this._hChip = this._lib._ntsld_opl_emu_new(sampleRate);
    }
    _chipGetSamples(buffer, sampleCount) {
        this._lib._ntsld_opl_emu_getsamples(this._hChip, buffer, sampleCount);
    }
    _chipClose() {
        this._lib._ntsld_opl_delete(this._hChip);
    }
}
