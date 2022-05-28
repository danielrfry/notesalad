#include <gtest/gtest.h>

#include <notesaladcore.h>

using namespace testing;

class SD1UtilsTest : public ::testing::Test {
};

TEST_F(SD1UtilsTest, ConvertsOPLTone)
{
    OPLTone oplTone;
    oplTone.set4Op(true);
    oplTone.setFB(3);
    oplTone.setCNT(1);

    for (uint8_t op = 0; op < 4; op++) {
        oplTone.setAM(op, op % 2);
        oplTone.setVIB(op, 1 - (op % 2));
        oplTone.setEGT(op, op % 3);
        oplTone.setKSR(op, 1 - (op % 3));
        oplTone.setMULT(op, 4 - op);
        oplTone.setKSL(op, op);
        oplTone.setTL(op, op * 3 + 1);
        oplTone.setAR(op, op * 2);
        oplTone.setDR(op, (3 - op) * 2);
        oplTone.setSL(op, op);
        oplTone.setRR(op, 3 - op);
        oplTone.setWS(op, op);
    }

    SD1Tone sd1Tone;
    SD1::convertOPLTone(oplTone, sd1Tone);

    EXPECT_EQ(sd1Tone.getALG(), 5) << "Incorrect algorithm";
    for (uint8_t op = 0; op < 4; op++) {
        EXPECT_EQ(sd1Tone.getEAM(op), oplTone.getAM(op)) << "EAM value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getEVB(op), oplTone.getVIB(op)) << "EVB value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getKSR(op), oplTone.getKSR(op)) << "KSR value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getMULTI(op), oplTone.getMULT(op)) << "MULTI value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getKSL(op), oplTone.getKSL(op)) << "KSL value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getTL(op), oplTone.getTL(op)) << "TL value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getAR(op), oplTone.getAR(op)) << "AR value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getDR(op), oplTone.getDR(op)) << "DR value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getSL(op), oplTone.getSL(op)) << "SL value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getRR(op), oplTone.getRR(op)) << "RR value differs for operator " << (int)op;
        EXPECT_EQ(sd1Tone.getWS(op), oplTone.getWS(op)) << "WS value differs for operator " << (int)op;

        uint8_t expectedSR = oplTone.getEGT(op) ? 0 : oplTone.getRR(op);
        EXPECT_EQ(sd1Tone.getSR(op), expectedSR) << "SR value is incorrect for operator " << (int)op;
    }
    EXPECT_EQ(sd1Tone.getFB(0), oplTone.getFB()) << "FB value differs";
}
