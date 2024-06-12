/*
 * File:   timer.c
 * Author: RomiC
 *
 * Created on March 12, 2024, 4:48 PM
 */


#include "timer.h"
#include "xc.h"

int set_prescaler(int ms){
    int prescaler = 1;
    long new_clock;
    long clock = 72000000;
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
        
        // Interrupt procedure 
        IFS0bits.T1IF = 0;  // Clear Timer1 Interrupt Flag
        IEC0bits.T1IE = 1;  // Enable Timer1 Interrupt
        
        T1CONbits.TON = 1;  // start the timer
    }
    else if (timer == TIMER2){
        T2CONbits.TON = 0;
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
        
        // Interrupt procedure
        //IFS0bits.T2IF = 0;  // Clear Timer1 Interrupt Flag
        //EIC0bits.T2IE = 1;  // Enable Timer1 Interrupt
        
        T2CONbits.TON = 1;  // Start the timer
    }
}

// use the timer flag to wait until it has expired
int tmr_wait_period(int timer){     
    if (timer == TIMER1){
        if (IFS0bits.T1IF == 1){
            //IFS0bits.T1IF = 0;
            return 1;
        }
        while(IFS0bits.T1IF == 0){} // Until the flag is not raised 
        //IFS0bits.T1IF = 0;
        return 0;
    }
    else if (timer == TIMER2){
        if (IFS0bits.T2IF == 1){
            //IFS0bits.T2IF = 0;
            return 1;
        }
        while(IFS0bits.T2IF == 0){}
        //IFS0bits.T2IF = 0;
        return 0;
    }
    return 0;
}

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

