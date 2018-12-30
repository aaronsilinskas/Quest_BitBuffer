/* Quest_BitBuffer.h Quest Bit Buffer Library
 * Utilities to read from and write bits to a byte buffer.
 *
 * See Quest_BitReader.h to read bits and Quest_BitWriter.h to
 * write bits.
 *
 * Format:
 * Reading and writing start at byte 0 in the buffer. Within each byte, bits are
 * read and written from left-most bit to right-most bit.
 */
#ifndef quest_bitbuffer_h
#define quest_bitbuffer_h

#include <Arduino.h>

#define QBB_FIRST_BIT 0b10000000
#define QBB_FIRST_BIT_OF_INT 0x80000000

void printBinaryArray(uint8_t *buffer, uint16_t length);

#endif
