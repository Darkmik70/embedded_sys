/*
 * File:   timer.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:40 PM
 */
#include "xc.h"
#include "timer.h"



void tmr_setup_period(int timer, int ms) {
    long clock = 72000000;      // 72 MHz
    int ps_mode = 0;            // Mode of prescaler 0, 1, 2, 3
    int prescaler = 1;          // Value of prescaler
    long new_clock;          // Prescaled clock
    /* check if we need to use prescaler */
    if ((float)ms * clock > 65535) // Check if the number of steps is within thershold of 16bit
    {
        // too many steps, use prescaler
        prescaler = 8;  // 1:8 
        new_clock = clock / prescaler; 
        ps_mode = 1;
        if ((float)ms * new_clock > 65535) // Check again
        {
            prescaler = 64; // 1:64
            new_clock = clock / prescaler; 
            ps_mode = 2;
            if ((float)ms * new_clock > 65535) //and again
            {
                prescaler = 256;    // 1:256
                new_clock = clock / prescaler; 
                ps_mode = 3;
            }
        }
    }
    /* Set the value for the timer period */
    long result = new_clock * ( (float) ms / 1000); // value of the timer
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
        else if (ps_mode == 0)
        {
            T1CONbits.TCKPS = 0;
        }
        //IFS0bits.T1IF = 0; // clear TIMER1 interrupt flag
        //IEC0bits.T1IE = 1; // enable TIMER1 interrupt
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
        if (ps_mode == 3)
        {
            T2CONbits.TCKPS = 3; 
        }
        else if (ps_mode == 2)
        {
            T2CONbits.TCKPS = 2; 
        }
        else if (ps_mode == 1)
        {
            T2CONbits.TCKPS = 1;
        }
        else if (ps_mode == 0)
        {
            T2CONbits.TCKPS = 0; 
        }
        IFS0bits.T2IF = 0; // clear TIMER1 interrupt flag
        IEC0bits.T2IE = 1; // enable TIMER1 interrupt
        /* Start the timer*/
        T2CONbits.TON = 1;
    }
}


int tmr_wait_period(int timer) {
    if (timer == TIMER1)
    {
        // Check if the flag is already raised
        if (IFS0bits.T1IF == 1)
        {
            IFS0bits.T1IF = 0;
            return 1;
        }
        // Wait until flag is raised
        while (IFS0bits.T1IF == 0)
        {
        }
        IFS0bits.T1IF = 0; // zero the flag from 1 to 0 again.
    }
    else if (timer == TIMER2)
    {
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
    }
    return 0;
}


void tmr_wait_ms(int timer, int ms)
{
    
}
