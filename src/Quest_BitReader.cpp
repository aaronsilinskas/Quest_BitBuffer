#include "Quest_BitReader.h"

Quest_BitReader::Quest_BitReader(uint8_t *buffer, uint8_t bufferLength)
{
    this->buffer = buffer;
    this->bufferLength = bufferLength;

    reset(bufferLength * 8);
}

bool Quest_BitReader::reset(uint16_t bitsAvailable)
{
    // bits available should never exceed buffer size
    bitCount = min(bitsAvailable, bufferLength * 8);
    bitPosition = 0;
    bufferPosition = 0;
    bitMask = QBB_FIRST_BIT;

    return bitCount == bitsAvailable;
}

uint16_t Quest_BitReader::bitsRemaining()
{
    return bitCount - bitPosition;
}

bool Quest_BitReader::readBit()
{
    if (bitPosition >= bitCount)
    {
        // already read all available bits
        return 0;
    }

    bool bit = buffer[bufferPosition] & bitMask;
    bitMask >>= 1;
    if (bitMask == 0)
    {
        // no more bits in the current byte, move to the next
        bufferPosition++;
        bitMask = QBB_FIRST_BIT;
    }
    bitPosition++;

    return bit;
}

uint32_t Quest_BitReader::readBits(uint8_t bitsToRead)
{
    if (bitPosition >= bitCount)
    {
        // already read all available bits
        return 0;
    }

    // do not read more bits than available
    if (bitPosition + bitsToRead > bitCount)
    {
        bitsToRead = bitCount - bitPosition;
    }

    uint32_t readBits = 0;
    uint8_t bufferByte = buffer[bufferPosition];

    for (uint16_t i = 0; i < bitsToRead; i++)
    {
        // shift the read bits, and set the next bit from the buffer
        readBits <<= 1;
        if (bufferByte & bitMask)
        {
            readBits |= 1;
        }

        // move to the next bit in the buffer
        bitMask >>= 1;
        if (bitMask == 0)
        {
            // the current buffer has been read, move to the next
            bufferPosition++;
            bufferByte = buffer[bufferPosition];
            bitMask = QBB_FIRST_BIT;
        }
    }

    bitPosition += bitsToRead;

    return readBits;
}
