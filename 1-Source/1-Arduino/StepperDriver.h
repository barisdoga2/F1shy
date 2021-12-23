#ifndef Stepper_h
#define Stepper_h

#include "Types.h"
#include "PinLayout.h"
#include "ISRHandler.h"

#define NUMBER_OF_STEPS_FOR_REVOLUTION 1024
#define STEP_DELAY_MS 3



class StepperDriver {
    
public:
    static void Init();
    static bool OnISR();
    static bool TurnSteps(StepperMotor* stepper, int number_of_steps);

    static StepperMotor* flap;
    static StepperMotor* pump;
    
private:
    static void StepMotor(StepperMotor* stepper, int this_step);
    static void Step(StepperMotor* stepper);


    static int HC595Registers[8];
    static void WriteHC595Registers(bool clear = false);
    
};

#endif

