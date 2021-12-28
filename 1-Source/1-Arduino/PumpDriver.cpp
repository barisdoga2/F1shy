#include "Arduino.h"
#include "PumpDriver.h"

const double PumpDriver::milliliters_per_ms = (double)MILLILITERS_PER_MIN / ((double)60 * (double)1000);
bool PumpDriver::isPumping = false;
PumpMotor* PumpDriver::pump;

void PumpDriver::Init()
{
    pump = new PumpMotor{PUMP_IN1, PUMP_IN2, PUMP_EN};
}

bool PumpDriver::OnISR()
{
    StopPump();
    return true;
}

int PumpDriver::MicrosToMilliliters(long micro_secs)
{
    return 10; // [TODO] too lazy atm to write this.
} 

void PumpDriver::StopPump()
{
    analogWrite(PUMP_EN, 0);
    isPumping = 0;
}

bool PumpDriver::PumpMilliliters(int milliliters) // ISR Support Up to 5ml
{
    bool retVal = false;

    if(isPumping == 0 && milliliters != 0 && milliliters >= -5 && milliliters <=5)
    {
        if(milliliters < 0)
        {
            digitalWrite(pump->motor_pin_1, LOW);
            digitalWrite(pump->motor_pin_2, HIGH);
        }else if(milliliters > 0)
        {
            digitalWrite(pump->motor_pin_1, HIGH);
            digitalWrite(pump->motor_pin_2, LOW);
        }

        double needed_ms = (long)abs(milliliters) / milliliters_per_ms;

        retVal = ISRHandler::EnableInterrupt(needed_ms, OnISR);
        if(retVal)
        {
            isPumping = true;
            analogWrite(PUMP_EN, PUMP_SPEED);
        }

    }
    return retVal;
}