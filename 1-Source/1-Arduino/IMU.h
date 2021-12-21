#ifndef IMU_H
#define IMU_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#ifdef DEBUG_IMU
  #define DPRINT(x) Serial.print(x)
  #define DPRINTLN(x) Serial.println(x)
#else
  #define DPRINT(x)
  #define DPRINTLN(x)
#endif

#define INTERRUPT_PIN 2

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
    pinMode(INTERRUPT_PIN, INPUT);

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

        digitalPinToInterrupt(INTERRUPT_PIN);
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;

        packetSize = mpu.dmpGetFIFOPacketSize();
    }else{
      DPRINT("IMU Initialization failed ");
      DPRINTLN(devStatus);
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
            DPRINT(F("quat "));
            DPRINT(quat.x);
            DPRINT(F(" "));
            DPRINT(quat.y);
            DPRINT(F(" "));
            DPRINT(quat.z);
            DPRINT(F(" "));
            DPRINTLN(quat.w);
            return 0;
        }
    }

    return 1;
}

#endif