#include "SensorDriver.h"

#define ARDUINO_DEVICE "/dev/ttyAMA0"
#define ARDUINO_BAUD_RATE 115200

int deviceControl;
char ioBuffer[1000];

SerialData serialData = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void ProcessSerialData() // Timestamp ErrorNo Leakage InternalTemp MotorTemp Pressure Voltage Current AccX, AccY, AccZ, GyrX, GyrY, GyrZ, MagX, MagY, MagZ, GPSDummy]
{
    // Split data by ' '
    char* parts[18];
    int partcount = 0;
    parts[partcount++] = ioBuffer;
    char* ptr = ioBuffer;
    while(*ptr) {
        if(*ptr == ' ') {
            *ptr = 0;
            parts[partcount++] = ptr + 1;
        }
        ptr++;
    }

    serialData.timestamp = parts[0];
    serialData.errorNo = parts[1];
    serialData.leakage = atof(parts[2]);
    serialData.internalTemperature = atof(parts[3]);
    serialData.motorTemperature = atof(parts[4]);
    serialData.pressure = atof(parts[5]);
    serialData.voltage = atof(parts[6]);
    serialData.current = atof(parts[7]);
    serialData.accX = atof(parts[8]);
    serialData.accY = atof(parts[9]);
    serialData.accZ = atof(parts[10]);
    serialData.gyrX = atof(parts[11]);
    serialData.gyrY = atof(parts[12]);
    serialData.gyrZ = atof(parts[13]);
    serialData.magX = atof(parts[14]);
    serialData.magY = atof(parts[15]);
    serialData.magZ = atof(parts[16]);
    serialData.gpsDummy = atof(parts[17]);    

    printf("Serial Data Received and Processed: %s\n", ioBuffer);
}

void InitSensorDriver()
{
    deviceControl = serialOpen(ARDUINO_DEVICE, ARDUINO_BAUD_RATE);
    if (deviceControl < 0)
        printf("Unable to open arduino device.");
}

void UpdateSensorDriver()
{
    static int n = 0;
    while (serialDataAvail(deviceControl)) {
        int ch = serialGetchar(deviceControl);
        ioBuffer[n++] = (char)ch;
        if (ch == 10)
        {
            ioBuffer[n] = '\0';
            ProcessSerialData();
            n = 0;
        }
    }
}

SerialData GetSerialData(){
    return serialData;
}