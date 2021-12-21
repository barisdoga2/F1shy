#include "SensorDriver.h"

long SensorDriver::lastCheck = 0;

float SensorDriver::temperature = 0;
bool SensorDriver::leakage = 0;
float SensorDriver::voltage = 0;
float SensorDriver::waterPressure = 0;

void SensorDriver::Init()
{
    /* Nothing to do */
}

void SensorDriver::Update()
{
    long now = millis();
    if(now - lastCheck >= SENSOR_INTERVAL_MS)
    {
        lastCheck = now;

        ReadTemperature();
        ReadLeakage();
        ReadVoltage();
        ReadWaterPressure();
    }
    
}

void SensorDriver::ReadTemperature()
{
    float mv = (analogRead(TEMPERATURE_SENS)/1024.0)*5000.0;
    temperature = mv/10;
}

void SensorDriver::ReadLeakage()
{
    float val = analogRead(LEAK_SENS);
    leakage = val > 100 ? 1 : 0;
}

void SensorDriver::ReadVoltage()
{
    float val = analogRead(VOLTAGE_SENS);
    voltage = val * MV_PER_COUNT * VOLTAGE_DIVIDER_MULTIPLIER;
}

void SensorDriver::ReadWaterPressure()
{
    int val = analogRead(PRESSURE_SENS);
    waterPressure = val;
}