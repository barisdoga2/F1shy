#include "Arduino.h"
#include "StepperDriver.h"

StepperMotor* StepperDriver::flap;
StepperMotor* StepperDriver::pump;

int StepperDriver::HC595Registers[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

ISR(TIMER1_COMPA_vect)
{
    StepperDriver::Step(StepperDriver::flap);
    StepperDriver::Step(StepperDriver::pump);
    
    StepperDriver::WriteHC595Registers();

    // If there is nothing left to do, we can stop the interrupt
    if(StepperDriver::flap->steps_left == 0 && StepperDriver::pump->steps_left == 0)
    {
        StepperDriver::WriteHC595Registers(true);
        StepperDriver::DisableInterrupts();
    }
}

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

void StepperDriver::TurnSteps(StepperMotor* stepper, int step_count)
{
    stepper->steps_left += step_count;
    
    // If something to do, we can enable the interrupt.
    if(stepper->steps_left != 0)
    {
        EnableInterrupts();
    }
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

void StepperDriver::WriteHC595Registers(bool clear = false)
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

void StepperDriver::EnableInterrupts()
{
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    TIMSK1 = 0;
    OCR1A = CMR;
    TCCR1B |= (1 << WGM12);
    if(1024 == PRESCALER)
        TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

void StepperDriver::DisableInterrupts()
{
    cli();
    TCCR1A = 0;
    TCNT1 = 0;
    TIMSK1 = 0;
    TIMSK1 |= (0 << OCIE1A);
    sei();
}