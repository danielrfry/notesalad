#include <gtest/gtest.h>

#include "IMIDIDriver_test.h"
#include <cstring>
#include <notesaladcore.h>

using namespace testing;

class MIDIParserTest : public Test {
protected:
    MockMIDIDriver driver;
    MIDIParser parser;

    MIDIParserTest() : driver(), parser(&driver)
    {
    }

    void SetUp() override
    {
    }
};

TEST_F(MIDIParserTest, ParsesNoteOnMessage)
{
    uint8_t buffer[] = {
        0x93, 0x40, 0x7f
    };
    EXPECT_CALL(driver, noteOn(3, 0x40, 0x7f));
    parser.putBuffer(&buffer[0], 3);
}

TEST_F(MIDIParserTest, ParsesNoteOffMessage)
{
    uint8_t buffer[] = {
        0x83, 0x40, 0x7f
    };
    EXPECT_CALL(driver, noteOff(3, 0x40, 0x7f));
    parser.putBuffer(&buffer[0], 3);
}

TEST_F(MIDIParserTest, ParsesProgramChangeMessage)
{
    uint8_t buffer[] = {
        0xc3, 0x42
    };
    EXPECT_CALL(driver, programChange(3, 0x42));
    parser.putBuffer(&buffer[0], 3);
}

TEST_F(MIDIParserTest, ParsesControlChangeMessage)
{
    uint8_t buffer[] = {
        0xb3, 0x42, 0x21
    };
    EXPECT_CALL(driver, controlChange(3, 0x42, 0x21));
    parser.putBuffer(&buffer[0], 3);
}

TEST_F(MIDIParserTest, ParsesSysExMessage)
{
    uint8_t buffer[] = {
        0xf0, 0x01, 0x02, 0x03, 0x04, 0x05, 0xf7
    };
    EXPECT_CALL(driver, sysEx(_, 7));
    parser.putBuffer(&buffer[0], 7);
}

TEST_F(MIDIParserTest, ParsesPitchWheelMessage)
{
    uint8_t buffer[] = {
        0xe3, 0x34, 0x24
    };
    EXPECT_CALL(driver, pitchWheel(3, 0x1234));
    parser.putBuffer(&buffer[0], 3);
}

TEST_F(MIDIParserTest, IgnoresSpuriousDataBeforeMessages)
{
    uint8_t buffer[] = {
        0x42, 0x38, 0x90, 0x42, 0x7f
    };
    EXPECT_CALL(driver, noteOn(0, 0x42, 0x7f)).Times(1);
    parser.putBuffer(&buffer[0], 5);
}

TEST_F(MIDIParserTest, IgnoresSpuriousDataBetweenMessages)
{
    InSequence seq;

    uint8_t buffer[] = {
        0x90, 0x42, 0x7f, 0x38, 0x42, 0x90, 0x43, 0x7f
    };
    EXPECT_CALL(driver, noteOn(0, 0x42, 0x7f)).Times(1);
    EXPECT_CALL(driver, noteOn(0, 0x43, 0x7f)).Times(1);
    parser.putBuffer(&buffer[0], 8);
}

TEST_F(MIDIParserTest, IgnoresSpuriousDataAfterMessages)
{
    uint8_t buffer[] = {
        0x90, 0x42, 0x7f, 0x38, 0x21
    };
    EXPECT_CALL(driver, noteOn(0, 0x42, 0x7f)).Times(1);
    parser.putBuffer(&buffer[0], 5);
}

TEST_F(MIDIParserTest, HandlesMessagesSplitOverMultipleBuffers)
{
    uint8_t buffer1[] = {
        0x90, 0x42
    };
    uint8_t buffer2[] = {
        0x7f
    };
    EXPECT_CALL(driver, noteOn(0, 0x42, 0x7f)).Times(1);
    parser.putBuffer(&buffer1[0], 2);
    parser.putBuffer(&buffer2[0], 1);
}

TEST_F(MIDIParserTest, HandlesSplitSysExMessages)
{
    uint8_t buffer1[] = {
        0xf0, 0x12, 0x34, 0x56
    };
    uint8_t buffer2[] = {
        0x78, 0x65, 0x43, 0x21, 0xf7
    };
    EXPECT_CALL(driver, sysEx(_, 9)).Times(1);
    parser.putBuffer(&buffer1[0], 4);
    parser.putBuffer(&buffer2[0], 5);
}

TEST_F(MIDIParserTest, IgnoresPartialMessages)
{
    InSequence seq;

    uint8_t buffer[] = {
        0x93,
        0x42,
        0x7f,
        0x92,
        0x42,
        0x94,
        0x43,
        0x7f,
    };
    EXPECT_CALL(driver, noteOn(3, 0x42, 0x7f)).Times(1);
    EXPECT_CALL(driver, noteOn(4, 0x43, 0x7f)).Times(1);
    parser.putBuffer(&buffer[0], 8);
}

TEST_F(MIDIParserTest, IgnoresUnrecognizedMessages)
{
    InSequence seq;
    uint8_t buffer[] = {
        0x90, 0x42, 0x7f,
        0xf4, 0x33, 0x22, 0x11,
        0x90, 0x43, 0x7f
    };
    EXPECT_CALL(driver, noteOn(0, 0x42, 0x7f)).Times(1);
    EXPECT_CALL(driver, noteOn(0, 0x43, 0x7f)).Times(1);
    parser.putBuffer(&buffer[0], 10);
}

TEST_F(MIDIParserTest, IgnoresUnterminatedSysExMessages)
{
    InSequence seq;
    uint8_t buffer[] = {
        0xf0, 0x12, 0x34, 0x56,
        0x90, 0x42, 0x7f
    };
    EXPECT_CALL(driver, sysEx(_, _)).Times(0);
    EXPECT_CALL(driver, noteOn(0, 0x42, 0x7f)).Times(1);
    parser.putBuffer(&buffer[0], 7);
}

TEST_F(MIDIParserTest, IgnoresMessagesThatOverflowBuffer)
{
    uint8_t buffer[MIDI_BUFFER_SIZE + 10];
    memset(&buffer[0], 0x77, MIDI_BUFFER_SIZE + 10);
    buffer[0] = 0x90;
    buffer[MIDI_BUFFER_SIZE + 7] = 0x90;
    buffer[MIDI_BUFFER_SIZE + 8] = 0x42;
    buffer[MIDI_BUFFER_SIZE + 9] = 0x7f;
    EXPECT_CALL(driver, noteOn(0, 0x42, 0x7f)).Times(1);
    parser.putBuffer(&buffer[0], MIDI_BUFFER_SIZE + 10);
}
