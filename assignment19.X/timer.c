/*
 * File:   timer.c
 * Author: emanu
 *
 * Created on 8 giugno 2024, 12.27
 */


#include <xc.h>
#include "timer.h"
#include "scheduler.h"

void init_timer() {
    // Configure Timer1 for 1 kHz interrupt
    T1CONbits.TCKPS = 0;    // Prescaler 1:1
    PR1 = 15999;            // Period register for 1 kHz with Fosc = 16 MHz
    TMR1 = 0;               // Clear timer register
    IPC0bits.T1IP = 5;      // Timer1 interrupt priority
    IFS0bits.T1IF = 0;      // Clear Timer1 interrupt flag
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    T1CONbits.TON = 1;      // Start Timer1
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    // Handle Timer1 interrupt
    IFS0bits.T1IF = 0; // Clear Timer1 interrupt flag
    scheduler();      // Call scheduler function for 1 kHz tasks
}

