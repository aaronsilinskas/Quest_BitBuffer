#include "Quest_BitBuffer.h"

void printBinaryArray(uint8_t *buffer, uint16_t length, const String &byteDelimiter)
{
    for (uint8_t i = 0; i < length; i++)
    {
        uint8_t bitMask = 0b10000000;
        for (uint8_t b = 0; b < 8; b++)
        {
            if (buffer[i] & bitMask)
            {
                Serial.print(F("1"));
            }
            else
            {
                Serial.print(F("0"));
            }
            bitMask >>= 1;
        }
        Serial.print(byteDelimiter);
    }
    Serial.println();
}
