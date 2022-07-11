// https://arduinogetstarted.com/tutorials/arduino-button-toggle-led

#include <IRremote.h>

#define FORWARD 0
#define REVERSE 1

#define MOTOR_A 0
#define MOTOR_B 1

// Pin Assignments //
#define DIRA 2  // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4  // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

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

int RECV_PIN = 13;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
    setupArdumoto();
    Serial.begin(9600);
    irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
    if (irrecv.decode(&results))
    {
        Serial.println(results.value, HEX);
        irrecv.resume(); // Receive the next value

        if (results.value == 0xA85740BF)
        {
            Serial.println("forward");
            driveArdumoto(MOTOR_A, FORWARD, 255);
            driveArdumoto(MOTOR_B, FORWARD, 255);
        }
        else if (results.value == 0xA85710EF)
        {
            Serial.println("backward");
            driveArdumoto(MOTOR_A, REVERSE, 255);
            driveArdumoto(MOTOR_B, REVERSE, 255);
        }
        else if (results.value == 0xA857609F)
        {
            turn(1);
        }
        else if (results.value == 0xA85720DF)
        {
            turn(-1);
        }
        else if (results.value == 0xA857906F)
        {
            Serial.println("backward-right");
            driveArdumoto(MOTOR_A, REVERSE, 255);
            driveArdumoto(MOTOR_B, REVERSE, 127);
        }
        else if (results.value == 0xA857E01F)
        {
            Serial.println("backward-left");
            driveArdumoto(MOTOR_A, REVERSE, 127);
            driveArdumoto(MOTOR_B, REVERSE, 255);
        }
        else if (results.value == 0xA857C03F)
        {
            Serial.println("forward-right");
            stopArdumoto(MOTOR_A);
            driveArdumoto(MOTOR_B, FORWARD, 255);
        }
        else if (results.value == 0xA857807F)
        {
            Serial.println("forward-left");
            stopArdumoto(MOTOR_B);
            driveArdumoto(MOTOR_A, FORWARD, 255);
        }
        else if (results.value == 0xA857A05F)
        {
            Serial.println("stop");
            stopArdumoto(MOTOR_A);
            stopArdumoto(MOTOR_B);
        }
    }
    delay(50);
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
