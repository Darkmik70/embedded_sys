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
    long clock = 72000000;      // 72 MHz
    int ps_mode = 0;            // Mode of prescaler 0, 1, 2, 3
    long new_clock;             // Prescaled clock

    /* check if we need to use prescaler */
    if (ms * clock > 65535) // Check if the number of steps is within thershold of 16bit
    {
        // too many steps, use prescaler
        new_clock = clock / 8; // 1:8 
        ps_mode = 1;
        if (ms * new_clock > 65535) // Check again
        {
            new_clock = clock / 16; // 1:16
            ps_mode = 2;
            if (ms * new_clock > 65535) //and again
            {
                new_clock = clock / 256 // 1:256
                ps_mode = 3;
            }
        }
    }
    /* Set the value for the timer period */
    long result = (new_clocks)*((float) ms / 1000); // value of the timer
    if (timer == TIMER1)
    {
        
        T1CONbits.TON = 0;
        IFS0bits.T1IF = 0;
        TMR1 = 0;               // reset the timer counter
        PR1 = result;           // set to compare the TMRx
        
        /* set the prescaler */
        if (ps_mode == 3)
        {
            T1CONbits.TCKPS = 3;
        }
        else if (ps_mode == 2)
        {
            T1CONbits.TCKPS = 2;
        }
        else if (ps_mode == 1)
        {
            T1CONbits.TCKPS = 1;
        }
        else if (num == 0)
        {
            T1CONbits.TCKPS = 0;
        }
        /* Start the timer*/
        T1CONbits.TON = 1; 
    }                      
    else if (timer == TIMER2)
    {
        T2CONbits.TON = 0;
        IFS0bits.T2IF = 0;
        TMR2 = 0;
        PR2 = result; // set to compare the TMRx

        /* set the prescaler */
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
        /* Start the timer*/
        T2CONbits.TON = 1;
    }
}

int tmr_wait_period(int timer)
{
    if (timer == TIMER1)
    {
        T1CONbits.TON = 0;
        // Check if the flag is already raised
        if (IFS0bits.T1IF == 1)
        {
            IFS0bits.T1IF = 0;
            return 1;
        }
        T1CONbits.TON = 1; // Start the timer
        // Wait until flag is raised
        while (IFS0bits.T1IF == 0)
        {
        }
        IFS0bits.T1IF = 0; // zero the flag from 1 to 0 again.
        return 0;
    }
    else if (timer == TIMER2)
    {
        T2CONbits.TON = 0;
        // Check if the flag is already raised
        if (IFS0bits.T2IF == 1)
        {
            IFS0bits.T2IF = 0;
            return 1;
        }
        T2CONbits.TON = 1;  // Start the timer
        // Wait until flag is raised
        while (IFS0bits.T2IF == 0)
        {
        }
        IFS0bits.T2IF = 0;  // zero the flag from 1 to 0 again.
        return 0;
    }
}


void tmr_wait_ms(int timer, int ms)
{
    tmr_setup_period(timer, ms);
    tmr_wait_period(timer);
}
