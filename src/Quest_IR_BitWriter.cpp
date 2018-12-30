#include "Quest_IR_BitWriter.h"

Quest_IR_BitWriter::Quest_IR_BitWriter(uint8_t *buffer, uint8_t bufferLength)
{
    this->buffer = buffer;
    this->bufferLength = bufferLength;

    reset();
}

void Quest_IR_BitWriter::reset()
{
    bitPosition = 0;
    bufferPosition = 0;
    bitMask = QIR_FIRST_BIT;
    memset(buffer, 0, bufferLength);
}

uint16_t Quest_IR_BitWriter::bitsWritten()
{
    return bitPosition;
}

uint16_t Quest_IR_BitWriter::bitsRemaining()
{
    return (bufferLength << 3) - bitPosition;
}

bool Quest_IR_BitWriter::writeBit(bool bit)
{
    // make sure there is enough room in the buffer
    if (bitsRemaining() == 0)
    {
        return false;
    }

    writeBitInternal(bit);

    return true;
}

bool Quest_IR_BitWriter::writeBits(uint32_t bits, uint8_t bitsToWrite)
{
    // make sure there is enough room in the buffer
    if (bitsToWrite >= bitsRemaining())
    {
        return false;
    }

    // shift the bits so the first bit to write is left-most
    bits <<= (32 - bitsToWrite);

    // add each bit to the buffer
    for (uint16_t i = 0; i < bitsToWrite; i++)
    {
        writeBitInternal(bits & QIR_FIRST_BIT_OF_INT);
        bits <<= 1;
    }

    return true;
}

inline void Quest_IR_BitWriter::writeBitInternal(bool bit)
{
    // update the bit in the buffer, assume buffer is all 0's so only need to write 1's
    if (bit)
    {
        buffer[bufferPosition] = buffer[bufferPosition] | bitMask;
    }

    bitMask >>= 1;
    if (bitMask == 0)
    {
        // no more bits in the current byte, move to the next
        bufferPosition++;
        bitMask = QIR_FIRST_BIT;
    }
    bitPosition++;
}
