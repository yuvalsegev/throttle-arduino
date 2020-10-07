#include "ThrottleHandle.h"
#include "AccelStepper.h"
#include "./Button.h"
#include "./throttle_interface.h"
#include <Servo.h>
#include "./Speedbrake.h"
#include "./Flaps.h"
#include "./Parkingbrake.h"
#include "./Output.h"
#include "./TrimIndicator.h"

AccelStepper right_thr_stepper(AccelStepper::FULL4WIRE, 12, 10, 9, 11);
AccelStepper left_thr_stepper(AccelStepper::FULL4WIRE, 7, 5, 4, 6);
ThrottleHandle right_thr(&right_thr_stepper, 8, A1, 980, 555, false, A0, 715, 550, false);
ThrottleHandle left_thr(&left_thr_stepper, 22, A2, 458, 903, false, A3, 640, 470, false);
Speedbrake speedbrake(3, A4, 440, 41);
Flaps flaps(A5, 0, 750);
Parkingbrake parking_brake(24, 23);
Output pb_light(25);
AccelStepper left_trim_stepper(AccelStepper::FULL4WIRE, 32, 28, 26, 30);
AccelStepper right_trim_stepper(AccelStepper::FULL4WIRE, 35, 31, 29, 33);
TrimIndicator left_trim(&left_trim_stepper, 27, 225);
TrimIndicator right_trim(&right_trim_stepper, 34, 210);

Button r_at(16);
Button r_toga(17);
Button r_cutoff_low(15);
Button l_at(18);
Button l_toga(19);
Button l_cutoff_low(20);
Button l_cutoff_high(21);

ServoEasing spoilers_servo;

throttle_data_down_t down_data = {0};
throttle_data_up_t up_data = {0};


void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(1000);
    down_data.pb = 1;
    
    // Serial.println("up");
    // pinMode(23, OUTPUT);
    // pinMode(25, OUTPUT);
    // digitalWrite(25, LOW);
    // // // pinMode(21, INPUT);
    // // digitalWrite(23, LOW);
    // // delay(500);
    // digitalWrite(23, LOW);
    // delay(1000);
    // digitalWrite(23, HIGH);

    while (true)
    {
        left_thr.run(true, 50);
        right_thr.run(true, 50);
        // Serial.println(analogRead(left_thr.pot_pin));
    }
    
}

// void loop()
// {
// //     // parking_brake.run(false);
// // //     // right_thr.run(true, MAX_THROTTLE);
// // //     // left_thr.run(true, MAX_THROTTLE);
// //     // Serial.println(analogRead(A2));
// // // //     // digitalWrite(13, digitalRead(21));

//     left_trim.run(500);
// }


#define BARKER (0xCA)

void loop()
{

    if(Serial.available())
    {
        char barker = Serial.read();
        if((uint8_t)barker == BARKER)
        {
            Serial.readBytes((char *)&down_data, sizeof(down_data));
            Serial.write(barker);
            Serial.write((char *)&up_data, sizeof(up_data));
        }
    }
    
    right_thr.run(down_data.throttle_motors_connected, down_data.right_val);
    left_thr.run(down_data.throttle_motors_connected, down_data.left_val);
    up_data.sb_moving = speedbrake.run(down_data.sb_value, down_data.sb_move_enable);
    parking_brake.run(down_data.pb);
    pb_light.write(!down_data.pb_light);
    left_trim.run(down_data.trim);
    right_trim.run(down_data.trim);
    
    up_data.right_val = right_thr.read_value();
    up_data.left_val = left_thr.read_value();
    up_data.r_at = r_at.read();
    up_data.r_toga = r_toga.read();
    up_data.r_cutoff_low = r_cutoff_low.read();
    up_data.l_at = l_at.read();
    up_data.l_toga = l_toga.read();
    up_data.l_cutoff_low = l_cutoff_low.read();
    up_data.l_cutoff_high = l_cutoff_high.read();
    up_data.sb_value = speedbrake.read();
    up_data.flaps_val = flaps.read();
    up_data.pb = parking_brake.read();
    // right_thr_stepper.runToNewPosition(100);
    // right_thr_stepper.move(10);
    // right_thr_stepper.run();
    // delay(10);
}