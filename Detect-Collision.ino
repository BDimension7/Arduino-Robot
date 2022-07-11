#include <AntiKeyRepetition.h>

AntiKeyRepetition power(2);
AntiKeyRepetition collision(3);

int motorState = -1; // current state of motors

// motor control functions
void halt()
{
    analogWrite(5, 0);
    analogWrite(6, 0);
    analogWrite(10, 0);
    analogWrite(11, 0);
}

void backward(int s) // speed (s) is 0-255
{
    analogWrite(5, 0);
    analogWrite(6, s);
    analogWrite(10, 0);
    analogWrite(11, s);
}

void forward(int s)
{
    analogWrite(5, s);
    analogWrite(6, 0);
    analogWrite(10, s);
    analogWrite(11, 0);
}

void turn(int direction) // 0 left, 1 right
{
    if (direction == 0)
    {
        analogWrite(5, 0);
        analogWrite(6, 0);
        analogWrite(10, 0);
        analogWrite(11, 255);
    }
    else if (direction == 1)
    {
        analogWrite(5, 0);
        analogWrite(6, 0);
        analogWrite(10, 255);
        analogWrite(11, 0);
    }
    delay(random(1000, 2000));
}

void backturn(int direction)
{
    if (direction == 0)
    {
        analogWrite(5, 0);
        analogWrite(6, 100);
        analogWrite(10, 0);
        analogWrite(11, 255);
    }
    else if (direction == 1)
    {
        analogWrite(5, 0);
        analogWrite(6, 255);
        analogWrite(10, 0);
        analogWrite(11, 100);
    }
    delay(random(500, 1000));
}

void setup()
{
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(A0, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop()
{
    int photoval = analogRead(A0);
    if (power.getAccessState() == 1)
    {
        Serial.println("motorState update");
        motorState = -motorState;
    }
    if ((collision.getAccessState() == 1 || photoval > 600) && motorState > 0)
    {
        Serial.println("collision detected");
        backturn(random(0, 2));
    }
    else
    {
        if (motorState > 0)
        {
            forward(255);
        }
        else if (motorState < 0)
        {
            halt();
        }
    }
    collision.delAccess();
    power.delAccess();
    delay(20);
}
