#pragma once

class Parkingbrake
{
public:
    Parkingbrake(uint8_t sw_pin, uint8_t solenoid_pin) : sw_pin(sw_pin), solenoid_pin(solenoid_pin), last_sim_val(true)
    {
        pinMode(sw_pin, INPUT);
        digitalWrite(sw_pin, HIGH);
        pinMode(solenoid_pin, OUTPUT);
        digitalWrite(solenoid_pin, HIGH);
    }

    bool read()
    {
        bool value = !digitalRead(sw_pin);
        if(value && !last_sw_val)
        {
            last_up_time = millis();
        }

        last_sw_val = value;
        return value;
    }

    void solenoid(bool value)
    {
        // Not yet ready, waiting for diode.
        digitalWrite(solenoid_pin, !value);
    }

    void run(bool sim_val)
    {
        if(!sim_val && read() && start_time == 0 && (millis() - last_up_time > 500))
        {
            start_time = millis();
            solenoid(true);
        }
        else if (sim_val || ((millis() - start_time) > 500))
        {
            solenoid(false);
        }

        if(((millis() - start_time) > 500) && !read())
        {
            start_time = 0;
        }
        last_sim_val = sim_val;
    }

    uint8_t sw_pin;
    uint8_t solenoid_pin;
    bool last_sim_val;
    long start_time;
    long last_up_time;
    bool last_sw_val;
};
