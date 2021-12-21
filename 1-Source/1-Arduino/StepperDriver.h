#ifndef Stepper_h
#define Stepper_h

#include "Types.h"
#include "PinLayout.h"

#define NUMBER_OF_STEPS_FOR_REVOLUTION 1024
#define STEP_DELAY_MS 3

#define PRESCALER       1024
#define CMR             (int)(((float)16000000 / (((float)1 / ((float)STEP_DELAY_MS / 1000)) * (float)PRESCALER)) - (float)1) // [TODO] So messy, clean it



extern "C" void TIMER1_COMPA_vect(void) __attribute__ ((signal));

class StepperDriver {
    
public:
    static void Init();
    static void TurnSteps(StepperMotor* stepper, int number_of_steps);

    static StepperMotor* flap;
    static StepperMotor* pump;
    
private:
    static void StepMotor(StepperMotor* stepper, int this_step);
    static void Step(StepperMotor* stepper);

    friend void TIMER1_COMPA_vect(void);
    static void EnableInterrupts();
    static void DisableInterrupts();

    static int HC595Registers[8];
    static void WriteHC595Registers(bool clear = false);
    
};

#endif

