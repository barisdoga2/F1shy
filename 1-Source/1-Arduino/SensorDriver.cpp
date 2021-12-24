#include "SensorDriver.h"

long SensorDriver::lastCheck = 0;
dht11 SensorDriver::dht_sensor;
float SensorDriver::temperature = 0;
float SensorDriver::humidity = 0;
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

        ReadDHT11();
        ReadLeakage();
        ReadVoltage();
        ReadWaterPressure();
    }
    
}

void SensorDriver::ReadDHT11()
{
    int chk = dht_sensor.read(A3);
 
    humidity = (float)dht_sensor.humidity;
    temperature = (float)dht_sensor.temperature;
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

void SensorDriver::PrintAll()
{
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(", Humidity: ");
    Serial.print(humidity);
    Serial.print(", Leakage: ");
    Serial.print(leakage);
    Serial.print(", Voltage: ");
    Serial.print(voltage);
    Serial.print(", WaterPressure: ");
    Serial.println(waterPressure);
}