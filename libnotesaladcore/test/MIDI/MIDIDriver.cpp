#include <cstdio>
#include <gtest/gtest.h>
#include <notesaladcore.h>
#include <vector>

#include "IToneGenerator_test.h"
#include "TestTone.h"

#define TEST_DEVICE_ID 0x18

using namespace testing;

class MIDIDriverTest : public Test {
protected:
    BasicVoiceAllocator<TestTone, 2> voiceAllocator;
    MockToneGenerator toneGenerator;
    RAMPatchManager<TestTone, 1> patchManager;
    TimeSource timeSource;
    MIDIDriver<TestTone, 2> midiDriver;
    MockFunction<void(std::vector<uint8_t> data)> onMIDISend;
    MIDIDriverTest() : voiceAllocator(), toneGenerator(), patchManager(nullptr), timeSource(), midiDriver(&voiceAllocator, &toneGenerator, &patchManager, &timeSource, TEST_DEVICE_ID, &TEST_PARAMS_INFO)
    {
        midiDriver.onSendMIDI = [this](void* data, uint32_t length) {
            this->onMIDISend.Call(std::vector<uint8_t>((uint8_t*)data, (uint8_t*)data + length));
        };
    }
    Patch<TestTone> getModifiedPatch();
    std::vector<uint8_t> getDefaultPatchSysExData();
    std::vector<uint8_t> getModifiedPatchSysExData();
};

static const std::vector<uint8_t> readDefaultPatchReplyMsg = {
    0xf0, // Start SysEx message
    0x00, 0x22, 0x53, // Manufacturer ID
    0x08, // Read patch reply command
    0x00, // Result: OK
    TEST_DEVICE_ID,
    0xf7 // End SysEx message
};

static const std::vector<uint8_t> readModifiedPatchReplyMsg = {
    0xf0, // Start SysEx message
    0x00, 0x22, 0x53, // Manufacturer ID
    0x08, // Read patch reply command
    0x00, // Result: OK
    TEST_DEVICE_ID,
    0x04, 0x00, 0x01, 0x1b, 0x02, 0x4d, 0x03, 0x03,
    0xf7 // End SysEx message
};

static const std::vector<uint8_t> readNonExistentPatchReplyMsg = {
    0xf0, // Start SysEx message
    0x00,
    0x22,
    0x53, // Manufacturer ID
    0x08, // Read patch reply command
    0x01, // Result: no patch
    0xf7 // End SysEx message
};

Patch<TestTone>
MIDIDriverTest::getModifiedPatch()
{
    Patch<TestTone> testPatch;
    testPatch.params.tone.paramA = 1;
    testPatch.params.tone.paramB = 2;
    testPatch.params.tone.paramC = 3;
    return testPatch;
}

TEST_F(MIDIDriverTest, HandlesReadPatchSysExMessageWithDefaultMelodicPatch)
{
    Patch<TestTone> testPatch;
    patchManager.setPatch(2, 3, 255, testPatch);
    auto readPatchMsg = SysEx::ReadPatchMsg(SysEx::PatchAddress::forMelodicPatch(2, 3));

    EXPECT_CALL(onMIDISend, Call(readDefaultPatchReplyMsg));
    midiDriver.sysEx((uint8_t*)&readPatchMsg, sizeof(readPatchMsg));
}

TEST_F(MIDIDriverTest, HandlesReadPatchSysExMessageWithMelodicPatch)
{
    auto testPatch = getModifiedPatch();
    patchManager.setPatch(2, 3, 255, testPatch);
    auto readPatchMsg = SysEx::ReadPatchMsg(SysEx::PatchAddress::forMelodicPatch(2, 3));

    EXPECT_CALL(onMIDISend, Call(readModifiedPatchReplyMsg));
    midiDriver.sysEx((uint8_t*)&readPatchMsg, sizeof(readPatchMsg));
}

TEST_F(MIDIDriverTest, HandlesReadPatchSysExMessageWithNoMelodicPatch)
{
    auto readPatchMsg = SysEx::ReadPatchMsg(SysEx::PatchAddress::forMelodicPatch(2, 4));
    EXPECT_CALL(onMIDISend, Call(readNonExistentPatchReplyMsg));
    midiDriver.sysEx((uint8_t*)&readPatchMsg, sizeof(readPatchMsg));
}

