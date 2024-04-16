/*
 * File:   timer.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:40 PM
 */
#include "xc.h"
#include "timer.h"




void tmr_setup_period(int timer, int ms)
{
    long clock = 72000000; // 72 MHz

    int num = get_prescaler_number(ms, clock);

    if (timer == TIMER1)
    {
        T1CONbits.TON = 0; // disable Timer
        IFS0bits.T1IF = 0; // set flag to 0
        TMR1 = 0;          // reset timer counter

        // set to compare with TMRx
        PR1 = get_prescaler_value(ms); 
        // Set the Prescaler to correct value
        T1CONbits.TCKPS = num; 
        // Start the timer
        T1CONbits.TON = 1; 
    }
    else if (timer == TIMER2)
    {
        T2CONbits.TON = 0;
        IFS0bits.T2IF = 0;
        TMR2 = 0;
        PR2 = get_prescaler_value(ms);

        if (num == 3)
        {
            T2CONbits.TCKPS = 3;
        }
        else if (num == 2)
        {
            T2CONbits.TCKPS = 2;
        }
        else if (num == 1)
        {
            T2CONbits.TCKPS = 1;
        }
        else if (num == 0)
        {
            T2CONbits.TCKPS = 0;
        }
        T2CONbits.TON = 1;
    }
}

// use the timer flag to wait until it has expired
int tmr_wait_period(int timer)
{
    int ret_val = 0;
    if (timer == TIMER1)
    {
        if (IFS0bits.T1IF == 1)
        {
            // Flag is raised, the period was reached already
            ret_val = 1;
        }
        while (IFS0bits.T1IF == 0) // Wait until the period
        {
        }
        IFS0bits.T1IF = 0; // reset the flag 
        return ret_val;
    }
    else if (timer == TIMER2)
    {
        if (IFS0bits.T2IF == 1)
        {
            ret_val = 1;
        }
        T2CONbits.TON = 1;
        while (IFS0bits.T2IF == 0)
        {
        }
        IFS0bits.T2IF = 0;
        return ret_val;
    }
}

void tmr_wait_ms(int timer, int ms)
{
    tmr_setup_period(timer, ms);
    tmr_wait_period(timer);
}


int get_prescaler_value(int ms, long clock)
{
    int prescaler;
    int num = get_prescaler_number(ms, clock);
    switch(num)
    {
        case 0:
            prescaler = 1;
        case 1:
            prescaler = 8;
        case 2:
            prescaler = 64;
        case 3:
            prescaler = 256;
        }
    int result = (clock / prescaler) * ((float)ms / 1000);
    return result;
}


int get_prescaler_number(int ms, long clock)
{
    int num = 0;
    int prescaler = 1;

    if (((float)ms) * clock > 65535)
    {
        num = 1;
        prescaler = 8;
        if (((float)ms) * (clock / prescaler) > 65535)
        {
            num = 2;
            prescaler = 64;
            if (((float)ms) * (clock / prescaler) > 65535)
            {
                num = 3;
                prescaler = 256;
            }
        }
    }
    return num;
}