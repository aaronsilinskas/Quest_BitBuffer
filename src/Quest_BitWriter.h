/* Quest_BitWriter.h Quest Bit Writer Library
 * Writes one or more bits to a byte buffer.
 */
#ifndef quest_bitwriter_h
#define quest_bitwriter_h

#include "Quest_BitBuffer.h"

class Quest_BitWriter
{
public:
  Quest_BitWriter(uint8_t *buffer, uint8_t bufferLength);

  uint16_t bitPosition;

  void reset();
  uint16_t bitsWritten();
  uint16_t bitsRemaining();

  bool writeBit(bool bit);
  bool writeBits(uint32_t bits, uint8_t bitsToWrite);
  bool writeBuffer(uint8_t *buffer, uint16_t bitsToWrite);

private:
  uint8_t *buffer;
  uint8_t bufferLength;
  uint8_t bufferPosition;
  uint8_t bitMask;

  void writeBitInternal(bool bit);
};

#endif