TEST_F(MIDIDriverTest, HandlesReadPatchSysExMessageWithDefaultDrumPatch)
{
    Patch<TestTone> testPatch;
    patchManager.setPatch(2, 3, 42, testPatch);
    auto readPatchMsg = SysEx::ReadPatchMsg(SysEx::PatchAddress::forDrumPatch(2, 3, 42));

    EXPECT_CALL(onMIDISend, Call(readDefaultPatchReplyMsg));
    midiDriver.sysEx((uint8_t*)&readPatchMsg, sizeof(readPatchMsg));
}

TEST_F(MIDIDriverTest, HandlesReadPatchSysExMessageWithDrumPatch)
{
    auto testPatch = getModifiedPatch();
    patchManager.setPatch(2, 3, 42, testPatch);
    auto readPatchMsg = SysEx::ReadPatchMsg(SysEx::PatchAddress::forDrumPatch(2, 3, 42));

    EXPECT_CALL(onMIDISend, Call(readModifiedPatchReplyMsg));
    midiDriver.sysEx((uint8_t*)&readPatchMsg, sizeof(readPatchMsg));
}

TEST_F(MIDIDriverTest, HandlesReadPatchSysExMessageWithNoDrumPatch)
{
    auto readPatchMsg = SysEx::ReadPatchMsg(SysEx::PatchAddress::forDrumPatch(0, 0, 1));
    EXPECT_CALL(onMIDISend, Call(readNonExistentPatchReplyMsg));
    midiDriver.sysEx((uint8_t*)&readPatchMsg, sizeof(readPatchMsg));
}

TEST_F(MIDIDriverTest, HandlesReadPatchSysExMessageWithChannelPatch)
{
    auto testPatch = getModifiedPatch();
    patchManager.setPatch(0, 1, 255, testPatch);
    midiDriver.programChange(3, 1);
    auto readPatchMsg = SysEx::ReadPatchMsg(SysEx::PatchAddress::forChannel(3));

    EXPECT_CALL(onMIDISend, Call(readModifiedPatchReplyMsg));
    midiDriver.sysEx((uint8_t*)&readPatchMsg, sizeof(readPatchMsg));
}

TEST_F(MIDIDriverTest, HandlesWritePatchSysExMessageWithDefaultMelodicPatch)
{
    std::vector<uint8_t> msgData = {
        0xf0, // Start SysEx message
        0x00, 0x22, 0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH, // Write patch command
        0x01, 0x02, 0x03, 0x00, // Write to melodic patch in bank 2, program 3
        TEST_DEVICE_ID,
        0xf7 // End SysEx message
    };
    std::vector<uint8_t> expectedReply = {
        0xf0, // Start SysEx message
        0x00,
        0x22,
        0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH_REPLY,
        SYSEX_WRITEPATCH_RESULTCODE_OK,
        0xf7 // End SysEx message
    };

    EXPECT_CALL(onMIDISend, Call(expectedReply));
    midiDriver.sysEx(&msgData[0], msgData.size());

    Patch<TestTone> patch;
    auto result = patchManager.getPatch(2, 3, 255, patch);

    EXPECT_TRUE(result);
    EXPECT_EQ(patch.params.tone.paramA, PARAM_DEFAULT_TEST_A);
    EXPECT_EQ(patch.params.tone.paramB, PARAM_DEFAULT_TEST_B);
    EXPECT_EQ(patch.params.tone.paramC, PARAM_DEFAULT_TEST_C);
}

TEST_F(MIDIDriverTest, HandlesWritePatchSysExMessageWithMelodicPatch)
{
    std::vector<uint8_t> msgData = {
        0xf0, // Start SysEx message
        0x00, 0x22, 0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH, // Write patch command
        0x01, 0x02, 0x03, 0x00, // Write to melodic patch in bank 2, program 3
        TEST_DEVICE_ID,
        0x04, 0x00, 0x01, 0x1b, 0x02, 0x4d, 0x03, 0x03, // Patch data
        0xf7 // End SysEx message
    };
    std::vector<uint8_t> expectedReply = {
        0xf0, // Start SysEx message
        0x00,
        0x22,
        0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH_REPLY,
        SYSEX_WRITEPATCH_RESULTCODE_OK,
        0xf7 // End SysEx message
    };

    EXPECT_CALL(onMIDISend, Call(expectedReply));
    midiDriver.sysEx(&msgData[0], msgData.size());

    Patch<TestTone> patch;
    auto expectedPatch = getModifiedPatch();
    auto result = patchManager.getPatch(2, 3, 255, patch);

    EXPECT_TRUE(result);
    EXPECT_EQ(patch.params.tone.paramA, expectedPatch.params.tone.paramA);
    EXPECT_EQ(patch.params.tone.paramB, expectedPatch.params.tone.paramB);
    EXPECT_EQ(patch.params.tone.paramC, expectedPatch.params.tone.paramC);
}

