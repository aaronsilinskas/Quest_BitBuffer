#include "Quest_BitWriter.h"

Quest_BitWriter::Quest_BitWriter(uint8_t *buffer, uint8_t bufferLength)
{
    this->buffer = buffer;
    this->bufferLength = bufferLength;

    reset();
}

void Quest_BitWriter::reset()
{
    bitPosition = 0;
    bufferPosition = 0;
    bitMask = QBB_FIRST_BIT;
}

uint16_t Quest_BitWriter::bitsWritten()
{
    return bitPosition;
}

uint16_t Quest_BitWriter::bitsRemaining()
{
    return (bufferLength << 3) - bitPosition;
}

bool Quest_BitWriter::writeBit(bool bit)
{
    // make sure there is enough room in the buffer
    if (bitsRemaining() == 0)
    {
        return false;
    }

    writeBitInternal(bit);

    return true;
}

bool Quest_BitWriter::writeBits(uint32_t bits, uint8_t bitsToWrite)
{
    // make sure there is enough room in the buffer
    if (bitsToWrite > bitsRemaining())
    {
        return false;
    }

    // shift the bits so the first bit to write is left-most
    bits <<= (32 - bitsToWrite);

    // add each bit to the buffer
    for (uint16_t i = 0; i < bitsToWrite; i++)
    {
        writeBitInternal(bits & QBB_FIRST_BIT_OF_INT);
        bits <<= 1;
    }

    return true;
}

bool Quest_BitWriter::writeBuffer(uint8_t *sourceBuffer, uint16_t bitsToWrite)
{
    // make sure there is enough room in the buffer
    if (bitsToWrite > bitsRemaining())
    {
        return false;
    }

    uint8_t sourceBits = sourceBuffer[0];
    uint8_t sourceBufferPosition = 0;
    uint8_t sourceBitMask = QBB_FIRST_BIT;

    // write each bit
    for (uint16_t i = 0; i < bitsToWrite; i++)
    {
        writeBitInternal(sourceBits & sourceBitMask);

        sourceBitMask >>= 1;
        if (sourceBitMask == 0)
        {
            sourceBufferPosition++;
            sourceBits = sourceBuffer[sourceBufferPosition];
            sourceBitMask = QBB_FIRST_BIT;
        }
    }

    return true;
}

inline void Quest_BitWriter::writeBitInternal(bool bit)
{
    // if we're writing the top bit, we need clear the remaining 7 bits
    if (bitMask == QBB_FIRST_BIT)
    {
        if (bit)
        {
            buffer[bufferPosition] = bitMask;
        }
        else
        {
            buffer[bufferPosition] = 0;
        }
    }
    else
    {
        // update the bit in the buffer, we only need to write 1's because we reset all values
        // to 0 when setting the top bit
        if (bit)
        {
            buffer[bufferPosition] = buffer[bufferPosition] | bitMask;
        }
    }

    bitMask >>= 1;
    if (bitMask == 0)
    {
        // no more bits in the current byte, move to the next
        bufferPosition++;
        bitMask = QBB_FIRST_BIT;
    }
    bitPosition++;
}
