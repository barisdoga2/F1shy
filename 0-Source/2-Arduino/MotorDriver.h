#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Stepper.h>

#define ENGINE_PIN1 3
#define ENGINE_PIN2 5
#define PUMP_PIN1 9
#define PUMP_PIN2 10
#define LED_MATRIX_PIN 0

#ifdef DEBUG_MOTOR
  #define DPRINT(x) Serial.print(x)
  #define DPRINTLN(x) Serial.println(x)
#else
  #define DPRINT(x)
  #define DPRINTLN(x)
#endif

typedef struct Motor{
  int pin1;
  int pin2;
  int speed;
  unsigned long startTime;
  unsigned long stopTime;
  int isRunning;
} Motor;
#define ENGINE_MOTOR 0
#define PUMP_MOTOR 1
Motor engineMotor = {9, 10, 0, 0, 0};
Motor pumpMotor = {3, 5, 0, 0, 0};

const int stepsPerRevolution = 1024;
Stepper leftStepper = Stepper(stepsPerRevolution, 4, 6, 7, 8);
Stepper rightStepper = Stepper(stepsPerRevolution, 11, 12, 13, A0);

int InitMotorDriver()
{
    pinMode(ENGINE_PIN1, OUTPUT);
    pinMode(ENGINE_PIN2, OUTPUT);
    pinMode(PUMP_PIN1, OUTPUT);
    pinMode(PUMP_PIN2, OUTPUT);

    leftStepper.setSpeed(20);
    leftStepper.setSpeed(20);

    return 0;
}

void SetSpeed(int MOTOR, int tspeed){
  Motor* m = 0x0;
  if(MOTOR == ENGINE_MOTOR){
    m = &engineMotor;
  }else if(MOTOR == PUMP_MOTOR){
    m = &pumpMotor;
  }
  
  if(m == 0x0){
    DPRINT("Motor ");
    DPRINT(MOTOR);
    DPRINTLN(" not found.");
    return;
  }
  
  if(tspeed == 0 && m->isRunning == 1){
    m->isRunning = 0;
    m->stopTime = millis();
    DPRINT("Motor ");
    DPRINT(MOTOR);
    DPRINTLN(" set stopped.");
  }else if(tspeed != 0 && m->isRunning == 0){
    m->isRunning = 1;
    m->startTime = millis();
    DPRINT("Motor ");
    DPRINT(MOTOR);
    DPRINTLN(" set running.");
  }
  
  m->speed = tspeed;
  DPRINT("Motor ");
  DPRINT(MOTOR);
  DPRINT(" speed is set to ");
  DPRINTLN(tspeed);

  if(m->speed < 0){
    analogWrite(m->pin1, -m->speed);
    analogWrite(m->pin2, 0);
  }else{
    analogWrite(m->pin1, 0);
    analogWrite(m->pin2, m->speed);
  }
}

int UpdateMotorDriver()
{

    leftStepper.step(stepsPerRevolution);
    delay(500);

    leftStepper.step(-stepsPerRevolution);
    delay(500);

    rightStepper.step(stepsPerRevolution);
    delay(500);

    rightStepper.step(-stepsPerRevolution);
    delay(500);

    return 0;
}

#endif
