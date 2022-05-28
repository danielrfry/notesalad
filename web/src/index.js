import PARAMS_OPL from '../../resources/params/opl.json';
import PARAMS_OPM from '../../resources/params/opm.json';
import PARAMS_SD1 from '../../resources/params/sd1.json';
import PARAMS_UNIVERSAL from '../../resources/params/universal.json';

export const initAudioContext = (context, libPath = 'libnotesalad.js') =>
    context.audioWorklet.addModule(libPath);

class NodeWrapperBase {
    constructor(node) {
        this.node = node;
        this.onReceiveMIDI = null;
        this.node.port.onmessage = (...args) => this._receiveMessage(...args);
    }

    writeEvents(events) {
        this.node.port.postMessage({ type: 'write', buffer: events });
    }

    writeMIDI(messages) {
        this.node.port.postMessage({ type: 'midiWrite', messages });
    }

    close() {
        this.node.port.postMessage({ type: 'close' });
    }

    _receiveMessage(e) {
        if (e.data && e.data.type === 'midiReceive') {
            if (this.onReceiveMIDI) {
                this.onReceiveMIDI(e.data.msgData);
            }
        }
    }
}

export class OPLNodeWrapper extends NodeWrapperBase {
    constructor(audioContext) {
        super(
            new AudioWorkletNode(audioContext, 'opl-processor', {
                numberOfInputs: 0,
                numberOfOutputs: 1,
                outputChannelCount: [2],
            })
        );
    }
}

export class OPMNodeWrapper extends NodeWrapperBase {
    constructor(audioContext) {
        super(
            new AudioWorkletNode(audioContext, 'opm-processor', {
                numberOfInputs: 0,
                numberOfOutputs: 1,
                outputChannelCount: [2],
            })
        );
    }
}

export { PARAMS_OPL, PARAMS_OPM, PARAMS_SD1, PARAMS_UNIVERSAL };
