#include <array>
#include <gtest/gtest.h>
#include <notesaladcore.h>

#include "TestTone.h"

TEST(SerializationTest, SerializesDefaultPatch)
{
    Patch<TestTone> defaultPatch;
    ArrayWriter<64> writer;
    auto result = PatchSerialization::serialize(TEST_PARAMS_INFO, defaultPatch, &writer);

    EXPECT_EQ(result, true);
    EXPECT_EQ(writer.getPosition(), 0);
}

TEST(SerializationTest, SerializesParamID0)
{
    Patch<TestTone> patch;
    patch.params.tone.paramA = 338;

    std::array<uint8_t, 3> expectedData = {
        0x00, 0xd2, 0x02
    };
    std::array<uint8_t, 3> buffer;
    BufferWriter writer(&buffer[0], buffer.size());

    auto result = PatchSerialization::serialize(TEST_PARAMS_INFO, patch, &writer);

    EXPECT_EQ(result, true);
    EXPECT_EQ(buffer, expectedData);
}

TEST(SerializationTest, SerializesMultipleChipParams)
{
    Patch<TestTone> patch;
    auto& tone = patch.params.tone;
    tone.paramB = 574;
    tone.paramC = 12345;

    std::array<uint8_t, 7> expectedData = {
        0x1b, 0xbe, 0x04, 0xcd, 0x03, 0xb9, 0x60
    };
    std::array<uint8_t, 7> buffer;
    BufferWriter writer(&buffer[0], buffer.size());

    auto result = PatchSerialization::serialize(TEST_PARAMS_INFO, patch, &writer);

    EXPECT_EQ(result, true);
    EXPECT_EQ(buffer, expectedData);
}

TEST(SerializationTest, SerializesMultipleGenericParams)
{
    Patch<TestTone> patch;
    patch.params.pitchOffset = 8765;
    patch.paramMaps[2].src = 49;
    patch.lfoParams[0].wave = LFO_WAVE_RANDOM;

    std::array<uint8_t, 10> expectedData = {
        0x82, 0x40, 0xbd, 0x44, 0x9e, 0x20, 0x31, 0xe0, 0x01, 0x04
    };
    std::array<uint8_t, 10> buffer;
    BufferWriter writer(&buffer[0], buffer.size());

    auto result = PatchSerialization::serialize(TEST_PARAMS_INFO, patch, &writer);

    EXPECT_EQ(result, true);
    EXPECT_EQ(buffer, expectedData);
}

TEST(SerializationTest, SerializesPatchHandlingBufferOverrun)
{
    Patch<TestTone> patch;
    patch.params.pitchOffset = 8765;

    std::array<uint8_t, 3> buffer;
    BufferWriter writer(&buffer[0], buffer.size());

    auto result = PatchSerialization::serialize(TEST_PARAMS_INFO, patch, &writer);

    EXPECT_EQ(result, false);
}

bool comparePatches(Patch<TestTone>& a, Patch<TestTone>& b)
{
    for (int i = 0; i < TEST_PARAMS_INFO.numParams; i++) {
        auto param = TEST_PARAMS_INFO.paramInfo[i];
        if (a.getParam(param.id) != b.getParam(param.id))
            return false;
    }
    for (int i = 0; i < UNIVERSAL_PARAMS_INFO.numParams; i++) {
        auto param = UNIVERSAL_PARAMS_INFO.paramInfo[i];
        if (a.getParam(param.id) != b.getParam(param.id))
            return false;
    }
    return true;
}

TEST(DeserializationTest, DeserializesDefaultPatch)
{
    Patch<TestTone> expectedPatch;
    uint8_t backingStore = 0;
    BufferReader reader(&backingStore, 0);
    Patch<TestTone> deserializedPatch;

    auto result = PatchSerialization::deserialize(deserializedPatch, &reader);

    EXPECT_EQ(result, true);
    EXPECT_EQ(comparePatches(deserializedPatch, expectedPatch), true);
}

TEST(DeserializationTest, DeserializesParamID0)
{
    Patch<TestTone> expectedPatch;
    expectedPatch.params.tone.paramA = 338;
    std::array<uint8_t, 3> buffer = {
        0x00, 0xd2, 0x02
    };
    BufferReader reader(&buffer[0], buffer.size());
    Patch<TestTone> deserializedPatch;

    auto result = PatchSerialization::deserialize(deserializedPatch, &reader);

    EXPECT_EQ(result, true);
    EXPECT_EQ(comparePatches(deserializedPatch, expectedPatch), true);
}

TEST(DeserializationTest, DeserializesMultipleChipParams)
{
    Patch<TestTone> expectedPatch;
    auto& tone = expectedPatch.params.tone;
    tone.paramB = 574;
    tone.paramC = 12345;
    std::array<uint8_t, 7> buffer = {
        0x1b, 0xbe, 0x04, 0xcd, 0x03, 0xb9, 0x60
    };
    BufferReader reader(&buffer[0], buffer.size());
    Patch<TestTone> deserializedPatch;

    auto result = PatchSerialization::deserialize(deserializedPatch, &reader);

    EXPECT_EQ(result, true);
    EXPECT_EQ(comparePatches(deserializedPatch, expectedPatch), true);
}

TEST(DeserializationTest, DeserializesMultipleGenericParams)
{
    Patch<TestTone> expectedPatch;
    expectedPatch.params.pitchOffset = 8765;
    expectedPatch.paramMaps[2].src = 49;
    expectedPatch.lfoParams[0].wave = LFO_WAVE_RANDOM;
    std::array<uint8_t, 10> buffer = {
        0x82, 0x40, 0xbd, 0x44, 0x9e, 0x20, 0x31, 0xe0, 0x01, 0x04
    };
    BufferReader reader(&buffer[0], buffer.size());
    Patch<TestTone> deserializedPatch;

    auto result = PatchSerialization::deserialize(deserializedPatch, &reader);

    EXPECT_EQ(result, true);
    EXPECT_EQ(comparePatches(deserializedPatch, expectedPatch), true);
}

TEST(DeserializationTest, DeserializesPatchHandlingBufferUnderrun)
{
    std::array<uint8_t, 3> buffer = {
        0x82, 0x40, 0xbd
    };
    BufferReader reader(&buffer[0], buffer.size());
    Patch<TestTone> deserializedPatch;

    auto result = PatchSerialization::deserialize(deserializedPatch, &reader);

    EXPECT_EQ(result, false);
}
