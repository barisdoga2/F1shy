#ifndef SENSORDRIVER_H
#define SENSORDRIVER_H

#include <wiringSerial.h>

typedef struct {
    long long timestamp;
    int errorNo;
    float leakage;
    float internalTemperature;
    float motorTemperature;
    float pressure;
    float voltage;
    float current;
    float accX;
    float accY;
    float accZ;
    float gyrX;
    float gyrY;
    float gyrZ;
    float magX;
    float magY;
    float magZ;
    int gpsDummy;
} SerialData;

void InitSensorDriver();
void UpdateSensorDriver();

SerialData GetSerialData();

#endif