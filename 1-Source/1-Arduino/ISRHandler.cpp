#include "ISRHandler.h"

OnISR_cbk ISRHandler::OnISR = 0x0;



ISR(TIMER1_COMPA_vect)
{
    bool ret = ISRHandler::OnISR();
    if(ret == true)
    {
        ISRHandler::DisableInterrupt();
        ISRHandler::OnISR = 0x0;
    }
}

bool ISRHandler::EnableInterrupt(double milliseconds_period, OnISR_cbk OnISR_func_ptr) //[TODO] Very first interrupt called and finished immediately after sei(); call. Weird bug.
{
    if(OnISR != 0x0)
    {
        return false;
    }

    OnISR = OnISR_func_ptr;
    unsigned int cmr = (int)(((float)16000000 / (((float)1 / (milliseconds_period / 1000)) * (float)1024)) - (float)1);

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
    sei();

    return true;
}

void ISRHandler::DisableInterrupt()
{
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 0;
    TIMSK1 = 0;
    TIMSK1 |= (0 << OCIE1A);
    sei();
}