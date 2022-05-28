import libnotesalad from '../NoteSaladLibrary';
import OPLEmulator from '../Emulators/OPLEmulator';
import OPL3MIDIDriver from '../MIDI/OPL3MIDIDriver';
import OPXProcessorBase from './OPXProcessorBase';

export default class OPLProcessor extends OPXProcessorBase {
    constructor() {
        const emulator = new OPLEmulator(libnotesalad);
        const midiDriver = new OPL3MIDIDriver(libnotesalad, emulator._hChip);
        super(emulator, midiDriver);
    }
}
