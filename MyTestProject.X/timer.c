/*
 * File:   timer.c
 * Author: aless
 *
 * Created on 12 marzo 2024, 15.11
 */


#include "xc.h"
#include "timer.h"

void tmr_setup_period(int timer, int ms) {
    long new_clocks, clock_steps, Fcy;
    int result;
    int num = 0;
    long Fosc = 144000000;
    Fcy = Fosc / 2;
    clock_steps = Fcy / 2;
    // Setting the prescaler
    if (clock_steps > 65535) {
        new_clocks = clock_steps / 8;
        num = 1;
        if (new_clocks > 65535) {
            new_clocks = clock_steps / 16;
            num = 2;
            if (new_clocks > 65535) {
                new_clocks = clock_steps / 256;
                num = 3;
            }
        }
    }
    result = (new_clocks)*((float) ms / 1000); // value of the timer
    // Setting the timer 1
    if (timer == TIMER1) {
        T1CONbits.TON = 0;
        IFS0bits.T1IF = 0;
        TMR1 = 0; // reset the timer counter
        PR1 = result; // set to compare the TMRx
        if (num == 3) {
            T1CONbits.TCKPS = 3; // sets the prescaler
        } else if (num == 2) {
            T1CONbits.TCKPS = 2; // sets the prescaler
        } else if (num == 1) {
            T1CONbits.TCKPS = 1; // sets the prescaler
        } else if (num == 0) {
            T1CONbits.TCKPS = 0; // sets the prescaler
        }
        T1CONbits.TON = 1; // starts the timer
    }        // Setting the timer 2
    else if (timer == TIMER2) {
        T2CONbits.TON = 0;
        IFS0bits.T2IF = 0;
        TMR2 = 0;
        PR2 = result; // set to compare the TMRx
        if (num == 3) {
            T2CONbits.TCKPS = 3; // sets the prescaler
        } else if (num == 2) {
            T2CONbits.TCKPS = 2; // sets the prescaler
        } else if (num == 1) {
            T2CONbits.TCKPS = 1; // sets the prescaler
        } else if (num == 0) {
            T2CONbits.TCKPS = 0; // sets the prescaler
        }
        T2CONbits.TON = 1; // starts the timer
    }
}

// use the timer flag to wait until it has expired

int tmr_wait_period(int timer) {
    if (timer == TIMER1) {
        T1CONbits.TON = 0;
        if (IFS0bits.T1IF == 1) {
            IFS0bits.T1IF = 0;
            return 1;
        }
        T1CONbits.TON = 1;
        while (IFS0bits.T1IF == 0) {
        } // Until the flag is not raised 
        IFS0bits.T1IF = 0;
        return 0;
    } else if (timer == TIMER2) {
        T2CONbits.TON = 0;
        if (IFS0bits.T2IF == 1) {
            IFS0bits.T2IF = 0;
            return 1;
        }
        T2CONbits.TON = 1;
        while (IFS0bits.T2IF == 0) {
        }
        IFS0bits.T2IF = 0;
        return 0;
    }
}

void tmr_wait_ms(int timer, int ms) {
    tmr_setup_period(timer, ms);
    tmr_wait_period(timer);
}