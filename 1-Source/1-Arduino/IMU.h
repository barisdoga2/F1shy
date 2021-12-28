#ifndef IMU_H
#define IMU_H

#include "Arduino.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "Types.h"

MPU6050 mpu;

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion quat;

volatile bool mpuInterrupt = false;

void dmpDataReady()
{
    mpuInterrupt = true;
}

int InitIMU()
{
    Wire.begin();
    Wire.setClock(400000);

    mpu.initialize();
    devStatus = mpu.dmpInitialize();

    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    if (devStatus == 0)
    {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        mpu.setDMPEnabled(true);

        digitalPinToInterrupt(IMU_INT);
        attachInterrupt(digitalPinToInterrupt(IMU_INT), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        Serial.print("Int status");
        Serial.println(mpuIntStatus);
        dmpReady = true;

        packetSize = mpu.dmpGetFIFOPacketSize();
    }else{
      Serial.print("IMU Initialization failed ");
      Serial.println(devStatus);
    }

    return devStatus == 0 ? 0 : 1;
}

int UpdateIMU()
{
    if (dmpReady)
    {
        if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
        {
            mpu.dmpGetQuaternion(&quat, fifoBuffer);
            Serial.print(F("quat "));
            Serial.print(quat.x);
            Serial.print(F(" "));
            Serial.print(quat.y);
            Serial.print(F(" "));
            Serial.print(quat.z);
            Serial.print(F(" "));
            Serial.println(quat.w);
            return 0;
        }
    }

    return 1;
}

#endif