#pragma once

class Output
{
private:
    /* data */
public:
    Output(uint8_t pin) : pin(pin)
    {
        pinMode(pin, OUTPUT);
    }

    void write(bool value)
    {
        return digitalWrite(pin, value);
    }

    uint8_t pin;
};