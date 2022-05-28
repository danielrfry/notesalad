import MIDIDriverBase from './MIDIDriverBase';

export default class OPMMIDIDriver extends MIDIDriverBase {
    constructor(lib, hChip) {
        const hMIDI = lib._ntsld_opmmidi_new(0, hChip);
        super(lib, hMIDI);
    }
}
