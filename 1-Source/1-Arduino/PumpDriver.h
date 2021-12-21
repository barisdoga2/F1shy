#ifndef PumpDriver_H
#define PumpDriver_H

#define PUMP_SPEED 255
#define MILLILITERS_PER_MIN 100
#define MILLILITERS_PER_MS (double)100 / ((double)60 * (double)1000)

#include "Arduino.h"
#include "Types.h"
#include "PinLayout.h"



class PumpDriver
{

public:
    static void Init();
    static void Update();
    static void PumpMilliliters(int milliliters);

private: 
    static PumpMotor* pump;
    static const double milliliters_per_ms;
    
};


#endif