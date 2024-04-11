/*
 * File:   interrupts.c
 * Author: RomiC  Aless
 *
 * Created on 17 marzo 2024, 10.05
 */


#include "xc.h"
#include "timer.h"
#include "function.h"
#include "uart.h"

// Interrupt routine of timer 1
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(){
    IFS0bits.T1IF = 0;  // clear the interrupt flag for no loop 
}

// Interrupt routine of timer 2
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(){
    IFS0bits.T2IF = 0;          // clear the interrupt flag
}

// Interrupt routine of timer 3, use to toggle the LED2
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(){
    toggleLed(2);
    IFS0bits.T3IF = 0;
}

// Interrupt routine of the timer 4, use to solving the debouncing problem
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(){
    IEC1bits.T4IE = 0;      // disable the interrupt of the timer 2
    T4CONbits.TON = 0;      // stop the timer 2
    //IFS1bits.T4IF = 0;      // clear the flag on the interrupt 2
}

// Interrupt routine associated to the pressed button T2
void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(){
    IEC1bits.INT1IE = 0;        // disable the interrupt of INT1
    IEC1bits.T4IE = 1;          // enable the interrupt of the TIMER4 
    tmr_setup_period(TIMER4,10);
}

// Interrupt routine associated to the pressed button T3
void __attribute__((__interrupt__, __auto_psv__))_INT2Interrupt(){
    IEC1bits.INT2IE = 0;        // disable the interrupt of the INT2
    IEC1bits.T4IE = 1;          // enable the interrupt of the TIMER4 
    tmr_setup_period(TIMER4,10);
}

// Interrupt routine associated to the receivement of message on the UART
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(){
    /* use in exercise 1 
    char data;
    data = U1RXREG;
    U1TXREG = data;
    */
    IFS0bits.U1RXIF = 0;
}
