export default class MIDIDriverBase {
    constructor(lib, hMIDI) {
        this._lib = lib;
        this._hMIDI = hMIDI;
        this._pReceiveMIDI = lib.addFunction(
            (...args) => this._receiveMIDI(...args),
            'viii'
        );
        this.onReceiveMIDI = null;
        lib._ntsld_midi_set_recv_cbk(this._hMIDI, 0, this._pReceiveMIDI);
    }

    reset() {
        this._lib._ntsld_midi_reset(this._hMIDI);
    }

    send(data) {
        const buffer = this._lib._calloc(data.length, 1);
        for (let i = 0; i < data.length; i++) {
            this._lib.setValue(buffer + i, data[i], 'i8');
        }
        this._lib._ntsld_midi_send(this._hMIDI, buffer, data.length);
        this._lib._free(buffer);
    }

    setTime(timeMS) {
        this._lib._ntsld_midi_set_time(this._hMIDI, timeMS);
    }

    update() {
        this._lib._ntsld_midi_update(this._hMIDI);
    }

    close() {
        if (this._hMIDI) {
            this._lib._ntsld_midi_delete(this._hMIDI);
            this._lib.removeFunction(this._pReceiveMIDI);
            this._hMIDI = null;
        }
    }

    _receiveMIDI(_, pMsgData, msgLength) {
        if (this.onReceiveMIDI) {
            const msgData = new Uint8Array(msgLength);
            msgData.set(
                this._lib.HEAPU8.subarray(pMsgData, pMsgData + msgLength)
            );
            this.onReceiveMIDI(msgData);
        }
    }
}
