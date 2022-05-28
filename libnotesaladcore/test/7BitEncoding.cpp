#include <gtest/gtest.h>

#include <cstring>
#include <notesaladcore.h>

using namespace testing;

const int numTestValues = 6;

const uint16_t decodedValues[] = {
    0x0000,
    0x007f,
    0x0080,
    0x3fff,
    0x4000,
    0xffff
};

const uint8_t encodedValueStep = 3;

const uint8_t encodedValues[] = {
    0x00, 0x00, 0x00,
    0x7f, 0x00, 0x00,
    0x80, 0x01, 0x00,
    0xff, 0x7f, 0x00,
    0x80, 0x80, 0x01,
    0xff, 0xff, 0x03
};

const size_t encodedValueSizes[] = {
    1, 1, 2, 2, 3, 3
};

TEST(Encoding7BitTest, EncodesLEB128)
{
    const size_t bufferSize = 3;

    for (uint8_t i = 0; i < numTestValues; i++) {
        ArrayWriter<3> destBuffer;

        bool result = encodeLEB128(decodedValues[i], &destBuffer);
        EXPECT_EQ(result, true) << "Expected result to be true for test value " << decodedValues[i];
        EXPECT_EQ(destBuffer.getPosition(), encodedValueSizes[i]) << "Expected final position to be " << encodedValueSizes[i] << " for test value " << decodedValues[i];

        for (size_t j = 0; j < bufferSize; j++) {
            uint8_t expectedByte = encodedValues[(i * encodedValueStep) + j];
            EXPECT_EQ(destBuffer.buffer[j], expectedByte) << "Expected byte " << j << " to be " << int(expectedByte) << " for test value " << decodedValues[i];
        }
    }
}

TEST(Encoding7BitTest, EncodesLEB128HandlingBufferOverflow)
{
    ArrayWriter<2> destBuffer;
    bool result = encodeLEB128(0xffff, &destBuffer);

    EXPECT_EQ(result, false) << "Expected result to be false";
}

TEST(Encoding7BitTest, EncodesLEB128HandlingZeroLengthBuffer)
{
    uint8_t backingStore;
    BufferWriter destBuffer(&backingStore, 0);
    bool result = encodeLEB128(0xffff, &destBuffer);

    EXPECT_EQ(result, false) << "Expected result to be false";
}

TEST(Encoding7BitTest, DecodesLEB128)
{
    for (uint8_t i = 0; i < numTestValues; i++) {
        BufferReader srcBuffer(&encodedValues[i * encodedValueStep], encodedValueStep);
        uint16_t decodedValue = 0;
        size_t expectedPosition = encodedValueSizes[i];
        bool result = decodeLEB128(decodedValue, &srcBuffer);

        EXPECT_EQ(result, true) << "Expected result to be true";
        EXPECT_EQ(decodedValue, decodedValues[i]) << "Expected decoded value to be " << decodedValues[i];
        EXPECT_EQ(srcBuffer.getPosition(), expectedPosition) << "Expected position to be " << expectedPosition;
    }
}

TEST(Encoding7BitTest, DecodesLEB128HandlingBufferUnderrun)
{
    const uint8_t encodedValue[] = { 0xff, 0xff };
    BufferReader srcBuffer(&encodedValue[0], 2);
    uint16_t decodedValue = 0;
    bool result = decodeLEB128(decodedValue, &srcBuffer);

    EXPECT_EQ(result, false) << "Expected result to be false";
    EXPECT_EQ(srcBuffer.getPosition(), 2) << "Expected reader position to be 2";
}

TEST(Encoding7BitTest, DecodesLEB128HandlingZeroLengthBuffer)
{
    const uint8_t backingStore = 0;
    BufferReader srcBuffer(&backingStore, 0);
    uint16_t decodedValue = 0;
    bool result = decodeLEB128(decodedValue, &srcBuffer);

    EXPECT_EQ(result, false) << "Expected result to be false";
}

TEST(Encoding7BitTest, Calculates7BitEncodedSize)
{
    EXPECT_EQ(get7BitEncodedSize(0), 0);
    EXPECT_EQ(get7BitEncodedSize(1), 2);
    EXPECT_EQ(get7BitEncodedSize(8), 10);
    EXPECT_EQ(get7BitEncodedSize(512), 586);
}

TEST(Encoding7BitTest, Calculates7BitDecodedSize)
{
    EXPECT_EQ(get7BitDecodedSize(0), 0);
    EXPECT_EQ(get7BitDecodedSize(1), 0);
    EXPECT_EQ(get7BitDecodedSize(2), 1);
    EXPECT_EQ(get7BitDecodedSize(9), 7);
    EXPECT_EQ(get7BitDecodedSize(10), 8);
    EXPECT_EQ(get7BitDecodedSize(585), 511);
    EXPECT_EQ(get7BitDecodedSize(586), 512);
}

void hexDump(char* output, const uint8_t* input, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        sprintf(output, "%02x ", input[i]);
        output += 3;
    }
}

TEST(Encoding7BitTest, Encodes7Bit)
{
    const uint8_t input[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
    const uint8_t expectedOutput[] = { 0x07, 0x01, 0x23, 0x45, 0x67, 0x09, 0x2b, 0x4d, 0x40, 0x6f };
    char expectedOutputStr[31];
    uint8_t output[10];
    char outputStr[31];

    bool result = encode7Bit(&input[0], 8, &output[0], 10);

    hexDump(&expectedOutputStr[0], &expectedOutput[0], 10);
    hexDump(&outputStr[0], &output[0], 10);

    EXPECT_TRUE(result);
    EXPECT_STREQ(expectedOutputStr, outputStr);
}

TEST(Encoding7BitTest, Decodes7Bit)
{
    const uint8_t input[] = { 0x07, 0x01, 0x23, 0x45, 0x67, 0x09, 0x2b, 0x4d, 0x40, 0x6f };
    const uint8_t expectedOutput[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
    char expectedOutputStr[25];
    uint8_t output[8];
    char outputStr[25];

    bool result = decode7Bit(&input[0], 10, &output[0], 8);

    hexDump(&expectedOutputStr[0], &expectedOutput[0], 8);
    hexDump(&outputStr[0], &output[0], 8);

    EXPECT_TRUE(result);
    EXPECT_STREQ(expectedOutputStr, outputStr);
}
