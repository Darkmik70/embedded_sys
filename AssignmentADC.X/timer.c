/*
 * File:   ex1.c
 * Author: RomiC
 *
 * Created on 5 march 2024, 18.46
 */

#include "xc.h"
#include "timer.h"

/**
 * jjjj
 * @param timer
 * @param ms
 */
void tmr_setup_period(int timer, int ms){
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
                prescaler = 256;
                num = 3;
            }
        }
    }
    int result = (clock/prescaler)*((float)ms/1000);
    if (timer == TIMER1){
        T1CONbits.TON = 0;
        IFS0bits.T1IF = 0;
        TMR1 = 0;                   // reset the timer counter
        PR1 = result;    // set to compare the TMRx
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
        
        //IFS0bits.T1IF = 0; // clear TIMER1 interrupt flag
        //IEC0bits.T1IE = 1; // enable TIMER1 interrupt
        
        T1CONbits.TON = 1;  // start the timer
    }
    else if (timer == TIMER2){
        T2CONbits.TON = 0;
        IFS0bits.T2IF = 0;
        TMR2 = 0;
        PR2 = result;
        
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
        
        //IFS0bits.T2IF = 0; // clear TIMER1 interrupt flag
        //IEC0bits.T2IE = 1; // enable TIMER1 interrupt
        
        T2CONbits.TON = 1;
    }
    else if (timer == TIMER3){
        T3CONbits.TON = 0;
        IFS0bits.T3IF = 0;
        TMR3 = 0;
        PR3 = result;
        
        if (num == 3){
            T3CONbits.TCKPS = 3;
        }
        else if (num == 2){
            T3CONbits.TCKPS = 2;
        }
        else if (num == 1){
            T3CONbits.TCKPS = 1;
        }
        else if (num == 0){
            T3CONbits.TCKPS = 0;
        }
        
        IFS0bits.T3IF = 0; // clear TIMER1 interrupt flag
        IEC0bits.T3IE = 1; // enable TIMER1 interrupt
        
        T3CONbits.TON = 1;
    }
    else if (timer == TIMER4){
        T4CONbits.TON = 0;
        TMR4 = 0;
        PR4 = result;
        
        if (num == 3){
            T4CONbits.TCKPS = 3;
        }
        else if (num == 2){
            T4CONbits.TCKPS = 2;
        }
        else if (num == 1){
            T4CONbits.TCKPS = 1;
        }
        else if (num == 0){
            T4CONbits.TCKPS = 0;
        }
        // Interrupt procedure
        IFS1bits.T4IF = 0;  // Clear Timer1 Interrupt Flag
        IEC1bits.T4IE = 1;  // Enable Timer1 Interrupt
        
        T4CONbits.TON = 1;  // Start the timer
    }
}

// use the timer flag to wait until it has expired
int tmr_wait_period(int timer){     
    if (timer == TIMER1){
        if (IFS0bits.T1IF == 1){
            IFS0bits.T1IF = 0;
            return 1;
        }
        while(IFS0bits.T1IF == 0){} // Until the flag is not raised 
        IFS0bits.T1IF = 0;
    }
    else if (timer == TIMER2){
        if (IFS0bits.T2IF == 1){
            IFS0bits.T2IF = 0;
            return 1;
        }
        while(IFS0bits.T2IF == 0){}
        IFS0bits.T2IF = 0;
    }
    else if (timer == TIMER3){
        if (IFS0bits.T3IF == 1){
            IFS0bits.T3IF = 0;
            return 1;
        }
        while(IFS0bits.T3IF == 0){}
        IFS0bits.T3IF = 0;
    }
    else if (timer == TIMER4){
        if (IFS1bits.T4IF == 1){
            IFS1bits.T4IF = 0;
            return 1;
        }
        while(IFS1bits.T4IF == 0){}
        IFS1bits.T4IF = 0;
    }
    return 0;
}

void tmr_wait_ms(int timer, int ms){
    int ret, t, rest;
    t = ms;
    while(t!=0){
        rest = t%200;
        if (rest==0){
            tmr_setup_period(timer, 200);
            ret = tmr_wait_period(timer);
            t = t - 200;
        }
        else{
            tmr_setup_period(timer, rest);
            ret = tmr_wait_period(timer);
            t = t - rest;
        }
    }
}