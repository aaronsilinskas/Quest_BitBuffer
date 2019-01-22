#include <Arduino.h>
#include <unity.h>

#include "Quest_BitReader.h"

#define BUFFER_SIZE 48
#define BUFFER_SIZE_IN_BITS BUFFER_SIZE * 8

uint8_t buffer[BUFFER_SIZE];
uint8_t readBuffer[BUFFER_SIZE];

void randomizeBuffer()
{
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = random(256);
    }
}

void test_new_instance_is_reset_to_full_buffer_size()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, br.bitsRemaining());
}

void test_reset_to_less_than_buffer_size()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    uint16_t smallerSize = BUFFER_SIZE_IN_BITS / 3;
    bool result = br.reset(smallerSize);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(smallerSize, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(smallerSize, br.bitsRemaining());
}

void test_reset_to_zero_bits_is_ok()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    bool result = br.reset(0);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(0, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
}

void test_reset_more_than_buffer_size_is_max_buffer_size()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    uint16_t largerSize = BUFFER_SIZE_IN_BITS * 3;
    bool result = br.reset(largerSize);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, br.bitsRemaining());
}

void test_reading_single_bits()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    buffer[0] = 0b01010100;
    br.reset(6); // buffer has 6 bits of a 0 1 pattern

    TEST_ASSERT_EQUAL(false, br.readBit());
    TEST_ASSERT_EQUAL(true, br.readBit());
    TEST_ASSERT_EQUAL(false, br.readBit());
    TEST_ASSERT_EQUAL(true, br.readBit());
    TEST_ASSERT_EQUAL(false, br.readBit());
    TEST_ASSERT_EQUAL(true, br.readBit());
}

void test_reading_multiple_bits()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    buffer[0] = 0b00110011;
    buffer[1] = 0b11001100;
    buffer[2] = 0b10101000;
    br.reset(22); // buffer has 22 bits of various patterns

    TEST_ASSERT_EQUAL(0b0011, br.readBits(4));
    TEST_ASSERT_EQUAL(0b00, br.readBits(2));
    TEST_ASSERT_EQUAL(0b11, br.readBits(2));
    TEST_ASSERT_EQUAL(0b11001100, br.readBits(8));
    TEST_ASSERT_EQUAL(0b101010, br.readBits(6));
}

void test_reading_to_buffer_byte_aligned()
{
    randomizeBuffer();

    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    uint16_t bitsToRead = BUFFER_SIZE_IN_BITS / 16;

    uint16_t bitsRead = br.readBuffer(readBuffer, bitsToRead);

    TEST_ASSERT_EQUAL(bitsToRead, bitsRead);
    TEST_ASSERT_EQUAL_INT8_ARRAY(buffer, readBuffer, bitsToRead / 8);
}

void test_reading_to_buffer_byte_unaligned()
{
    randomizeBuffer();

    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);

    // read some bits to unalign bit position
    br.readBits(3);

    // read to a buffer
    uint16_t bitsToRead = BUFFER_SIZE_IN_BITS / 4;
    uint16_t bitsRead = br.readBuffer(readBuffer, bitsToRead);

    TEST_ASSERT_EQUAL(bitsToRead, bitsRead);
    for (uint16_t i = 0; i < bitsToRead / 8; i++)
    {
        uint8_t expected = (buffer[i] << 3) | (buffer[i + 1] >> 5);
        TEST_ASSERT_EQUAL(expected, readBuffer[i]);
    }
}

void test_reading_state()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    uint16_t readableBits = BUFFER_SIZE_IN_BITS / 3;
    br.reset(readableBits);
    TEST_ASSERT_EQUAL(readableBits, br.bitCount);

    br.readBits(6);
    TEST_ASSERT_EQUAL(6, br.bitPosition);
    TEST_ASSERT_EQUAL(readableBits - 6, br.bitsRemaining());

    br.readBit();
    TEST_ASSERT_EQUAL(7, br.bitPosition);
    TEST_ASSERT_EQUAL(readableBits - 7, br.bitsRemaining());

    br.readBits(7);
    TEST_ASSERT_EQUAL(14, br.bitPosition);
    TEST_ASSERT_EQUAL(readableBits - 14, br.bitsRemaining());

    br.readBuffer(readBuffer, 48);
    TEST_ASSERT_EQUAL(62, br.bitPosition);
    TEST_ASSERT_EQUAL(readableBits - 62, br.bitsRemaining());
}

void test_buffer_reset_multiple_times()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    buffer[0] = 0b01010000;
    TEST_ASSERT_TRUE(br.reset(4));
    TEST_ASSERT_EQUAL(0b0101, br.readBits(4));
    TEST_ASSERT_EQUAL(4, br.bitCount);
    TEST_ASSERT_EQUAL(4, br.bitPosition);
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());

    buffer[0] = 0b10101000;
    TEST_ASSERT_TRUE(br.reset(6));
    TEST_ASSERT_EQUAL(0b101010, br.readBits(6));
    TEST_ASSERT_EQUAL(6, br.bitCount);
    TEST_ASSERT_EQUAL(6, br.bitPosition);
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());

    buffer[0] = 0b11000000;
    TEST_ASSERT_TRUE(br.reset(2));
    TEST_ASSERT_EQUAL(0b11, br.readBits(2));
    TEST_ASSERT_EQUAL(2, br.bitCount);
    TEST_ASSERT_EQUAL(2, br.bitPosition);
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
}

void test_zero_returned_for_bits_read_past_available()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    buffer[0] = 0b10101010;

    // buffer has more data, but only make 4 bits available
    br.reset(4);

    // read all available bits
    TEST_ASSERT_EQUAL(0b1010, br.readBits(8));

    // read past available bits
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
    TEST_ASSERT_EQUAL(false, br.readBit());
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
    TEST_ASSERT_EQUAL(0, br.readBuffer(readBuffer, 10 * 8));
}

void setup()
{
    delay(4000);

    UNITY_BEGIN();

    RUN_TEST(test_new_instance_is_reset_to_full_buffer_size);
    RUN_TEST(test_reset_to_less_than_buffer_size);
    RUN_TEST(test_reset_to_zero_bits_is_ok);
    RUN_TEST(test_reset_more_than_buffer_size_is_max_buffer_size);
    RUN_TEST(test_reading_single_bits);
    RUN_TEST(test_reading_multiple_bits);
    RUN_TEST(test_reading_to_buffer_byte_aligned);
    RUN_TEST(test_reading_to_buffer_byte_unaligned);
    RUN_TEST(test_reading_state);
    RUN_TEST(test_buffer_reset_multiple_times);
    RUN_TEST(test_zero_returned_for_bits_read_past_available);

    UNITY_END();
}

void loop()
{
}
