import libnotesalad from '../NoteSaladLibrary';
import OPMEmulator from '../Emulators/OPMEmulator';
import OPMMIDIDriver from '../MIDI/OPMMIDIDriver';
import OPXProcessorBase from './OPXProcessorBase';

export default class OPMProcessor extends OPXProcessorBase {
    constructor() {
        const emulator = new OPMEmulator(libnotesalad);
        const midiDriver = new OPMMIDIDriver(libnotesalad, emulator._hChip);
        super(emulator, midiDriver);
    }
}
