/*
 * File:   ex1.c
 * Author: RomiC
 *
 * Created on 5 march 2024, 18.46
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

void tmr_wait_ms(int timer, int ms){
    int ret;
    tmr_setup_period(timer, ms);
    ret = tmr_wait_period(timer);
}

int main(void) {
    
    // all the analog are disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // initialized the function of the pins
    TRISAbits.TRISA0 = 0;   // set pin as output
    TRISGbits.TRISG9 = 0;   // set pin as output
    
    tmr_setup_period(TIMER1,200);
    
    int delay = 200;     // we have to try 50, 200, 2000
    int ret;
    
    while(1){
        LATAbits.LATA0 = 1;
        tmr_wait_ms(TIMER2,delay);
        LATAbits.LATA0 = 0;  
        ret = tmr_wait_period(TIMER1);
        if (ret){
            LATGbits.LATG9 = 1;
        }
        else {
            LATGbits.LATG9 = 0;
        }
    }
    return 0;
}

/* Exercise n.1
int main(void) {
    // all the analog are disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // initialized the function of the pins
    TRISAbits.TRISA0 = 0; // set as output
    
    // initialized the variables
    int pinValue;
    pinValue = 0;   // initial value of the pin

    tmr_setup_period(TIMER1,200); // set up the timer to count 200 millisecond
    while(1){        
        if (pinValue ==0){
            LATAbits.LATA0 = 1;
            pinValue = 1;
        }
        else{
            LATAbits.LATA0 = 0;
            pinValue = 0;
        }
        tmr_wait_period(TIMER1);
    }
    return 0;
}
*/

/* Exercise n.2
int main(void) {
    // all the analog are disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // initialized the function of the pins
    TRISAbits.TRISA0 = 0;   // set pin as output
    TRISGbits.TRISG9 = 0;   // set pin as output
    
    tmr_setup_period(TIMER1,200);
    while(1){
        tmr_wait_ms(TIMER2,20);
        LATAbits.LATA0 = 0;
        tmr_wait_ms(TIMER2,200);
        LATAbits.LATA0 = 1;          
    }
    return 0;
}

 */


