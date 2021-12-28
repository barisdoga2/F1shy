#include "WaterTankManager.h"

int WaterTankManager::operationState;
PumpActuatorTarget WaterTankManager::targetTank;
int WaterTankManager::millilitersToPump;



void WaterTankManager::Init()
{
    operationState = OPERATION_DONE;
    targetTank = WATER_TANK_NONE;
    millilitersToPump = 0;
}

void WaterTankManager::Update()
{
    switch (operationState)
    {
    case OPERATION_PENDING:
        if(StepperDriver::TurnPumpActuator(targetTank))
        {
            operationState = OPERATION_ACTUATING;
        }
        break;

    case OPERATION_ACTUATING:
        if(PumpDriver::PumpMilliliters(millilitersToPump))
        {
            operationState = OPERATION_PUMPING;
        }
        break;
    
    case OPERATION_PUMPING:
        if(!PumpDriver::IsPumping())
        {
            StepperDriver::UnlockStepper(StepperDriver::pumpActuator);
            operationState = OPERATION_DONE;
        }
        break;

    case OPERATION_DONE:
        break;
    default:
        break;
    }
}

bool WaterTankManager::KillCurrentOperation(int* pumpedWater) // Probably not safe to use :D
{
    bool retVal = true;
    *pumpedWater = 0;
    
    if(operationState == OPERATION_PENDING)
    {
        operationState = OPERATION_DONE;
    }
    else if(operationState == OPERATION_ACTUATING)
    {
        ISRHandler::KillInterrupt(StepperDriver::PumpActuatorOnISR);
        StepperDriver::UnlockStepper(StepperDriver::pumpActuator);
        operationState = OPERATION_DONE;
    }
    else if(operationState == OPERATION_PUMPING)
    {
        long runningus = ISRHandler::KillInterrupt(PumpDriver::OnISR);
        PumpDriver::StopPump();
        operationState = OPERATION_DONE;
        if(runningus != 0)
        {
            *pumpedWater = PumpDriver::MicrosToMilliliters(runningus);
        }
    }

    return retVal;
}

bool WaterTankManager::FillTank(int tank, int milliliters)
{
    bool retVal = true;

    if(tank != HEAD_WATER_TANK && tank != REAR_WATER_TANK)
    {
        retVal = false;
    }
    if(milliliters == 0 || !(milliliters >= -5 && milliliters <=5))
    {
        retVal = false;
    }
    if(operationState != OPERATION_DONE)
    {
        retVal = false;
    }

    if(retVal)
    {
        targetTank = tank;
        millilitersToPump = milliliters;
        operationState = OPERATION_PENDING;
    }

    return retVal;
}