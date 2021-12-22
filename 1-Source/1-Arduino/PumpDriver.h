#ifndef PumpDriver_H
#define PumpDriver_H

#define PUMP_SPEED 255
#define MILLILITERS_PER_MIN 84

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