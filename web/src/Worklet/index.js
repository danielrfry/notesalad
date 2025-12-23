import OPLProcessor from './Processors/OPLProcessor';
import OPMProcessor from './Processors/OPMProcessor';
import { initializeLibNoteSalad } from './NoteSaladLibrary';

await initializeLibNoteSalad();

registerProcessor('opl-processor', OPLProcessor);
registerProcessor('opm-processor', OPMProcessor);
