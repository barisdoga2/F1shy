#include "ISRHandler.h"

OnISR_cbk ISRHandler::OnISR = 0x0;
long ISRHandler::isr_start = 0;



ISR(TIMER1_COMPA_vect)
{
    bool retVal = ISRHandler::OnISR();
    if(retVal == true)
    {
        ISRHandler::DisableInterrupt();
    }
}

long ISRHandler::KillInterrupt(OnISR_cbk OnISR_func_ptr)
{
    long retVal = 1;

    if(OnISR == 0x0)
    {
        retVal = 0;
    }

    if(OnISR != OnISR_func_ptr)
    {
        retVal = 0;
    }

    if(retVal)
    {
        retVal = DisableInterrupt();
    }
    
    return retVal;
}

bool ISRHandler::EnableInterrupt(double milliseconds_period, OnISR_cbk OnISR_func_ptr)
{
    bool retVal = true;

    if(OnISR != 0x0)
    {
        retVal = false;
    }

    if(retVal)
    {
        OnISR = OnISR_func_ptr;
        unsigned int cmr = (int)(((double)16000000 / (((double)1 / (milliseconds_period / 1000)) * (double)1024)) - (double)1); // [TODO] To work more precise, lower prescelar needed. Should align it according to period.

        cli();
        TCCR1A = 0;
        TCCR1B = 0;
        TCNT1  = 0;
        TIMSK1 = 0;
        OCR1A = cmr;
        TIFR1 = (1<<OCF1A);
        TCCR1B |= (1 << WGM12);
        TCCR1B |= (1 << CS12) | (1 << CS10);
        TIMSK1 |= (1 << OCIE1A);
        isr_start = micros();
        sei();
    }
    
    return retVal;
}

long ISRHandler::DisableInterrupt()
{
    cli();
    ISRHandler::OnISR = 0x0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 0;
    TIMSK1 = 0;
    TIMSK1 |= (0 << OCIE1A);
    sei();
    return micros() - isr_start; // [TODO] micros will overflow in 70mins, need a time manager.
}