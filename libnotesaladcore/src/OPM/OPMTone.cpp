#include "OPMTone.h"
#include "../Utils.h"

OPMToneData::OPMToneData()
{
    for (uint8_t op = 0; op < 4; op++) {
        this->op[op] = OPMOperatorData();
    }
    this->op[3].tl = 0x00;

    this->rl_fb_conect = 0xc0;
    this->pms_ams = 0x00;
    this->ops_enabled = 0x0f;
}

uint16_t OPMTone::getParam(uint16_t paramID)
{
    uint8_t opID = paramID / 16;
    uint8_t param = paramID % 16;

    if (opID == 0) {
        switch (param) {
        case OPM_TONE_PARAM_RL:
            return getRL();
        case OPM_TONE_PARAM_FB:
            return getFB();
        case OPM_TONE_PARAM_CONECT:
            return getConect();
        case OPM_TONE_PARAM_PMS:
            return getPMS();
        case OPM_TONE_PARAM_AMS:
            return getAMS();
        case OPM_TONE_PARAM_OPS_ENABLED:
            return getOpsEnabled();
        }
    } else if (opID >= 1 && opID <= 4) {
        uint8_t op = opID - 1;

        switch (param) {
        case OPM_TONE_PARAM_OP_DT1:
            return getDT1(op);
        case OPM_TONE_PARAM_OP_MUL:
            return getMUL(op);
        case OPM_TONE_PARAM_OP_TL:
            return getTL(op);
        case OPM_TONE_PARAM_OP_KS:
            return getKS(op);
        case OPM_TONE_PARAM_OP_AR:
            return getAR(op);
        case OPM_TONE_PARAM_OP_AMSEN:
            return getAMSEN(op);
        case OPM_TONE_PARAM_OP_D1R:
            return getD1R(op);
        case OPM_TONE_PARAM_OP_DT2:
            return getDT2(op);
        case OPM_TONE_PARAM_OP_D2R:
            return getD2R(op);
        case OPM_TONE_PARAM_OP_D1L:
            return getD1L(op);
        case OPM_TONE_PARAM_OP_RR:
            return getRR(op);
        }
    }

    return 0;
}

void OPMTone::setParam(uint16_t paramID, uint16_t value)
{
    uint8_t opID = paramID / 16;
    uint8_t param = paramID % 16;

    if (opID == 0) {
        switch (param) {
        case OPM_TONE_PARAM_RL:
            return setRL(clamp<uint8_t>((uint8_t)value, 0, 3));
        case OPM_TONE_PARAM_FB:
            return setFB(clamp<uint8_t>((uint8_t)value, 0, 7));
        case OPM_TONE_PARAM_CONECT:
            return setConect(clamp<uint8_t>((uint8_t)value, 0, 7));
        case OPM_TONE_PARAM_PMS:
            return setPMS(clamp<uint8_t>((uint8_t)value, 0, 7));
        case OPM_TONE_PARAM_AMS:
            return setAMS(clamp<uint8_t>((uint8_t)value, 0, 7));
        case OPM_TONE_PARAM_OPS_ENABLED:
            return setOpsEnabled(value);
        }
    } else if (opID >= 1 && opID <= 4) {
        uint8_t op = opID - 1;

        switch (param) {
        case OPM_TONE_PARAM_OP_DT1:
            return setDT1(op, clamp<uint8_t>((uint8_t)value, 0, 7));
        case OPM_TONE_PARAM_OP_MUL:
            return setMUL(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case OPM_TONE_PARAM_OP_TL:
            return setTL(op, clamp<uint8_t>((uint8_t)value, 0, 127));
        case OPM_TONE_PARAM_OP_KS:
            return setKS(op, clamp<uint8_t>((uint8_t)value, 0, 3));
        case OPM_TONE_PARAM_OP_AR:
            return setAR(op, clamp<uint8_t>((uint8_t)value, 0, 31));
        case OPM_TONE_PARAM_OP_AMSEN:
            return setAMSEN(op, clamp<uint8_t>((uint8_t)value, 0, 1));
        case OPM_TONE_PARAM_OP_D1R:
            return setD1R(op, clamp<uint8_t>((uint8_t)value, 0, 31));
        case OPM_TONE_PARAM_OP_DT2:
            return setDT2(op, clamp<uint8_t>((uint8_t)value, 0, 3));
        case OPM_TONE_PARAM_OP_D2R:
            return setD2R(op, clamp<uint8_t>((uint8_t)value, 0, 31));
        case OPM_TONE_PARAM_OP_D1L:
            return setD1L(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case OPM_TONE_PARAM_OP_RR:
            return setRR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        }
    }
}

void OPMTone::applyVolume(uint8_t opIndex, uint8_t vol)
{
    vol = lookupMIDIVolume(vol);
    uint8_t tl = this->getTL(opIndex);
    tl = 0x7f - (((0x7f - tl) * vol) / 127);
    this->setTL(opIndex, tl);
}

void OPMTone::applyVolume(uint8_t vol)
{
    switch (this->getConect()) {
    case 0:
    case 1:
    case 2:
    case 3:
        this->applyVolume(3, vol);
        break;
    case 4:
        this->applyVolume(2, vol);
        this->applyVolume(3, vol);
        break;
    case 5:
    case 6:
        this->applyVolume(1, vol);
        this->applyVolume(2, vol);
        this->applyVolume(3, vol);
        break;
    case 7:
        this->applyVolume(0, vol);
        this->applyVolume(1, vol);
        this->applyVolume(2, vol);
        this->applyVolume(3, vol);
        break;
    }
}

void OPMTone::applyPan(uint8_t pan)
{
    if (pan > 95) {
        setBits<6, 2>(this->toneData.rl_fb_conect, 2);
    } else if (pan < 33) {
        setBits<6, 2>(this->toneData.rl_fb_conect, 1);
    } else {
        setBits<6, 2>(this->toneData.rl_fb_conect, 3);
    }
}