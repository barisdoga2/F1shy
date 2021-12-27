#ifndef Stepper_h
#define Stepper_h

#include "Types.h"
#include "PinLayout.h"
#include "ISRHandler.h"

#define NUMBER_OF_STEPS_FOR_REVOLUTION 1024
#define STEP_DELAY_MS 3



class StepperDriver {
    friend class WaterTankManager;
    
public:
    static void Init();

    static bool TurnFlap(int number_of_steps);
    
private:
    static StepperMotor* flap;
    static StepperMotor* pumpActuator;

    static bool FlapOnISR();
    static bool PumpActuatorOnISR();
    static bool TurnPumpActuator(PumpActuatorTarget targetTank);

    static void UnlockStepper(StepperMotor* stepMotor);
    static void StepMotor(StepperMotor* stepper, int this_step);
    static void Step(StepperMotor* stepper);

    static int HC595Registers[8];
    static void WriteHC595Registers();
    
};

#endif

