/*
 * File:   interrupts.c
 * Author: RomiC
 *
 * Created on 17 marzo 2024, 10.05
 */

#include "interrupts.h"
#include "xc.h"

// Interrupt routine of timer 1
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void){
    IFS0bits.T1IF = 0;  // clear the iterrupt flag for no loop 
}

// Interrupt routine of timer 2
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void){
    //LATGbits.LATG9 = !LATGbits.LATG9;   // change the state of the pin use in exercise 1
    IFS0bits.T2IF = 0;          // clear the interrupt flag
}

//Interrupt routine for pressed button
void __attribute__((__interrupt__, no_auto_psv))_INT1Interrupt(void){
    //tmr_wait_ms(TIMER2,20);
    LATGbits.LATG9 = !LATGbits.LATG9;   // change the state of the pin
    IFS1bits.INT1IF = 0;        // clear the interrupt flag
}