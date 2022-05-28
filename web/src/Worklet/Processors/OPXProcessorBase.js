export default class OPXProcessorBase extends AudioWorkletProcessor {
    constructor(emulator, midiDriver) {
        super();

        this._midiDriver = midiDriver;
        this._emulator = emulator;
        this._sampleCount = 0;
        this._writeBuffer = [];
        this._writeBufferIndex = 0;

        this.port.onmessage = e => this._handleMessage(e);
        this._midiDriver.onReceiveMIDI = (...args) =>
            this._receiveMIDI(...args);
        this._updateMIDITime();
    }

    _handleMessage(e) {
        if (e.data.type === 'reset') {
            this._handleResetMessage();
        } else if (e.data.type === 'write') {
            this._handleWriteMessage(e.data);
        } else if (e.data.type === 'midiWrite') {
            this._handleMIDIWriteMessage(e.data);
        } else if (e.data.type === 'close') {
            this._handleCloseMessage();
        }
    }

    _handleResetMessage() {
        if (this._midiDriver) {
            this._midiDriver.reset();
        } else {
            this._emulator.reset();
        }
    }

    _handleWriteMessage(data) {
        this._sampleCount = 0;
        this._writeBuffer = data.buffer;
        this._writeBufferIndex = 0;
    }

    _handleMIDIWriteMessage(data) {
        const messages = data.messages;
        for (let msg of messages) {
            const { data } = msg;
            this._midiDriver.send(data);
        }
    }

    _handleCloseMessage() {
        if (this._midiDriver) {
            this._midiDriver.close();
            this._midiDriver = null;
        }
        if (this._emulator) {
            this._emulator.close();
            this._emulator = null;
        }
    }

    _peekEvent() {
        const buffer = this._writeBuffer;
        const index = this._writeBufferIndex;

        return index < buffer.length ? buffer[index] : null;
    }

    _getEvent() {
        const event = this._peekEvent();
        if (event) {
            this._writeBufferIndex++;
        }
        return event;
    }

    _updateMIDITime() {
        const timeMS = Math.floor((this._sampleCount / sampleRate) * 1000);
        this._midiDriver.setTime(timeMS);
    }

    _receiveMIDI(msgData) {
        this.port.postMessage({ type: 'midiReceive', msgData });
    }

    process(inputs, outputs, parameters) {
        if (!this._emulator) {
            return false;
        }

        const bufferSize = outputs[0][0].length;
        let offset = 0;

        if (this._midiDriver) {
            this._updateMIDITime();
            this._midiDriver.update();
        }

        while (offset < bufferSize) {
            let nextChunkSize = 0;

            do {
                const event = this._peekEvent();
                if (event) {
                    const eventSample = Math.round(
                        (event.time / 1000) * sampleRate
                    );
                    if (eventSample <= this._sampleCount) {
                        this._getEvent();
                        this._emulator.write(event.reg, event.value);
                    } else {
                        nextChunkSize = Math.min(
                            bufferSize - offset,
                            eventSample - this._sampleCount
                        );
                        break;
                    }
                } else {
                    nextChunkSize = bufferSize - offset;
                    break;
                }
            } while (true);

            this._emulator.getSamples(offset, nextChunkSize, outputs[0]);
            offset += nextChunkSize;
            this._sampleCount += nextChunkSize;
        }

        return true;
    }
}
