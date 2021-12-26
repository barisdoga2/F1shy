  #define DEBUG_IMU

#include "PinLayout.h"
#include "StepperDriver.h"
#include "EngineDriver.h"
#include "PumpDriver.h"
#include "SensorDriver.h"
#include "ISRHandler.h"
//#include "IMU.h"

int led = 0;

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

char serialCommandBuff[100];
int commandLen = 0;
bool processSerialCommand()
{

  bool retVal = false;
  if(strncmp(&serialCommandBuff[0], "pa", 2) == 0)
  {    
    if(strncmp(&serialCommandBuff[2], "1", 1) == 0)
      retVal = StepperDriver::TurnPumpActuator(REAR_WATER_TANK);
    else if(strncmp(&serialCommandBuff[2], "-1", 2) == 0)
      retVal = StepperDriver::TurnPumpActuator(HEAD_WATER_TANK);
  }
  else if(strncmp(&serialCommandBuff[0], "f", 1) == 0)
  {
    retVal = StepperDriver::TurnFlap(atoi(&serialCommandBuff[1]));
  }else if(strncmp(&serialCommandBuff[0], "e", 1) == 0)
  {
    retVal = EngineDriver::SetTargetPower(atoi(&serialCommandBuff[1]));
  }
  else if(strncmp(&serialCommandBuff[0], "p", 1) == 0)
  {
    retVal = PumpDriver::PumpMilliliters(atoi(&serialCommandBuff[1]));
  }else if(strncmp(&serialCommandBuff[0], "s", 1) == 0)
  {
    SensorDriver::PrintAll();
    retVal = true;
  }else if(strncmp(&serialCommandBuff[0], "l", 1) == 0)
  {
    led = atoi(&serialCommandBuff[1]);
    if(led < 0 || led > 255)  
    {
      retVal = false;
    }else{
      retVal = true;
      analogWrite(11, led);
    }
  }

  return retVal;
}

void loop()
{
  EngineDriver::Update();
  SensorDriver::Update();

  while(Serial.available() > 0)
  {
    char c = (char)Serial.read();
    serialCommandBuff[commandLen++] = c;
    if(serialCommandBuff[commandLen - 1] == '\n')
    {
      serialCommandBuff[commandLen - 1] = '\0';
      bool retVal = processSerialCommand();
      Serial.print("Command: '");
      Serial.print(serialCommandBuff);
      Serial.print("' Return: '");
      Serial.print(retVal);
      Serial.println("'");
      commandLen = 0;
    }
  }
  //UpdateIMU();
}
