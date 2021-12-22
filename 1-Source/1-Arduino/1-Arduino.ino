  #define DEBUG_IMU

#include "PinLayout.h"
#include "StepperDriver.h"
#include "EngineDriver.h"
#include "PumpDriver.h"
#include "SensorDriver.h"
//#include "IMU.h"


void setup()
{
  Serial.begin(9600);
  Serial.println("Welcome!");

  PinLayout::Init();
  StepperDriver::Init();
  EngineDriver::Init();
  PumpDriver::Init();
  SensorDriver::Init();
  //InitIMU();
}

void loop()
{
  EngineDriver::Update();
  PumpDriver::Update();
  SensorDriver::Update();

  if(Serial.available() > 0){
    char c = (char)Serial.read();
    if(c == '+')
    {
      StepperDriver::TurnSteps(StepperDriver::pump, 2048);
      StepperDriver::TurnSteps(StepperDriver::flap, 2048);
    }else if(c == '-')
    {      
      StepperDriver::TurnSteps(StepperDriver::pump, -2048);
      StepperDriver::TurnSteps(StepperDriver::flap, -2048);
    }else if(c == 'a')
    {      
      EngineDriver::SetTargetPower(255);
    }else if(c == 'b')
    {      
      EngineDriver::SetTargetPower(-255);
    }else if(c == 'c')
    {      
      EngineDriver::SetTargetPower(0);
    }else if(c == 'd')
    {      
      PumpDriver::PumpMilliliters(5);
    }else if(c == 'f')
    {      
      PumpDriver::PumpMilliliters(-5);
    }
  }
  //UpdateIMU();
}
