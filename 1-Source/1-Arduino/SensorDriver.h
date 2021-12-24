#ifndef SENSORDRIVER_H
#define SENSORDRIVER_H

#include "Arduino.h"
#include "PinLayout.h"
#include <dht11.h>

#define SENSOR_INTERVAL_MS 1000
#define REF_5V 5.155
#define MV_PER_COUNT (REF_5V / 1024) * 1000
#define VOLTAGE_DIVIDER_MULTIPLIER 14.7 / 4.7



class SensorDriver{

public:
    static void Init();
    static void Update();
    static void PrintAll();

    static float GetTemperature() {return temperature;}
    static float GetHumidity() {return humidity;}
    static bool GetLeakage() {return leakage;}
    static float GetVoltage() {return voltage;}
    static float GetWaterPressure() {return waterPressure;}

private:
    static void ReadDHT11();
    static void ReadLeakage();
    static void ReadVoltage();
    static void ReadWaterPressure();

    static long lastCheck;
    
    static dht11 dht_sensor;
    static float humidity;
    static float temperature;
    static bool leakage;
    static float voltage;
    static float waterPressure;

};

#endif