TEST_F(MIDIDriverTest, HandlesWritePatchSysExMessageWithDrumPatch)
{
    std::vector<uint8_t> msgData = {
        0xf0, // Start SysEx message
        0x00, 0x22, 0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH, // Write patch command
        0x02, 0x02, 0x03, 0x04, // Write to drum patch in bank 2, program 3, note 4
        TEST_DEVICE_ID,
        0x04, 0x00, 0x01, 0x1b, 0x02, 0x4d, 0x03, 0x03, // Patch data
        0xf7 // End SysEx message
    };
    std::vector<uint8_t> expectedReply = {
        0xf0, // Start SysEx message
        0x00,
        0x22,
        0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH_REPLY,
        SYSEX_WRITEPATCH_RESULTCODE_OK,
        0xf7 // End SysEx message
    };

    EXPECT_CALL(onMIDISend, Call(expectedReply));
    midiDriver.sysEx(&msgData[0], msgData.size());

    Patch<TestTone> patch;
    auto expectedPatch = getModifiedPatch();
    auto result = patchManager.getPatch(2, 3, 4, patch);

    EXPECT_TRUE(result);
    EXPECT_EQ(patch.params.tone.paramA, expectedPatch.params.tone.paramA);
    EXPECT_EQ(patch.params.tone.paramB, expectedPatch.params.tone.paramB);
    EXPECT_EQ(patch.params.tone.paramC, expectedPatch.params.tone.paramC);
}

TEST_F(MIDIDriverTest, HandlesWritePatchSysExMessageWithChannelPatch)
{
    std::vector<uint8_t> msgData = {
        0xf0, // Start SysEx message
        0x00, 0x22, 0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH, // Write patch command
        0x00, 0x03, 0x00, 0x00, // Write to channel 3
        TEST_DEVICE_ID,
        0x04, 0x00, 0x01, 0x1b, 0x02, 0x4d, 0x03, 0x03, // Patch data
        0xf7 // End SysEx message
    };
    std::vector<uint8_t> expectedReply = {
        0xf0, // Start SysEx message
        0x00,
        0x22,
        0x53, // Manufacturer ID
        SYSEX_CMD_WRITEPATCH_REPLY,
        SYSEX_WRITEPATCH_RESULTCODE_OK,
        0xf7 // End SysEx message
    };

    EXPECT_CALL(onMIDISend, Call(expectedReply));
    midiDriver.sysEx(&msgData[0], msgData.size());

    auto expectedPatch = getModifiedPatch();
    auto patch = *this->midiDriver.getChannelPatch(3);

    EXPECT_EQ(patch.params.tone.paramA, expectedPatch.params.tone.paramA);
    EXPECT_EQ(patch.params.tone.paramB, expectedPatch.params.tone.paramB);
    EXPECT_EQ(patch.params.tone.paramC, expectedPatch.params.tone.paramC);
}

TEST_F(MIDIDriverTest, HandlesParamSetNRPN)
{
    midiDriver.controlChange(0, 99, (PARAM_ID_TEST_C >> 7) & 0x7f);
    midiDriver.controlChange(0, 98, PARAM_ID_TEST_C & 0x7f);
    midiDriver.controlChange(0, 6, 0x12);
    midiDriver.controlChange(0, 38, 0x34);

    auto patch = *this->midiDriver.getChannelPatch(0);

    EXPECT_EQ(patch.params.tone.paramC, (0x12 << 7) | 0x34);
}

TEST_F(MIDIDriverTest, HandlesPitchWheelRangeRPN)
{
    midiDriver.controlChange(0, 101, 0);
    midiDriver.controlChange(0, 100, 0);
    midiDriver.controlChange(0, 6, 3);
    midiDriver.controlChange(0, 38, 62);

    midiDriver.pitchWheel(0, 0x3fff);

    EXPECT_CALL(toneGenerator, noteOn(_, 45.62f));
    midiDriver.noteOn(0, 42, 127);
    midiDriver.noteOff(0, 42, 127);
}
