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
    bool retVal = false;
    if(c == '+')
    {
      retVal = StepperDriver::TurnSteps(StepperDriver::pump, 2048);
    }else if(c == '-')
    {      
      retVal = StepperDriver::TurnSteps(StepperDriver::flap, -2048);
    }else if(c == 'a')
    {      
      retVal = 1;
      EngineDriver::SetTargetPower(255);
    }else if(c == 'b')
    {      
      retVal = 1;
      EngineDriver::SetTargetPower(-255);
    }else if(c == 'c')
    {      
      retVal = 1;
      EngineDriver::SetTargetPower(0);
    }else if(c == 'd')
    {      
      retVal = PumpDriver::PumpMilliliters(2);
    }else if(c == 'f')
    {      
      retVal = PumpDriver::PumpMilliliters(-2);
    }
    
    if(c != '\n')
    {
      Serial.print("Command Returned: ");
      Serial.println(retVal);
    }
  }
  //UpdateIMU();
}
