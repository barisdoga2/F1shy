#include "Arduino.h"
#include "PumpDriver.h"

const double PumpDriver::milliliters_per_ms = (double)MILLILITERS_PER_MIN / ((double)60 * (double)1000);
PumpMotor* PumpDriver::pump;

void PumpDriver::Init()
{
    pump = new PumpMotor{PUMP_IN1, PUMP_IN2, PUMP_EN};
}

void PumpDriver::Update()
{
    if(pump->isPumping == 1)
    {
        unsigned long now = micros();
        
        if(pump->endTime <= now)
        {
            analogWrite(pump->motor_pin_enable, LOW);
            pump->isPumping = 0;
            Serial.print("Pump Error in micros: ");
            Serial.print(now - pump->endTime);
            Serial.print(", Pump Error in ml: ");
            Serial.println((now - pump->endTime) * milliliters_per_ms, 6);
        }else if(pump->endTime - now < 100)
        {
            delayMicroseconds(pump->endTime - micros());
            PumpDriver::Update();
        }
    }
}
void PumpDriver::PumpMilliliters(int milliliters)
{
    if(pump->isPumping == 0 && milliliters != 0)
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

        unsigned long needed_ms = (long)abs(milliliters) / (milliliters_per_ms / 1000);
        
        pump->endTime = micros() + needed_ms;
        analogWrite(pump->motor_pin_enable, PUMP_SPEED);
        pump->isPumping = 1;
    }
}