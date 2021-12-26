#include "Arduino.h"
#include "StepperDriver.h"

StepperMotor* StepperDriver::flap;
StepperMotor* StepperDriver::pumpActuator;

int StepperDriver::HC595Registers[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};



void StepperDriver::Init()
{
    // Initialize Steppers
    flap = new StepperMotor{STEPPER_1_1, STEPPER_1_2, STEPPER_1_3, STEPPER_1_4};
    pumpActuator = new StepperMotor{STEPPER_2_1, STEPPER_2_2, STEPPER_2_3, STEPPER_2_4};
    StepMotor(pumpActuator, 0); // Pump Actuator must be locked.
    
    pinMode(NOT_A_PIN, OUTPUT); // [TODO] Without this stepper motors does not work, investigate.
    
    // Clear HC595 Registers
    WriteHC595Registers();    
}

bool StepperDriver::PumpActuatorOnISR()
{
    bool retVal = false;

    Step(pumpActuator);
    WriteHC595Registers();

    int sensorVal = analogRead(pumpActuator->target == REAR_WATER_TANK ? PUMP_ACT_R_SENS : PUMP_ACT_H_SENS) > 200;
    if(sensorVal == 1)
    {
        pumpActuator->steps_left = 0;
        retVal = true;
    }

    return retVal;
}

bool StepperDriver::FlapOnISR()
{
    bool retVal = false;

    Step(flap);
    WriteHC595Registers();

    retVal = flap->steps_left == 0;
    if(retVal)
    {
        //StepMotor(flap, 4);//Only unlock flap stepper.
        WriteHC595Registers();
    }
    return retVal;
}

bool StepperDriver::TurnFlap(int number_of_steps)
{
    bool retVal = false;
    if(number_of_steps != 0)
    {
        retVal = ISRHandler::EnableInterrupt(STEP_DELAY_MS, FlapOnISR);
        if(retVal)
        {
            flap->steps_left = number_of_steps;
        }
    }
    return retVal;
}

bool StepperDriver::TurnPumpActuator(PumpActuatorTarget targetTank)
{
    bool retVal = false;

    retVal = ISRHandler::EnableInterrupt(STEP_DELAY_MS, PumpActuatorOnISR);
    if(retVal)
    {
        pumpActuator->steps_left = targetTank * 1233;
        //pumpActuator->steps_left = targetTank * 1233; //[TODO] Activate sensor mechanism first
        pumpActuator->target = targetTank;
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

void StepperDriver::WriteHC595Registers()
{
    digitalWrite(HC595_LATCH, LOW);
    for(int i = 7 ; i >=  0; i--)
    {
        digitalWrite(HC595_CLOCK, LOW);
        digitalWrite(HC595_DATA, HC595Registers[i]);
        digitalWrite(HC595_CLOCK, HIGH);

    }
    digitalWrite(HC595_LATCH, HIGH);
}