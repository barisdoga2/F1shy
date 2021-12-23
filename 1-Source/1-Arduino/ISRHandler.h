#ifndef ISRHANDLER_H
#define ISRHANDLER_H

#include "Arduino.h"
#include "PumpDriver.h"
#include "StepperDriver.h"

extern "C" void TIMER1_COMPA_vect(void) __attribute__ ((signal));

typedef bool(*OnISR_cbk)(void);



class ISRHandler{

public:
    static bool EnableInterrupt(double milliseconds_period, OnISR_cbk OnISR_func_ptr);

private:
    static void DisableInterrupt();
    friend void TIMER1_COMPA_vect(void);
    static OnISR_cbk OnISR;
};


#endif