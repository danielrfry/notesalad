export default class EmulatorBase {
    constructor(lib, hChip) {
        this._lib = lib;
        this._hChip = hChip;
        this._sampleBuffer = null;
        this._sampleBufferSize = 0;
        this._chipInitialised = true;
    }
    getSamples(offset, sampleCount, output) {
        // Resize buffer if needed
        if (sampleCount > this._sampleBufferSize) {
            if (this._sampleBuffer) {
                this._lib._free(this._sampleBuffer);
            }
            this._sampleBuffer = this._lib._calloc(sampleCount * 2, 2);
        }

        this._chipGetSamples(this._sampleBuffer, sampleCount);

        for (let i = 0; i < sampleCount; i++) {
            const sampleL =
                this._lib.getValue(this._sampleBuffer + i * 4, 'i16') / 32768;
            const sampleR =
                this._lib.getValue(this._sampleBuffer + i * 4 + 2, 'i16') /
                32768;

            if (output.length == 1) {
                output[0][i + offset] = (sampleL + sampleR) / 2;
            } else {
                output[0][i + offset] = sampleL;
                output[1][i + offset] = sampleR;
            }
        }
    }
    _chipGetSamples(buffer, sampleCount) {
        // Abstract
    }
    _chipClose() {
        // Abstract
    }
    close() {
        if (this._sampleBuffer) {
            this._lib._free(this._sampleBuffer);
            this._sampleBuffer = null;
        }
        if (this._chipInitialised) {
            this._chipClose();
            this._chipInitialised = false;
        }
    }
}
