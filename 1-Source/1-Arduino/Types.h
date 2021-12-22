#ifndef TYPES_H
#define TYPES_H

typedef struct Pin{
    int pinNumber;
    int mode;
    int state;
} Pin;

typedef struct StepperMotor
{
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
    
    int direction;
    int current_step_number;
    int steps_left;
};

typedef struct EngineMotor
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

typedef struct PumpMotor
{
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_enable;

    bool isPumping;
    unsigned long endTime;
};


#endif