#include <Arduino.h>
#include <unity.h>

#include "Quest_BitReader.h"

#define BUFFER_SIZE 48
#define BUFFER_SIZE_IN_BITS BUFFER_SIZE * 8

uint8_t buffer[BUFFER_SIZE];

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
    br.reset(smallerSize);

    TEST_ASSERT_EQUAL(smallerSize, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(smallerSize, br.bitsRemaining());
}

void test_reset_to_zero_bits_is_ok()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    br.reset(0);

    TEST_ASSERT_EQUAL(0, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
}

void test_reset_more_than_buffer_size_is_max_buffer_size()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    uint16_t largerSize = BUFFER_SIZE_IN_BITS * 3;
    br.reset(largerSize);

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
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
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
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
}

void test_bits_remaining()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    buffer[0] = 0b01010101;
    buffer[1] = 0b01010000;
    br.reset(12); // buffer has 12 bits of a 0 1 pattern

    TEST_ASSERT_EQUAL(0b010101, br.readBits(6));
    TEST_ASSERT_EQUAL(0b010, br.readBits(3));
    TEST_ASSERT_EQUAL(3, br.bitsRemaining());
}

void test_buffer_reset_multiple_times()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    buffer[0] = 0b01010000;
    br.reset(4);
    TEST_ASSERT_EQUAL(0b0101, br.readBits(4));
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());

    buffer[0] = 0b10101000;
    br.reset(6);
    TEST_ASSERT_EQUAL(0b101010, br.readBits(6));
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());

    buffer[0] = 0b11000000;
    br.reset(2);
    TEST_ASSERT_EQUAL(0b11, br.readBits(2));
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
}

void test_zero_returned_for_bits_read_past_available()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    buffer[0] = 0b10101010;
    br.reset(4); // buffer has more data, but only make 4 bits available
    TEST_ASSERT_EQUAL(0b1010, br.readBits(8));
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
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
    RUN_TEST(test_bits_remaining);
    RUN_TEST(test_buffer_reset_multiple_times);
    RUN_TEST(test_zero_returned_for_bits_read_past_available);

    UNITY_END();
}

void loop()
{
}
