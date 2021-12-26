#ifndef PINLAYOUT
#define PINLAYOUT

#include "Arduino.h"
#include "Types.h"

#define HC595_CLOCK 10
#define HC595_DATA  8
#define HC595_LATCH 13
#define VOLTAGE_SENS A1
#define PRESSURE_SENS A2
#define LEAK_SENS A6
#define DHT11_SENS A3
#define ENGINE_EN 3
#define ENGINE_IN1 2
#define ENGINE_IN2 7
#define PUMP_EN 9
#define PUMP_IN1 12
#define PUMP_IN2 4
#define LED 11
#define PUMP_ACT_R_SENS A0
#define PUMP_ACT_H_SENS A7
#define NUMBER_OF_PINS_USED 16

// Virtual 74HC595 Register Indexes
#define STEPPER_1_1 7
#define STEPPER_1_2 5
#define STEPPER_1_3 6
#define STEPPER_1_4 0
#define STEPPER_2_1 1
#define STEPPER_2_2 3
#define STEPPER_2_3 2
#define STEPPER_2_4 4



class PinLayout {

public:
    static void Init();
    
private:
    static Pin pinMap[NUMBER_OF_PINS_USED];

};

#endif

