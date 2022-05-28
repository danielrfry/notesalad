#include "midi.h"
#include "IMIDIManager.h"

void ntsld_midi_set_recv_cbk(MIDIRef midiRef, void* context, MIDIReceiveCallback callback)
{
    if (callback == nullptr) {
        ((IMIDIManager*)midiRef)->setReceiveCallback(nullptr);
    } else {
        ((IMIDIManager*)midiRef)->setReceiveCallback([context, callback](void* msgData, uint32_t msgLength) {
            callback(context, (uint8_t*)msgData, msgLength);
        });
    }
}

void ntsld_midi_send(MIDIRef midiRef, uint8_t* msgData, uint32_t msgLength)
{
    ((IMIDIManager*)midiRef)->sendMIDI(msgData, msgLength);
}

void ntsld_midi_set_time(MIDIRef midiRef, uint32_t timeMS)
{
    ((IMIDIManager*)midiRef)->setTime(timeMS);
}

void ntsld_midi_update(MIDIRef midiRef)
{
    ((IMIDIManager*)midiRef)->update();
}

void ntsld_midi_reset(MIDIRef midiRef)
{
    ((IMIDIManager*)midiRef)->reset();
}

void ntsld_midi_delete(MIDIRef midiRef)
{
    delete ((IMIDIManager*)midiRef);
}
