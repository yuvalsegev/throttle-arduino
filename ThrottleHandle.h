#pragma once

#include <AccelStepper.h>

#define MAX_THROTTLE 87.7
#define MIN_THROTTLE -37.2

class ThrottleHandle
{
private:
    /* data */
public:
    ThrottleHandle(AccelStepper *stepper,
                   uint8_t enable_pin,
                   uint8_t pot_pin,
                   int pot_zero,
                   int pot_max,
                   bool pot_inv,
                   uint8_t rev_pin,
                   int rev_zero,
                   int rev_max,
                   bool rev_inv) : stepper(stepper),
                                   enable_pin(enable_pin),
                                   pot_pin(pot_pin),
                                   pot_zero(pot_zero),
                                   pot_max(pot_max),
                                   pot_inv(pot_inv),
                                   rev_pin(rev_pin),
                                   rev_zero(rev_zero),
                                   rev_max(rev_max),
                                   rev_inv(rev_inv)
    {
        pinMode(enable_pin, OUTPUT);
        stepper->setEnablePin(enable_pin);
        stepper->setPinsInverted(false, false, true);
        stepper->setAcceleration(10000);
        stepper->setMaxSpeed(300);
    }

    float read_value()
    {
        float pot_value = map(analogRead(pot_pin), pot_zero, pot_max, 0, MAX_THROTTLE * 100) / 100.0;
        float rev_value = map(analogRead(rev_pin), rev_zero, rev_max, 0, MIN_THROTTLE * 100) / 100.0;

        // if (pot_value < 10 && rev_value < 0)
        // {
        //     return constrain(rev_value, MIN_THROTTLE, 0);
        // }

        return constrain(pot_value, 0, MAX_THROTTLE);
    }

    void run(bool motor_enabled = false, float value = 0)
    {
        float handle_value = read_value();
        // Serial.println(handle_value);
        if (motor_enabled && (handle_value >= 0))
        {
            float lower_value_tol = value - 1;
            float higher_value_tol = value + 1;
            stepper->enableOutputs();
            if ((handle_value > lower_value_tol) &&
                (handle_value < higher_value_tol))
            {
                // If value between tolerance, do nothing.
                return;
            }
            if (handle_value < value)
            {
                if(value - handle_value > 3)
                {
                    stepper->move(5);
                    Serial.println("move");
                }
                else
                {
                    stepper->move(1);
                    Serial.println("move");
                }
                // Serial.println("up");
            }
            else
            {
                if(handle_value - value > 3)
                {
                    stepper->move(-5);
                    Serial.println("move");
                }
                else
                {
                    stepper->move(-1);
                    Serial.println("move");
                }
                
                // Serial.println("down");
            }

            stepper->run();
        }
        else
        {
            stepper->disableOutputs();
            return;
        }
    }

    AccelStepper *stepper;
    uint8_t enable_pin;
    uint8_t pot_pin;
    int pot_zero;
    int pot_max;
    bool pot_inv;
    uint8_t rev_pin;
    int rev_zero;
    int rev_max;
    bool rev_inv;
    uint8_t at_disengage_pin;
    uint8_t toga_pin;
};