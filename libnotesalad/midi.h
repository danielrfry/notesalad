#pragma once

#include <cstdint>

extern "C" {
    typedef void* MIDIRef;
    typedef void (*MIDIReceiveCallback)(void* context, uint8_t* msgData, uint32_t msgLength);

    void ntsld_midi_set_recv_cbk(MIDIRef midiRef, void* context, MIDIReceiveCallback callback);
    void ntsld_midi_send(MIDIRef midiRef, uint8_t* msgData, uint32_t msgLength);
    void ntsld_midi_set_time(MIDIRef midiRef, uint32_t timeMS);
    void ntsld_midi_update(MIDIRef midiRef);
    void ntsld_midi_reset(MIDIRef midiRef);
    void ntsld_midi_delete(MIDIRef midiRef);
}
