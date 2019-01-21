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

void test_writing_single_bits()
{
    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);
    bw.writeBit(true);
    bw.writeBit(false);
    bw.writeBit(true);
    TEST_ASSERT_EQUAL(3, bw.bitPosition);
    TEST_ASSERT_EQUAL(3, bw.bitsWritten());
    bw.writeBit(true);
    bw.writeBit(true);
    bw.writeBit(false);
    bw.writeBit(false);
    bw.writeBit(true);
    bw.writeBit(true);
    bw.writeBit(false);
    bw.writeBit(false);
    bw.writeBit(true);
    bw.writeBit(true);
    TEST_ASSERT_EQUAL(13, bw.bitPosition);
    TEST_ASSERT_EQUAL(13, bw.bitsWritten());

    // make sure the buffer reflects all bits written
    TEST_ASSERT_EQUAL(0b10111001, buffer[0]);
    TEST_ASSERT_EQUAL(0b10011000, buffer[1]);
}

void test_writing_multiple_bits()
{
    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);
    bw.writeBits(0b1100, 4);
    TEST_ASSERT_EQUAL(4, bw.bitPosition);
    TEST_ASSERT_EQUAL(4, bw.bitsWritten());
    bw.writeBits(0b101010, 6);
    TEST_ASSERT_EQUAL(10, bw.bitPosition);
    TEST_ASSERT_EQUAL(10, bw.bitsWritten());
    bw.writeBits(0b1, 1);
    TEST_ASSERT_EQUAL(11, bw.bitPosition);
    TEST_ASSERT_EQUAL(11, bw.bitsWritten());
    bw.writeBits(0b01010101, 8);
    TEST_ASSERT_EQUAL(19, bw.bitPosition);
    TEST_ASSERT_EQUAL(19, bw.bitsWritten());

    // make sure the buffer reflects all bits written
    TEST_ASSERT_EQUAL(0b11001010, buffer[0]);
    TEST_ASSERT_EQUAL(0b10101010, buffer[1]);
    TEST_ASSERT_EQUAL(0b10100000, buffer[2]);
}

void test_writing_bits_from_another_buffer()
{
    // clear the test buffer, we're going to check it for 0's later
    memset(buffer, 0, BUFFER_SIZE);

    // create another buffer and fill it with non-zero values
    uint8_t anotherBuffer[BUFFER_SIZE];
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        anotherBuffer[i] = random(256);
    }

    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);

    // copy part of the other buffer to the bit writer buffer
    uint16_t bitsToWrite = BUFFER_SIZE_IN_BITS / 4;
    bw.writeBuffer(anotherBuffer, bitsToWrite);

    TEST_ASSERT_EQUAL(bitsToWrite, bw.bitPosition);
    TEST_ASSERT_EQUAL(bitsToWrite, bw.bitsWritten());
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS - bitsToWrite, bw.bitsRemaining());

    // make sure the bit writer's buffer reflects the other buffer, but only to the
    // amount of bits that were copied
    TEST_ASSERT_EQUAL_INT8_ARRAY(anotherBuffer, buffer, bitsToWrite / 8);

    for (uint16_t i = bitsToWrite / 8; i < BUFFER_SIZE; i++)
    {
        TEST_ASSERT_EQUAL(0, buffer[i]);
    }
}

void test_bits_remaining()
{
    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);
    bw.writeBits(0b1010, 4);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS - 4, bw.bitsRemaining());
    bw.writeBit(true);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS - 5, bw.bitsRemaining());
    bw.writeBits(0b110011, 6);
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS - 11, bw.bitsRemaining());
}

void test_reset_to_start_of_buffer()
{
    // a new bit writer should be reset
    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(0, bw.bitPosition);
    TEST_ASSERT_EQUAL(0, bw.bitsWritten());
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, bw.bitsRemaining());

    // write some data to the buffer
    uint8_t testValue = random(256);
    for (uint16_t i = 0; i < BUFFER_SIZE / 2; i++)
    {
        bw.writeBits(testValue, 8);
    }

    // reset the buffer
    bw.reset();

    TEST_ASSERT_EQUAL(0, bw.bitPosition);
    TEST_ASSERT_EQUAL(0, bw.bitsWritten());
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, bw.bitsRemaining());
}

void test_reset_does_not_change_buffer()
{
    // fill the buffer with non-zero values
    uint8_t testValue = random(255) + 1;
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = testValue;
    }

    // constructing the bit writer resets the buffer
    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);

    // the buffer should not be touched
    TEST_ASSERT_EACH_EQUAL_INT8(testValue, buffer, BUFFER_SIZE);

    // reset the buffer
    bw.reset();

    // the buffer should not be touched
    TEST_ASSERT_EACH_EQUAL_INT8(testValue, buffer, BUFFER_SIZE);
}

void setup()
{
    delay(4000);

    UNITY_BEGIN();

    RUN_TEST(test_new_instance_is_reset_to_no_bits_written);
    RUN_TEST(test_writing_single_bits);
    RUN_TEST(test_writing_multiple_bits);
    RUN_TEST(test_writing_bits_from_another_buffer);
    RUN_TEST(test_bits_remaining);
    RUN_TEST(test_reset_to_start_of_buffer);
    RUN_TEST(test_reset_does_not_change_buffer);

    UNITY_END();
}

void loop()
{
}
