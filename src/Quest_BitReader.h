/* Quest_BitReader.h Quest Bit Reader Library
 * Reads one or more bits from a byte buffer.
 */
#ifndef quest_bitreader_h
#define quest_bitreader_h

#include "Quest_BitBuffer.h"

class Quest_BitReader
{
public:
  Quest_BitReader(uint8_t *buffer, uint8_t bufferLength);

  uint16_t bitCount;
  uint16_t bitPosition;

  void reset(uint16_t bitsAvailable);
  uint16_t bitsRemaining();

  bool readBit();
  uint32_t readBits(uint8_t bitsToRead);

private:
  uint8_t *buffer;
  uint8_t bufferLength;
  uint8_t bufferPosition;
  uint8_t bitMask;
};

#endif
