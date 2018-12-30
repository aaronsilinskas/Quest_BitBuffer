#include "Quest_IR_BitBuffer.h"

void printBinaryArray(uint8_t *buffer, uint16_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        Serial.print(buffer[i], BIN);
        Serial.print(F(" "));
    }
    Serial.println();
}
