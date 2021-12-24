#ifndef ENGINEDRIVER_H
#define ENGINEDRIVER_H

#include "Arduino.h"
#include "Types.h"
#include "PinLayout.h"

#define ACCELERATION 0.2


class EngineDriver
{

public:
    static void Init();
    static void Update();

    static void SetAcceleration(int acceleration);
    static bool SetTargetPower(int target_power);

private:
    static void Accelerate();

    static EngineMotor* engine;
    
};

#endif