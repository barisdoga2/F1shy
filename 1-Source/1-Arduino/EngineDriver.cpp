#include "Arduino.h"
#include "EngineDriver.h"

EngineMotor* EngineDriver::engine;

void EngineDriver::Init()
{
    engine = new EngineMotor{ENGINE_IN1, ENGINE_IN2, ENGINE_EN, ACCELERATION};
}

void EngineDriver::Update()
{
    Accelerate();

    if(engine->current_power < 0)
    {
        digitalWrite(engine->motor_pin_1, LOW);
        digitalWrite(engine->motor_pin_2, HIGH);
    }else if(engine->current_power > 0)
    {  
        digitalWrite(engine->motor_pin_1, HIGH);
        digitalWrite(engine->motor_pin_2, LOW);
    }
    analogWrite(engine->motor_pin_enable, abs((int)(engine->current_power)));
}

void EngineDriver::SetAcceleration(int acceleration)
{
    engine->acceleration = acceleration;
}

void EngineDriver::SetTargetPower(int target_power)
{
    engine->target_power = target_power;
    engine->lastAcceleration = millis();
}

void EngineDriver::Accelerate()
{
    if(engine->target_power != engine->current_power)
    {
        long now = millis();
        long diff_ms = now - engine->lastAcceleration;
        engine->lastAcceleration = now;
        
        float acc_amount = engine->acceleration * diff_ms;
        
        if(engine->target_power < engine->current_power)
        {
            acc_amount *= -1;
            engine->current_power += acc_amount;
            
            if(engine->target_power > engine->current_power)
                engine->current_power = engine->target_power;
        }else if(engine->target_power > engine->current_power){
            acc_amount *= 1;
            engine->current_power += acc_amount;
            
            if(engine->target_power < engine->current_power)
                engine->current_power = engine->target_power;
        }
    }
}