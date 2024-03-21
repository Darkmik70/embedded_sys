/*
 * File:   interrupts.c
 * Author: RomiC
 *
 * Created on 17 marzo 2024, 10.05
 */

#include "interrupts.h"
#include "timer.h"
#include "xc.h"

// Interrupt routine of timer 1
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void){
    IFS0bits.T1IF = 0;  // clear the iterrupt flag for no loop 
}

// Interrupt routine of timer 2
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;      // clear the flag on the interrupt 2
    IEC0bits.T2IE = 0;      // disable the interrupt of the timer 2
    
    T2CONbits.TON = 0;      // stop the timer 2
    IEC1bits.INT1IE = 1;
    
    if(PORTEbits.RE8 == 1){    
        LATGbits.LATG9 = !LATGbits.LATG9;
    }
    IFS0bits.T2IF = 0;          // clear the interrupt flag
}

//Interrupt routine for pressed button
void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(void){
    IFS1bits.INT1IF = 0;        // clear the interrupt flag
    IEC0bits.T2IE = 1;          // disable the interrupt 
    tmr_setup_period(TIMER2,20);
}