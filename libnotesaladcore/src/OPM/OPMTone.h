#pragma once

#include <cstdint>

#include "../Utils.h"

#define OPM_TONE_PARAM_RL 0
#define OPM_TONE_PARAM_FB 1
#define OPM_TONE_PARAM_CONECT 2
#define OPM_TONE_PARAM_PMS 3
#define OPM_TONE_PARAM_AMS 4
#define OPM_TONE_PARAM_OPS_ENABLED 5

#define OPM_TONE_PARAM_OP_DT1 0
#define OPM_TONE_PARAM_OP_MUL 1
#define OPM_TONE_PARAM_OP_TL 2
#define OPM_TONE_PARAM_OP_KS 3
#define OPM_TONE_PARAM_OP_AR 4
#define OPM_TONE_PARAM_OP_AMSEN 5
#define OPM_TONE_PARAM_OP_D1R 6
#define OPM_TONE_PARAM_OP_DT2 7
#define OPM_TONE_PARAM_OP_D2R 8
#define OPM_TONE_PARAM_OP_D1L 9
#define OPM_TONE_PARAM_OP_RR 10

typedef struct __attribute__((__packed__)) OPMOperatorData {
    uint8_t dt1_mul = 0x01;
    uint8_t tl = 0x7f;
    uint8_t ks_ar = 0x1f;
    uint8_t amsen_d1r = 0x00;
    uint8_t dt2_d2r = 0x00;
    uint8_t d1l_rr = 0x0f;
} OPMOperatorData;

typedef struct __attribute__((__packed__)) OPMToneData {
    OPMToneData();
    OPMOperatorData op[4];
    uint8_t rl_fb_conect;
    uint8_t pms_ams;
    uint8_t ops_enabled;
} OPMToneData;

class OPMTone {
public:
    OPMToneData toneData;

    uint16_t getParam(uint16_t paramID);
    void setParam(uint16_t paramID, uint16_t value);

    inline uint8_t getDT1(uint8_t op)
    {
        return getBits<4, 3>(toneData.op[op].dt1_mul);
    }
    inline void setDT1(uint8_t op, uint8_t value)
    {
        setBits<4, 3>(toneData.op[op].dt1_mul, value);
    }
    inline uint8_t getMUL(uint8_t op)
    {
        return getBits<0, 4>(toneData.op[op].dt1_mul);
    }
    inline void setMUL(uint8_t op, uint8_t value)
    {
        setBits<0, 4>(toneData.op[op].dt1_mul, value);
    }
    inline uint8_t getTL(uint8_t op)
    {
        return getBits<0, 7>(toneData.op[op].tl);
    }
    inline void setTL(uint8_t op, uint8_t value)
    {
        setBits<0, 7>(toneData.op[op].tl, value);
    }
    inline uint8_t getKS(uint8_t op)
    {
        return getBits<6, 2>(toneData.op[op].ks_ar);
    }
    inline void setKS(uint8_t op, uint8_t value)
    {
        return setBits<6, 2>(toneData.op[op].ks_ar, value);
    }
    inline uint8_t getAR(uint8_t op)
    {
        return getBits<0, 5>(toneData.op[op].ks_ar);
    }
    inline void setAR(uint8_t op, uint8_t value)
    {
        return setBits<0, 5>(toneData.op[op].ks_ar, value);
    }
    inline bool getAMSEN(uint8_t op)
    {
        return getBit<7>(toneData.op[op].amsen_d1r);
    }
    inline void setAMSEN(uint8_t op, bool value)
    {
        setBit<7>(toneData.op[op].amsen_d1r, value);
    }
    inline uint8_t getD1R(uint8_t op)
    {
        return getBits<0, 5>(toneData.op[op].amsen_d1r);
    }
    inline void setD1R(uint8_t op, uint8_t value)
    {
        setBits<0, 5>(toneData.op[op].amsen_d1r, value);
    }
    inline uint8_t getDT2(uint8_t op)
    {
        return getBits<6, 2>(toneData.op[op].dt2_d2r);
    }
    inline void setDT2(uint8_t op, uint8_t value)
    {
        setBits<6, 2>(toneData.op[op].dt2_d2r, value);
    }
    inline uint8_t getD2R(uint8_t op)
    {
        return getBits<0, 5>(toneData.op[op].dt2_d2r);
    }
    inline void setD2R(uint8_t op, uint8_t value)
    {
        setBits<0, 5>(toneData.op[op].dt2_d2r, value);
    }
    inline uint8_t getD1L(uint8_t op)
    {
        return getBits<4, 4>(toneData.op[op].d1l_rr);
    }
    inline void setD1L(uint8_t op, uint8_t value)
    {
        setBits<4, 4>(toneData.op[op].d1l_rr, value);
    }
    inline uint8_t getRR(uint8_t op)
    {
        return getBits<0, 4>(toneData.op[op].d1l_rr);
    }
    inline void setRR(uint8_t op, uint8_t value)
    {
        setBits<0, 4>(toneData.op[op].d1l_rr, value);
    }

    inline uint8_t getRL()
    {
        return getBits<6, 2>(toneData.rl_fb_conect);
    }
    inline void setRL(uint8_t value)
    {
        setBits<6, 2>(toneData.rl_fb_conect, value);
    }
    inline uint8_t getFB()
    {
        return getBits<3, 3>(toneData.rl_fb_conect);
    }
    inline void setFB(uint8_t value)
    {
        setBits<3, 3>(toneData.rl_fb_conect, value);
    }
    inline uint8_t getConect()
    {
        return getBits<0, 3>(toneData.rl_fb_conect);
    }
    inline void setConect(uint8_t value)
    {
        setBits<0, 3>(toneData.rl_fb_conect, value);
    }
    inline uint8_t getPMS()
    {
        return getBits<4, 3>(toneData.pms_ams);
    }
    inline void setPMS(uint8_t value)
    {
        setBits<4, 3>(toneData.pms_ams, value);
    }
    inline uint8_t getAMS()
    {
        return getBits<0, 2>(toneData.pms_ams);
    }
    inline void setAMS(uint8_t value)
    {
        setBits<0, 2>(toneData.pms_ams, value);
    }
    inline uint8_t getOpsEnabled()
    {
        return toneData.ops_enabled & 0x0f;
    }
    inline void setOpsEnabled(uint8_t value)
    {
        toneData.ops_enabled = value & 0x0f;
    }

    void applyVolume(uint8_t vol);
    void applyPan(uint8_t pan);

private:
    void applyVolume(uint8_t opIndex, uint8_t vol);
};
