/*
 * File:   timer.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:40 PM
 */
#include "xc.h"
#include "timer.h"


/* BUTTONS */
void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt() {
    IEC1bits.INT1IE = 0; // Disable INT1 interrupt
    IEC1bits.T4IE = 1; // Enable T4 interrupt

    tmr_setup_period(TIMER4, 10);
}

void __attribute__((__interrupt__, __auto_psv__)) _INT2Interrupt() {
    IEC1bits.INT2IE = 0; // Disable INT2 interrupt
    IEC1bits.T4IE = 1; // Enable T4 interrupt

    //IFS1bits.INT2IF = 0;            // Clear INT2 interrupt flag
    tmr_setup_period(TIMER4, 10);
}

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


// TMR1 - Interrupts are enabled
// TMR2 - Interrupts are enabled
// TMR3 - Interrupts are enabled
// TMR4 - Interrupts are disabled
void tmr_setup_period(int timer, int ms){
    int num = number_prescaler(ms);
    if (timer == TIMER1){
        T1CONbits.TON = 0;
        IFS0bits.T1IF = 0;          // interrupt flag
        IEC0bits.T1IE = 1;          // enable TMR1 interrupt
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
        // start the timer
        T1CONbits.TON = 1;  
    }
    else if (timer == TIMER2){
        T2CONbits.TON = 0;
        IFS0bits.T2IF = 0;
        IEC0bits.T2IE = 1;          // Enable TMR2 interrupt
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
        // Start the timer
        T2CONbits.TON = 1;
    }
    else if (timer == TIMER3){
        T3CONbits.TON = 0;
        IFS0bits.T3IF = 0;
        IEC0bits.T3IE = 1;          // Enable TMR3 interrupt
        TMR3 = 0;
        PR3 = set_prescaler(ms);
        
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
        // Start the timer
        T3CONbits.TON = 1;
    }
    else if (timer == TIMER4){
        T4CONbits.TON = 0;
        IFS1bits.T4IF = 0;
        IEC1bits.T4IE = 1;          // Enable TMR4 interrupt
        TMR4 = 0;
        PR4 = set_prescaler(ms);
        
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
        // Start the timer
        T4CONbits.TON = 1;
    }
}




// TMR1 - Interrupts are enabled
// TMR2 - Interrupts are enabled
// TMR3 - Interrupts are enabled
// TMR4 - Interrupts are enabled
int tmr_wait_period(int timer){     
    if (timer == TIMER1){
        if (IFS0bits.T1IF == 1){
//            IFS0bits.T1IF = 0;
            return 1;
        }
        while(IFS0bits.T1IF == 0){} // Until the flag is not raised 
//        IFS0bits.T1IF = 0;
        return 0;
    }
    else if (timer == TIMER2){
        if (IFS0bits.T2IF == 1){
//            IFS0bits.T2IF = 0;
            return 1;
        }
        while(IFS0bits.T2IF == 0)
        {
        }
//        IFS0bits.T2IF = 0;
        return 0;
    }
    else if (timer == TIMER3) {
        if (IFS0bits.T3IF == 1) {
//            IFS0bits.T3IF = 0;
            return 1;
        }
        while(IFS0bits.T3IF == 0)
        {
        }
//        IFS0bits.T3IF = 0;
        return 0;
    }
    else if (timer == TIMER4) {
        if (IFS1bits.T4IF == 1) {
//            IFS1bits.T4IF = 0;
            return 1;
        }
        while (IFS1bits.T4IF == 0) 
        {
        }
//        IFS1bits.T4IF = 0;
        return 0;
    }
    else
    {
        return 2;
    }
}

void tmr_wait_ms(int timer, int ms)
{
    tmr_setup_period(timer, ms);
    tmr_wait_period(timer);
}
