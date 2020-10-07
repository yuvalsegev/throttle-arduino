#pragma once

#include <AccelStepper.h>

#define MAX_THROTTLE 87.7
#define MIN_THROTTLE -37.2

class Button
{
private:
    /* data */
public:
    Button(uint8_t pin) : pin(pin)
    {
        pinMode(pin, INPUT);
        digitalWrite(pin, HIGH);
    }

    bool read()
    {
        return !digitalRead(pin);
    }

    uint8_t pin;
};