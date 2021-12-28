   #define DEBUG_IMU

#include "PinLayout.h"
#include "StepperDriver.h"
#include "EngineDriver.h"
#include "SensorDriver.h"
#include "ISRHandler.h"
#include "WaterTankManager.h"
#include "IMU.h"

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
  WaterTankManager::Init();
  InitIMU();
}

char serialCommandBuff[100];
int commandLen = 0;
bool processSerialCommand()
{

  bool retVal = false;
  if(strncmp(&serialCommandBuff[0], "f", 1) == 0)
  {
    retVal = StepperDriver::TurnFlap(atoi(&serialCommandBuff[1]));
  }else if(strncmp(&serialCommandBuff[0], "c", 1) == 0)
  {
    int waterPumped = 0;
    retVal = WaterTankManager::KillCurrentOperation(&waterPumped);
    Serial.print("Water flow canceled. Pumped Water: ");
    Serial.print(waterPumped);
    Serial.println("ml.");
  }else if(strncmp(&serialCommandBuff[0], "e", 1) == 0)
  {
    retVal = EngineDriver::SetTargetPower(atoi(&serialCommandBuff[1]));
  }
  else if(strncmp(&serialCommandBuff[0], "pr", 2) == 0)
  {
    retVal = WaterTankManager::FillTank(REAR_WATER_TANK, atoi(&serialCommandBuff[2]));
  }else if(strncmp(&serialCommandBuff[0], "ph", 2) == 0)
  {
    retVal = WaterTankManager::FillTank(HEAD_WATER_TANK, atoi(&serialCommandBuff[2]));
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
  WaterTankManager::Update();

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
