#include "PinLayout.h"

Pin PinLayout::pinMap[NUMBER_OF_PINS_USED] = {
    {ENGINE_IN1,        OUTPUT,   LOW},
    {ENGINE_EN,         OUTPUT,   LOW},
    {PUMP_IN2,          OUTPUT,   LOW},
    {PUMP_IN1,          OUTPUT,   LOW},
    {PUMP_EN,           OUTPUT,   LOW},
    {ENGINE_IN2,        OUTPUT,   LOW},
    {HC595_DATA,        OUTPUT,   LOW},
    {HC595_LATCH,       OUTPUT,   LOW},
    {HC595_CLOCK,       OUTPUT,   LOW},
    {LED,               OUTPUT,   LOW},
    {VOLTAGE_SENS,      INPUT,    LOW},
    {PRESSURE_SENS,     INPUT,    LOW},
    {LEAK_SENS,         INPUT,    LOW},
    {DHT11_SENS,        INPUT,    LOW},
    {PUMP_ACT_R_SENS,   INPUT,    LOW},
    {PUMP_ACT_H_SENS,   INPUT,    LOW},
};

void PinLayout::Init()
{
    for(int i = 0 ; i < NUMBER_OF_PINS_USED ; i++)
    {
        Pin pin = pinMap[i];
        pinMode(pin.pinNumber, pin.mode);

        if(pin.mode == OUTPUT)
            digitalWrite(pin.pinNumber, pin.state);
    }
}