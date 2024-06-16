/*
 * File:   ex1_basic.c
 * Author: aless
 *
 * Created on 21 marzo 2024, 14.29
 */


#include "xc.h"
#include "functions.h"
#include "timer.h"

#define FCY 72000000
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16) - 1

//void set_UART1() {
//    TRISDbits.TRISD11 = 1;  // Set RD11 as input
//    RPINR18bits.U1RXR = 75; // Map UART1 receive to RD11
//    RPOR0bits.RP64R = 1;    // Map UART1 transmit to RD0
//    
//    U1BRG = BRGVAL;         // Baud rate register
//    
//    IFS0bits.U1RXIF = 0;    // Clear the U1RX interrupt flag
//    IEC0bits.U1RXIE = 1;    // Enable the U1RX interrupt
//    
//    U1MODEbits.UARTEN = 1;  // Enable UART
//    U1STAbits.UTXEN = 1;    // Enable U1TX (must be after UARTEN)
//    
//    U1STAbits.URXISEL = 0;  // Receive Interrupt Mode Selection bit
//}
void __attribute__ (( __interrupt__ , __auto_psv__ )) _U1RXInterrupt() {
    char data;
    data = U1RXREG;         // Read from RX register
    U1TXREG = data;         // Send to TX register
    
    IFS0bits.U1RXIF = 0;    // Reset interrupt flag
}


int main(void) {
    initializeIO();
    set_UART1();
    while (1) {
    }
    return 0;
}
