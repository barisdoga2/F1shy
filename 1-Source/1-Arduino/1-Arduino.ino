  #define DEBUG_IMU

#include "PinLayout.h"
#include "StepperDriver.h"
#include "EngineDriver.h"
#include "PumpDriver.h"
#include "SensorDriver.h"
#include "ISRHandler.h"
//#include "IMU.h"


void setup()
{
  Serial.begin(9600);
  Serial.println("Welcome!");

  ISRHandler::EnableInterrupt(0, 0x0); // Because of weird interrupt bug
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
  SensorDriver::Update();

  if(Serial.available() > 0){
    char c = (char)Serial.read();
    if(c == '+')
    {
      StepperDriver::TurnSteps(StepperDriver::pump, 2048);
    }else if(c == '-')
    {      
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
      PumpDriver::PumpMilliliters(2);
    }else if(c == 'f')
    {      
      PumpDriver::PumpMilliliters(-2);
    }
  }
  //UpdateIMU();
}
