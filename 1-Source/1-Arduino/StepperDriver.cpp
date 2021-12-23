#include "Arduino.h"
#include "StepperDriver.h"

StepperMotor* StepperDriver::flap;
StepperMotor* StepperDriver::pump;

int StepperDriver::HC595Registers[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};



void StepperDriver::Init()
{
    // Initialize Steppers
    flap = new StepperMotor{STEPPER_1_1, STEPPER_1_2, STEPPER_1_3, STEPPER_1_4};
    pump = new StepperMotor{STEPPER_2_1, STEPPER_2_2, STEPPER_2_3, STEPPER_2_4};
    
    // [TODO] Without this stepper motors does not work, investigate.
    pinMode(NOT_A_PIN, OUTPUT); 
    
    // Clear HC595 Registers
    WriteHC595Registers(true);    
}

bool StepperDriver::OnISR()
{
    Step(flap);
    Step(pump);
    WriteHC595Registers();
    // If there is nothing left to do, we can stop the interrupt
    if(flap->steps_left == 0 && pump->steps_left == 0)
    {
        WriteHC595Registers(true);
        return true;
    }
    return false;
}

bool StepperDriver::TurnSteps(StepperMotor* stepper, int step_count)
{
    bool retVal = false;
    if(step_count != 0)
    {
        retVal = ISRHandler::EnableInterrupt(STEP_DELAY_MS, OnISR);
        if(retVal)
            stepper->steps_left += step_count;
    }
    return retVal;
}

void StepperDriver::Step(StepperMotor* stepper)
{
    if (stepper->steps_left > 0) { stepper->direction = 1; }
    if (stepper->steps_left < 0) { stepper->direction = 0; }
    if (abs(stepper->steps_left) > 0)
    {
        if (stepper->direction == 1)
        {
            stepper->current_step_number++;
            if (stepper->current_step_number == NUMBER_OF_STEPS_FOR_REVOLUTION) {
                stepper->current_step_number = 0;
            }
        }
        else
        {
            if (stepper->current_step_number == 0) {
                stepper->current_step_number = NUMBER_OF_STEPS_FOR_REVOLUTION;
            }
            stepper->current_step_number--;
        }
        if(stepper->steps_left > 0)
            stepper->steps_left--;
        else if(stepper->steps_left < 0)
            stepper->steps_left++;
        StepMotor(stepper, stepper->current_step_number % 4);
    }
}

void StepperDriver::StepMotor(StepperMotor* stepper, int thisStep)
{
    switch (thisStep) {
    case 0:  // 1010
        HC595Registers[stepper->motor_pin_1] = HIGH;
        HC595Registers[stepper->motor_pin_2] = LOW;
        HC595Registers[stepper->motor_pin_3] = HIGH;
        HC595Registers[stepper->motor_pin_4] = LOW;
        break;
    case 1:  // 0110
        HC595Registers[stepper->motor_pin_1] = LOW;
        HC595Registers[stepper->motor_pin_2] = HIGH;
        HC595Registers[stepper->motor_pin_3] = HIGH;
        HC595Registers[stepper->motor_pin_4] = LOW;
        break;
    case 2:  //0101
        HC595Registers[stepper->motor_pin_1] = LOW;
        HC595Registers[stepper->motor_pin_2] = HIGH;
        HC595Registers[stepper->motor_pin_3] = LOW;
        HC595Registers[stepper->motor_pin_4] = HIGH;
        break;
    case 3:  //1001
        HC595Registers[stepper->motor_pin_1] = HIGH;
        HC595Registers[stepper->motor_pin_2] = LOW;
        HC595Registers[stepper->motor_pin_3] = LOW;
        HC595Registers[stepper->motor_pin_4] = HIGH;
        break;
     case 4:  // Only used to clear HC595 Registers
        HC595Registers[stepper->motor_pin_1] = LOW;
        HC595Registers[stepper->motor_pin_2] = LOW;
        HC595Registers[stepper->motor_pin_3] = LOW;
        HC595Registers[stepper->motor_pin_4] = LOW;
        break;
    }
}

void StepperDriver::WriteHC595Registers(bool clear)
{
    digitalWrite(HC595_LATCH, LOW);
    for(int i = 7 ; i >=  0; i--)
    {
        if(clear == 1)
            HC595Registers[i] = LOW;
        digitalWrite(HC595_CLOCK, LOW);
        digitalWrite(HC595_DATA, HC595Registers[i]);
        digitalWrite(HC595_CLOCK, HIGH);

    }
    digitalWrite(HC595_LATCH, HIGH);
}