#pragma once

class Flaps
{
public:
    Flaps(uint8_t pin, int pot_zero, int pot_max) : pin(pin), pot_zero(pot_zero), pot_max(pot_max)
    {
    }
    
    float read()
    {
        return map(constrain(analogRead(pin), pot_zero, pot_max), pot_zero, pot_max, 0, 100 * 100) / 100.0;
    }

    uint8_t pin;
    int pot_zero;
    int pot_max;
};