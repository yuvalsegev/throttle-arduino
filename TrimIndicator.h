#pragma once

#define TRIM_0 (-325)
#define TRIM_STEPPER_MAX (800)
class TrimIndicator
{
public:
    TrimIndicator(AccelStepper *stepper, uint8_t endstop_pin, int zero_pos) : stepper(stepper), endstop_pin(endstop_pin), zero_pos(zero_pos), initalized(false)
    {
        pinMode(endstop_pin, INPUT_PULLUP);
        stepper->setAcceleration(10000);
        stepper->setMaxSpeed(400);
        start_time = millis();
    }

    void init()
    {
        if (!endstop() && (millis() - start_time) < 10000)
        {
            stepper->move(-5);
            stepper->run();
        }
        else
        {
            stepper->setCurrentPosition(0);
            initalized = true;
        }
        
    }

    void run(int value)
    {
        if(!initalized)
        {
            init();
            return;
        }

        int stepper_value = constrain(map(value, TRIM_0, 1000, 0, TRIM_STEPPER_MAX), 0, TRIM_STEPPER_MAX);
        stepper->moveTo(stepper_value + zero_pos);
        stepper->run();
    }

    bool endstop()
    {
        return !digitalRead(endstop_pin);
    }

    long start_time;
    bool initalized;
    AccelStepper *stepper;
    uint8_t endstop_pin;
    int zero_pos;
};