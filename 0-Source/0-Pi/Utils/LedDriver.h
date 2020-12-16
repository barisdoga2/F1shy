#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#define RED_PIN 0
#define MATRIX_PIN 0

unsigned long red_start = 0, red_last = 0, red_timeout = 0, red_onTime, red_offTime;
int is_red_blinking = 0;
int red_led = 0;
int matrix_led = 0;

int InitLedDriver()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(MATRIX_PIN, OUTPUT);

    return 0;
}

void Enable(int LED_PIN, int value)
{
    digitalWrite(LED_PIN, value);

    if (LED_PIN == RED_PIN)
        red_led = value;
    else if (LED_PIN == MATRIX_PIN)
        matrix_led = value;
}

void BlinkRed(unsigned long onTime, unsigned long offTime, unsigned long timeout)
{
    if (is_red_blinking == 0)
    {
        red_onTime = onTime;
        red_offTime = offTime;
        red_timeout = timeout;
        red_start = millis();
        red_last = millis();
        Enable(RED_PIN, 1);
        is_red_blinking = 1;
    }
}

int UpdateLedDriver()
{
    unsigned long current = millis();
    if (is_red_blinking > 0)
    {
        if (is_red_blinking == 1)
        {
            if (red_last + red_onTime <= current)
            {
                red_last = current;
                Enable(RED_PIN, 0);
                is_red_blinking = 2;
            }
        }
        else if (is_red_blinking == 2)
        {
            if (red_last + red_offTime <= current)
            {
                red_last = current;
                Enable(RED_PIN, 1);
                is_red_blinking = 1;
            }
        }
    }

    if(red_start + red_timeout <= current && is_red_blinking != 0){
#ifdef DEBUG_LED
        Serial.println(F("Red led timed out."));
#endif
        Enable(RED_PIN, 0);
        is_red_blinking = 0;
    }

    return 0;
}

#endif
