#ifndef TYPES_H
#define TYPES_H

struct Pin{
    int pinNumber;
    int mode;
    int state;
};

struct StepperMotor
{
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
    
    int direction;
    int current_step_number;
    int steps_left;
};

struct EngineMotor
{
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_enable;
    float acceleration; // pwr/ms

    int target_power;
    float current_power;
    long startTime;
    long lastAcceleration;
};

struct PumpMotor
{
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_enable;
};


#endif