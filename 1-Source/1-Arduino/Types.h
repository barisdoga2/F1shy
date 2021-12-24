#ifndef TYPES_H
#define TYPES_H

#define UINT_MAX 32767

enum PumpActuatorTarget
{
    REAR_WATER_TANK = -1,
    HEAD_WATER_TANK = 1
};

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
    PumpActuatorTarget target; // Only used for pump actuator
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