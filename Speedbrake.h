#pragma once

#include <AccelStepper.h>
#include <Servo.h>
#include <ServoEasing.hpp>

#define MAX_SB 75.3
#define MIN_SB 0

class Speedbrake
{
private:
    /* data */
public:
    Speedbrake(uint8_t servo_pin, uint8_t pot_pin, int pot_zero, int pot_max) : servo_pin(servo_pin),
                                                                                pot_pin(pot_pin), 
                                                                                pot_zero(pot_zero),
                                                                                pot_max(pot_max),
                                                                                servo_attached(false),
                                                                                target_value(-1)
    {
    }

    float read()
    {
        float value = map(analogRead(pot_pin), pot_zero, pot_max, MIN_SB * 100, MAX_SB * 100) / 100.0;
        if(value > MAX_SB)
        {
            return MAX_SB;
        }
        else if (abs(value - 0.0) < 3.5)
        {
            return 0;
        }
        
        return value;
    }

    bool run(float value, int enable)
    {
        // servo.attach(servo_pin);
        // servo.write(0);
        // return true;
        // return false;
        float handle_value = read();
        if ((abs(value - handle_value) > 70 && enable && handle_value >= 0) || target_value != -1)
        {
            target_value = value;
            if (!servo_attached)
            {
                servo.attach(servo_pin);
                servo_attached = true;
            }
            if (value == 0)
            {
                servo.write(180);
            }
            if (value >= 72 && handle_value >= 0)
            {
                servo.write(0);
            }
            if(abs(handle_value - value) < 5 || (value == 0 && handle_value < 0))
            {
                target_value = -1;
            }
            return true;
        }
        else
        {
            servo.detach();
            servo_attached = false;
            return false;
        }
    }

    bool servo_attached;
    uint8_t servo_pin;
    uint8_t pot_pin;
    int pot_zero;
    int pot_max;
    ServoEasing servo;
    float target_value;
};