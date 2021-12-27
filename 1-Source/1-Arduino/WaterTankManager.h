#ifndef WATERTANKMANAGER
#define WATERTANKMANAGER

#include "Arduino.h"
#include "Types.h"
#include "StepperDriver.h"
#include "PumpDriver.h"

#define OPERATION_PENDING    0
#define OPERATION_ACTUATING  1
#define OPERATION_PUMPING    2
#define OPERATION_DONE       3


class WaterTankManager
{

public:
    static void Init();
    static void Update();

    static bool FillTank(int tank, int milliliters); // Support up to 5ml because ISRHandler
    static bool KillCurrentOperation(int* pumpedWater);

private:
    static int operationState;
    static PumpActuatorTarget targetTank;
    static int millilitersToPump;


};


#endif