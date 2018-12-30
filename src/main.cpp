/* receive-encoded.ino Example sketch for Quest IR Library
 * This example will receive IR signals and decode them
 * based on the Quest IR format.
 *
 * Note: Update PIN_IR_RECEIVER to the pin connected to
 * an IR receiver such as the Vishay TSOP38238.
 */

#include "Quest_IR_Receiver.h"

// update this pin
const int PIN_IR_RECEIVER = 7;
Quest_IR_Receiver irReceiver;

void setup()
{
    Serial.begin(9600);

    // start to listen for IR signals on the given pin
    irReceiver.begin(PIN_IR_RECEIVER);
    irReceiver.enableBlink(true);
}

void loop()
{
    // check to see if the receiver decoded a signal
    if (irReceiver.hasSignal())
    {
        // data was received, dump it to Serial
        irReceiver.printRawSignal();

        // output the received bits
        Serial.print("Received ");
        Serial.print(irReceiver.decodedBitCount);
        Serial.print(" bits: ");

        uint8_t bytesWithData = irReceiver.decodedBitCount / 8;
        if (irReceiver.decodedBitCount % 8 > 0)
        {
            bytesWithData++;
        }

        for (int i = 0; i < bytesWithData; i++)
        {
            uint8_t bits = irReceiver.decodedBits[i];
            Serial.print(bits, BIN);
            Serial.print(" ");
        }
        Serial.println();

        // make sure the receiver is reset to decode the next signal
        irReceiver.reset();
    }
}
