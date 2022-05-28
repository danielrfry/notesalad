import MIDIDriverBase from './MIDIDriverBase';

export default class OPL3MIDIDriver extends MIDIDriverBase {
    constructor(lib, hChip) {
        const hMIDI = lib._ntsld_opl3midi_new(0, hChip);
        super(lib, hMIDI);
    }
}
