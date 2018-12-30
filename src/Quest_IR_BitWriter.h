/* Quest_IR_BitWriter.h Quest IR Bit Writer Library
 * Writes one or more bits to a byte buffer.
 */
#ifndef quest_ir_bitwriter_h
#define quest_ir_bitwriter_h

#include "Quest_IR_BitBuffer.h"

class Quest_IR_BitWriter
{
  public:
    Quest_IR_BitWriter(uint8_t *buffer, uint8_t bufferLength);

    uint16_t bitPosition;

    void reset();
    uint16_t bitsWritten();
    uint16_t bitsRemaining();

    bool writeBit(bool bit);
    bool writeBits(uint32_t bits, uint8_t bitsToWrite);

  private:
    uint8_t *buffer;
    uint8_t bufferLength;
    uint8_t bufferPosition;
    uint8_t bitMask;

    void writeBitInternal(bool bit);
};

#endif
