/*
 * File:   interrupts.c
 * Author: aless
 *
 * Created on 19 marzo 2024, 11.54
 */


#include "xc.h"
#include "functions.h"
#include "timer.h"

void __attribute__ (( __interrupt__ , __auto_psv__ )) _T1Interrupt() {
    IFS0bits.T1IF = 0; // reset interrupt flag
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _T2Interrupt() {
    toggleLed(2);
    IFS0bits.T2IF = 0; // reset interrupt flag
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _INT1Interrupt() {
    tmr_setup_period(TIMER2,20);
    IFS1bits.INT1IF = 0; // reset interrupt flag
}
