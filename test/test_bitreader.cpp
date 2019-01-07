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

void test_reset_zero()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    br.reset(0);

    TEST_ASSERT_EQUAL(0, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(0, br.bitsRemaining());
}

void test_reset_can_not_exceed_buffer_size()
{
    Quest_BitReader br = Quest_BitReader(buffer, BUFFER_SIZE);
    uint16_t largerSize = BUFFER_SIZE_IN_BITS * 3;
    br.reset(largerSize);

    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, br.bitCount);
    TEST_ASSERT_EQUAL(0, br.bitPosition);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, br.bitsRemaining());
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_new_instance_is_reset_to_full_buffer_size);
    RUN_TEST(test_reset_to_less_than_buffer_size);
    RUN_TEST(test_reset_zero);
    RUN_TEST(test_reset_can_not_exceed_buffer_size);

    UNITY_END();
}

void loop()
{
}
