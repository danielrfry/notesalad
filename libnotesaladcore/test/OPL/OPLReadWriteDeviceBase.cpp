#include <gtest/gtest.h>

#include <notesaladcore.h>

using namespace testing;

class OPLReadWriteDeviceBaseTest : public ::testing::Test {
protected:
    OPLRegisterSet regs;
};

TEST_F(OPLReadWriteDeviceBaseTest, DeterminesVoiceEnabledStatus)
{
    this->regs.write(0x105, 1);
    for (uint8_t ch4Op = 0; ch4Op < 6; ch4Op++) {
        this->regs.write(0x104, 1 << ch4Op);

        uint8_t v4Op = ch4Op + 18;
        uint8_t ch2Op[2];
        OPL::get2OpChannelsFor4OpChannel(ch4Op, &ch2Op[0]);

        for (uint8_t v = 0; v < OPL_NUM_VOICES; v++) {
            bool voiceEnabled = this->regs.isVoiceEnabled(v);
            if (v == ch2Op[0] || v == ch2Op[1]) {
                EXPECT_EQ(voiceEnabled, false) << "2-op voice " << int(v) << " enabled, should be disabled for active 4-op channel " << int(ch4Op);
            } else if (v == v4Op) {
                EXPECT_EQ(voiceEnabled, true) << "4-op voice " << int(v) << " disabled, should be enabled for active 4-op channel " << int(ch4Op);
            } else if (v < 18) {
                EXPECT_EQ(voiceEnabled, true) << "2-op voice " << int(v) << " disabled, should be enabled for active 4-op channel " << int(ch4Op);
            } else {
                EXPECT_EQ(voiceEnabled, false) << "4-op voice " << int(v) << " enabled, should be disabled for active 4-op channel " << int(ch4Op);
            }
        }
    }
}