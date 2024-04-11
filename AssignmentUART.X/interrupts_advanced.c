/*
 * File:   interrupts_advanced.c
 * Author: aless
 *
 * Created on 10 aprile 2024, 15.23
 */


#include "xc.h"
#include "functions.h"
#include "timer.h"
#include "uart.h"

void __attribute__ (( __interrupt__ , __auto_psv__ )) _T1Interrupt() {
    IFS0bits.T1IF = 0;              // Clear T1 interrupt flag
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _T2Interrupt() {
    IFS0bits.T2IF = 0;              // Clear T2 interrupt flag
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _T3Interrupt() {
    toggleLed(2);
    IFS0bits.T3IF = 0;              // Clear T3 interrupt flag
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _T4Interrupt() {
    int data;
    //IFS1bits.T4IF = 0;              // Clear the interrupt 4 flag
    IEC1bits.T4IE = 0;              // Disable the interrupt of the timer 4
    
    T4CONbits.TON = 0;              // Stop timer 4
    
    // Send the number of character or the number of time the timer expired
    if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
        data = sendNumberChar('C');
        U1TXREG = 'C';
        U1TXREG = '=';
        sendIntAsChars(data);
        
        IFS1bits.INT1IF = 0;        // Clear the interrupt flag of INT1
        IEC1bits.INT1IE = 1;        // Enable the interrupt of INT1
    }
    if (T3_BUTTON == 1 && IFS1bits.INT2IF == 1) {
        data = sendNumberChar('D');
        U1TXREG = 'D';
        U1TXREG = '=';
        sendIntAsChars(data);
        
        IFS1bits.INT2IF = 0;        // Clear the interrupt flag of INT2
        IEC1bits.INT2IE = 1;        // Enable the interrupt of INT1
    }

    IFS1bits.T4IF = 0;
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _INT1Interrupt() {
    IEC1bits.INT1IE = 0;            // Disable INT1 interrupt
    IEC1bits.T4IE = 1;              // Enable T4 interrupt
    
    tmr_setup_period(TIMER4,10);        
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _INT2Interrupt() {
    IEC1bits.INT2IE = 0;            // Disable INT2 interrupt
    IEC1bits.T4IE = 1;              // Enable T4 interrupt
    
    //IFS1bits.INT2IF = 0;            // Clear INT2 interrupt flag
    tmr_setup_period(TIMER4,10);
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _U1RXInterrupt() {
    IFS0bits.U1RXIF = 0;            // Reset interrupt flag
}