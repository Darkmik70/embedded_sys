/*
 * File:   interrupts.c
 * Author: RomiC
 *
 * Created on March 21, 2024, 2:25 PM
 */

#include "interrupts.h"
#include "timer.h"
#include "xc.h"

// Interrupt routine of timer 1
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void){
    IFS0bits.T1IF = 0;          // clear the iterrupt flag 
}

// Interrupt routine of timer 2
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;          // clear the interrupt flag
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void){
    IFS0bits.T3IF = 0;
}

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void){
    IFS1bits.T4IF = 0;
}
//Interrupt routine for pressed button
void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(void){
    IFS1bits.INT1IF = 0;        // clear the interrupt flag
}

void __attribute__((__interrupt__, __auto_psv__))_INT2Interrupt(void){
    IFS1bits.INT2IF = 0;       // clear interrupt flag INT2    
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void){
    char data;
    data = U1RXREG;
    U1TXREG = data;
    
    IFS0bits.U1RXIF = 0;
}
