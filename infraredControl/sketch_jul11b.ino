#include <Arduino.h>
#include <IRremote.hpp>
#include "PinDefinitionsAndMore.h" //Define macros for input and output pin etc.

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols are active.
 */
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // the same as DECODE_KASEIKYO
//#define DECODE_LG
#define DECODE_NEC            // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER

//#define DECODE_DISTANCE     // universal decoder for pulse distance protocols
//#define DECODE_HASH         // special decoder for all protocols

//#define DEBUG               // Activate this for lots of lovely debug output from the decoders.

// Motor Direction //
#define FORWARD 0
#define REVERSE 1

#define MOTOR_A 0
#define MOTOR_B 1

// Pin Assignments //
#define DIRA 2  // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4  // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

void setup()
{
    setupArdumoto();
    Serial.begin(9600);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    //    printActiveIRProtocols(&Serial);
    Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));
}

void loop()
{
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (IrReceiver.decode())
    {
        // Print a short summary of received data
        IrReceiver.printIRResultShort(&Serial);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN)
        {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }

        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume(); // Enable receiving of the next value

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == 0x14)
        {
            Serial.println("forward");
            driveArdumoto(MOTOR_A, FORWARD, 255);
            driveArdumoto(MOTOR_B, FORWARD, 255);
        }
        else if (IrReceiver.decodedIRData.command == 0x16)
        {
            Serial.println("backward");
            driveArdumoto(MOTOR_A, REVERSE, 255);
            driveArdumoto(MOTOR_B, REVERSE, 255);
        }
        else if (IrReceiver.decodedIRData.command == 0x11)
        {
            Serial.println("in-place right");
            turn(1);
        }
        else if (IrReceiver.decodedIRData.command == 0x19)
        {
            Serial.println("in-place left");
            turn(-1);
        }
        else if (IrReceiver.decodedIRData.command == 0x12)
        {
            Serial.println("backward-right");
            driveArdumoto(MOTOR_A, REVERSE, 255);
            driveArdumoto(MOTOR_B, REVERSE, 127);
        }
        else if (IrReceiver.decodedIRData.command == 0x1A)
        {
            Serial.println("backward-left");
            driveArdumoto(MOTOR_A, REVERSE, 127);
            driveArdumoto(MOTOR_B, REVERSE, 255);
        }
        else if (IrReceiver.decodedIRData.command == 0x10)
        {
            Serial.println("forward-right");
            stopArdumoto(MOTOR_A);
            driveArdumoto(MOTOR_B, FORWARD, 255);
        }
        else if (IrReceiver.decodedIRData.command == 0x18)
        {
            Serial.println("forward-left");
            stopArdumoto(MOTOR_B);
            driveArdumoto(MOTOR_A, FORWARD, 255);
        }
        else if (IrReceiver.decodedIRData.command == 0x15)
        {
            Serial.println("stop");
            stopArdumoto(MOTOR_A);
            stopArdumoto(MOTOR_B);
        }
    }
}

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
    if (motor == MOTOR_A)
    {
        digitalWrite(DIRA, dir);
        analogWrite(PWMA, spd);
    }
    else if (motor == MOTOR_B)
    {
        digitalWrite(DIRB, dir);
        analogWrite(PWMB, spd);
    }
}

// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
    driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
    // All pins should be setup as outputs:
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(DIRA, OUTPUT);
    pinMode(DIRB, OUTPUT);
    // Initialize all pins as low:
    digitalWrite(PWMA, LOW);
    digitalWrite(PWMB, LOW);
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, LOW);
}

void turn(int direction) // -1 left, 1 right
{
    if (direction == -1)
    {
        driveArdumoto(MOTOR_A, FORWARD, 255);
        driveArdumoto(MOTOR_B, REVERSE, 255);
    }
    else if (direction == 1)
    {
        driveArdumoto(MOTOR_A, REVERSE, 255);
        driveArdumoto(MOTOR_B, FORWARD, 255);
    }
}
