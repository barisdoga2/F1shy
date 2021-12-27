#ifndef PumpDriver_H
#define PumpDriver_H

#define PUMP_SPEED 255
#define MILLILITERS_PER_MIN 84

#include "Arduino.h"
#include "Types.h"
#include "PinLayout.h"
#include "ISRHandler.h"




class PumpDriver
{
    friend class WaterTankManager;

public:
    static void Init();
    static bool PumpMilliliters(int milliliters); // ISR Support Up to 5ml
    static bool IsPumping() {return isPumping;}
    static int GetMillilitersMicros(long micro_secs);
    static void StopPump();


private: 
    static bool OnISR();

    static PumpMotor* pump;
    static bool isPumping;
    static const double milliliters_per_ms;
    
};


#endif