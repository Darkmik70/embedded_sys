/*
 * File:   timer.c
 * Author: RomiC
 *
 * Created on March 21, 2024, 2:24 PM
 */


#include "xc.h"
#include "timer.h"

/**
 * Function that setup the period of the timer, choose a prescaler and convert 
 * the ms into time of the clock
 * @param timer: Name of the timer
 * @param ms: Time in ms
 * @return None
 */
 void tmr_setup_period(int timer, int ms){
    int num = 0;
    int prescaler = 1;
    long new_clock;
    long clock = 72000000;
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
        T1CONbits.TON = 0;  // stop the time
  
        TMR1 = 0;        // reset the timer counter
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
        
        IFS0bits.T1IF = 0;  // Clear Timer1 Interrupt Flag
        
        // Uncomment this two lines if the interrupt is used
        //IEC0bits.T1IE = 1;  // Enable Timer1 Interrupt
        
        T1CONbits.TON = 1;  // start the timer
    }
    else if (timer == TIMER2){
        T2CONbits.TON = 0;  // stop the timer 

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
        
        IFS0bits.T2IF = 0;  // clear Timer1 Interrupt Flag
        
        // Unccoment if the interrupt is use
        //IEC0bits.T2IE = 1;  // enable Timer1 Interrupt
        
        T2CONbits.TON = 1;  // start the timer
    }    
    else if (timer == TIMER3){
        T3CONbits.TON = 0;  // stop the timer3
        IFS0bits.T3IF = 0;  // clear the flag of the timer 3
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
        
        IFS0bits.T3IF = 0;  // clear Timer3 Interrupt Flag
        
        // Uncomment if the interrupt if use
        //IEC0bits.T3IE = 1;  // enable Timer3 Interrupt
        
        T3CONbits.TON = 1;  // start the timer
    }
    else if (timer == TIMER4){
        T4CONbits.TON = 0;  // stop the timer

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
        
        IFS1bits.T4IF = 0;  // clear Timer4 Interrupt Flag
        
        // Uncomment this line is the interrupt are use
        IEC1bits.T4IE = 1;  // enable Timer4 Interrupt
        
        T4CONbits.TON = 1;  // start the timer
    }
}


int get_timer_status(int timer) {
    switch(timer) {
        case TIMER1:
            return IFS0bits.T1IF;
        case TIMER2:
            return IFS0bits.T2IF;
        case TIMER3:
            return IFS0bits.T3IF;
        case TIMER4:
            return IFS1bits.T4IF;
        }
}

/**
 * Function that control if the timer it's expired and also wait for the timer to 
 * expire the period impose
 * @param timer: Name of the TIMER
 * @return 1: TIMER EXPIRED 0: TIMER ONGOING
 */
int tmr_wait_period(int timer){     
    if (timer == TIMER1){
        if (IFS0bits.T1IF == 1){
            //IFS0bits.T1IF = 0;
            return 1;
        }
        // wait for the flag to be raised
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

/**
 * Function that set the timer period and wait to expired also handle time 
 * over 200ms.
 * @param timer: Name of the timer
 * @param ms: time in ms
 * @return None
 */
void tmr_wait_ms(int timer, int ms){
    int ret, t, resto;
    t = ms;
    while(t!=0){
        resto = t%200;
        if (resto==0){
            tmr_setup_period(timer, 200);
            ret = tmr_wait_period(timer);
            t = t - 200;
        }
        else{
            tmr_setup_period(timer, resto);
            ret = tmr_wait_period(timer);
            t = t - resto;
        }
    }    
}
