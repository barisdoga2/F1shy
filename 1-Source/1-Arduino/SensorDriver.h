#ifndef SENSORDRIVER_H
#define SENSORDRIVER_H

#include "Arduino.h"
#include "PinLayout.h"

#define SENSOR_INTERVAL_MS 1000
#define REF_5V 5.155
#define MV_PER_COUNT (REF_5V / 1024) * 1000
#define VOLTAGE_DIVIDER_MULTIPLIER 14.7 / 4.7



class SensorDriver{

public:
    static void Init();
    static void Update();

    static float GetTemperature() {return temperature;}
    static bool GetLeakage() {return leakage;}
    static float GetVoltage() {return voltage;}
    static float GetWaterPressure() {return waterPressure;}

private:
    static void ReadTemperature();
    static void ReadLeakage();
    static void ReadVoltage();
    static void ReadWaterPressure();

    static long lastCheck;
    
    static float temperature;
    static bool leakage;
    static float voltage;
    static float waterPressure;

};

#endif