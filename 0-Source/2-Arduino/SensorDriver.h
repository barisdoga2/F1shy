#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#define TEMP_PIN A6
#define CURRENT_PIN A7
#define PRESSURE_PIN A3
#define LEAK_PIN A2
#define VOLTAGE_PIN A1

#ifdef DEBUG_SENSOR
  #define DPRINT(x) Serial.print(x)
  #define DPRINTLN(x) Serial.println(x)
#else
  #define DPRINT(x)
  #define DPRINTLN(x)
#endif

int mVperAmp = 66;
int RawValue = 0;
int ACSoffset = 2500;

float sensorData[5];  // temp current pressure leak voltage

int InitSensorDriver()
{
    pinMode(TEMP_PIN, INPUT);
    pinMode(CURRENT_PIN, INPUT);
    pinMode(PRESSURE_PIN, INPUT);
    pinMode(LEAK_PIN, INPUT);
    pinMode(VOLTAGE_PIN, INPUT);

    return 0;
}

int UpdateSensorDriver()
{

    // Calculate Temperature
    sensorData[0] = ((analogRead(TEMP_PIN)/1023.0)*5000.0) / 10.0;
    
    // Calculate Current
    sensorData[1] = ((((analogRead(CURRENT_PIN) / 1024.0) * 5000.0) - ACSoffset) / mVperAmp);

    // Calculate Water Pressure
    sensorData[2] = analogRead(PRESSURE_PIN);

    // Calculate Water Leak
    sensorData[3] = analogRead(LEAK_PIN);

    // Calculate Voltage
    sensorData[4] = analogRead(VOLTAGE_PIN);

    DPRINT("Temperature: ");
    DPRINT(/*sensorData[0]*/0);
    DPRINT(" \t Current: ");
    DPRINT(sensorData[1]);
    DPRINT(" \t Water Pressure: ");
    DPRINT(/*sensorData[2]*/0);
    DPRINT(" \t Water Leak: ");
    DPRINT(sensorData[3]);
    DPRINT(" \t Voltage: ");
    DPRINTLN(sensorData[4]);
    return 0;
}

#endif
