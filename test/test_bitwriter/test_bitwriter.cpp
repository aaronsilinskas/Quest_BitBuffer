#include <Arduino.h>
#include <unity.h>

#include "Quest_BitWriter.h"

#define BUFFER_SIZE 48
#define BUFFER_SIZE_IN_BITS BUFFER_SIZE * 8

uint8_t buffer[BUFFER_SIZE];

void test_new_instance_is_reset_to_no_bits_written()
{
    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(0, bw.bitsWritten());
    TEST_ASSERT_EQUAL(0, bw.bitPosition);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, bw.bitsRemaining());
}

void setup()
{
    delay(4000);

    UNITY_BEGIN();

    RUN_TEST(test_new_instance_is_reset_to_no_bits_written);

    UNITY_END();
}

void loop()
{
}
