#include "OPMUtils.h"

uint8_t OPM::getKCForMIDINote(uint8_t note)
{
    uint8_t oct = ((note - 1) / 12) - 1;
    uint8_t kc = (note - 1) % 12;

    switch (kc) {
    case 3:
        kc = 4;
        break;
    case 4:
        kc = 5;
        break;
    case 5:
        kc = 6;
        break;
    case 6:
        kc = 8;
        break;
    case 7:
        kc = 9;
        break;
    case 8:
        kc = 10;
        break;
    case 9:
        kc = 12;
        break;
    case 10:
        kc = 13;
        break;
    case 11:
        kc = 14;
        break;
    }

    kc |= (oct << 4);

    return kc;
}
