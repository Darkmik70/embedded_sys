/*
 * File:   timer.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:40 PM
 */
#include "xc.h"
#include "timer.h"

int set_prescaler(int ms){
    int prescaler = 1;
    long new_clock;
    long clock = 72000000;
    // 65535 + 1 = 2^16
    if (((float)ms)*clock > 65535){
        prescaler = 8;
        new_clock = clock/prescaler;
        if (((float)ms)*new_clock > 65535){
            prescaler = 64;
            new_clock = clock/prescaler;
            if (((float)ms)*new_clock > 65535){
                prescaler = 256;
            }
        }
    }
    int result = (clock/prescaler)*((float)ms/1000);
    return result;
}

int number_prescaler(int ms){
    int prescaler = 1;
    long new_clock;
    long clock = 72000000;
    int num = 0;
    if (((float)ms)*clock > 65535){
        prescaler = 8;
        new_clock = clock/prescaler;
        num = 1;
        if (((float)ms)*new_clock > 65535){
            prescaler = 64;
            new_clock = clock/prescaler;
            num = 2;
            if (((float)ms)*new_clock > 65535){
                num = 3;
            }
        }
    }
    return num;
}

void tmr_setup_period(int timer, int ms){
    int num = number_prescaler(ms);
    if (timer == TIMER1){
        T1CONbits.TON = 0;
        IFS0bits.T1IF = 0;
        TMR1 = 0;                   // reset the timer counter
        PR1 = set_prescaler(ms);    // set to compare the TMRx
        if (num == 3){
            T1CONbits.TCKPS = 3;
        }
        else if (num == 2){
            T1CONbits.TCKPS = 2;
        }
        else if (num == 1){
            T1CONbits.TCKPS = 1;
        }
        else if (num == 0){
            T1CONbits.TCKPS = 0;
        }
        T1CONbits.TON = 1;  // start the timer
    }
    else if (timer == TIMER2){
        T2CONbits.TON = 0;
        IFS0bits.T2IF = 0;
        TMR2 = 0;
        PR2 = set_prescaler(ms);
        
        if (num == 3){
            T2CONbits.TCKPS = 3;
        }
        else if (num == 2){
            T2CONbits.TCKPS = 2;
        }
        else if (num == 1){
            T2CONbits.TCKPS = 1;
        }
        else if (num == 0){
            T2CONbits.TCKPS = 0;
        }
        T2CONbits.TON = 1;
    }
}

// use the timer flag to wait until it has expired
int tmr_wait_period(int timer){     
    if (timer == TIMER1){
        T1CONbits.TON = 0;
        if (IFS0bits.T1IF == 1){
            IFS0bits.T1IF = 0;
            return 1;
        }
        T1CONbits.TON = 1;
        while(IFS0bits.T1IF == 0){} // Until the flag is not raised 
        IFS0bits.T1IF = 0;
        return 0;
    }
    else if (timer == TIMER2){
        T2CONbits.TON = 0;
        if (IFS0bits.T2IF == 1){
            IFS0bits.T2IF = 0;
            return 1;
        }
        T2CONbits.TON = 1;
        while(IFS0bits.T2IF == 0){}
        IFS0bits.T2IF = 0;
        return 0;
    }
}


void tmr_wait_ms(int timer, int ms)
{
    tmr_setup_period(timer, ms);
    tmr_wait_period(timer);
}
