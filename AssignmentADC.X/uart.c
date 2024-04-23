/*
 * File:   uart.c
 * Author: aless
 *
 * Created on 10 aprile 2024, 15.48
 */


#include "xc.h"
#include "uart.h"
#include <stdio.h>

int countChar, missedTimer;

void receiveMsg(int num, char type) {
    if (type == 'C') {
        countChar = num;
    } else if (type == 'D') {
        missedTimer = num;
    }
}

int sendNumberChar(char type) {
    if (type == 'C') {
        return countChar;
    } else if (type == 'D') {
        return missedTimer;
    }
    return 0;
}

void sendIntAsChars(int num) {
    // Convert the integer into a sequence of characters
    char buffer[10]; // Assume the integer has at most 10 digits
    sprintf(buffer, "%d", num);

    // Send each character of the sequence
    for (int i = 0; i < strlen(buffer);) {
        
        if (U1STAbits.UTXBF == 0) {
            U1TXREG = buffer[i];
            i++;
        } else {
            while (U1STAbits.UTXBF == 1) {}
        }
    }
}

void set_UART1() {
    TRISDbits.TRISD11 = 1; // Set RD11 as input
    RPINR18bits.U1RXR = 75; // Map UART1 receive to RD11
    RPOR0bits.RP64R = 1; // Map UART1 transmit to RD0

    U1BRG = BRGVAL; // Baud rate register

    IFS0bits.U1RXIF = 0; // Clear the U1RX interrupt flag
    IEC0bits.U1RXIE = 1; // Enable the U1RX interrupt

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable U1TX (must be after UARTEN)

    U1STAbits.URXISEL = 0; // Receive Interrupt Mode Selection bit
}
