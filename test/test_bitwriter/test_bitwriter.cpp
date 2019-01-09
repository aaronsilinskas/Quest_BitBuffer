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
    // create another buffer and fill it with non-zero values
    uint8_t anotherBuffer[BUFFER_SIZE];
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        anotherBuffer[i] = 0b11001100;
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
    for (uint16_t i = 0; i < bitsToWrite / 8; i++)
    {
        TEST_ASSERT_EQUAL(0b11001100, buffer[i]);
    }
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

void test_reset_fills_buffer_with_zeroes()
{
    // fill the buffer with non-zero values
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = 0b11110000;
    }

    // constructing the bit writer resets the buffer
    Quest_BitWriter bw = Quest_BitWriter(buffer, BUFFER_SIZE);

    // the buffer should be zero'd out
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        TEST_ASSERT_EQUAL(0, buffer[i]);
    }

    // fill the buffer with non-zero values using the bit writer
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        bw.writeBits(0b01010101, 8);
    }

    // reset the buffer again
    bw.reset();
    TEST_ASSERT_EQUAL(0, bw.bitPosition);
    TEST_ASSERT_EQUAL(0, bw.bitsWritten());
    TEST_ASSERT_EQUAL(BUFFER_SIZE_IN_BITS, bw.bitsRemaining());

    // the buffer should be zero'd out again
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        TEST_ASSERT_EQUAL(0, buffer[i]);
    }
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
    RUN_TEST(test_reset_fills_buffer_with_zeroes);

    UNITY_END();
}

void loop()
{
}
