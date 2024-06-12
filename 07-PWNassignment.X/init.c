/*
 * File:   init.c
 * Author: RomiC
 *
 * Created on April 28, 2024, 12:36 PM
 */

#include "xc.h"
#include "timer.h"
#include "init.h"
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This function assign the UART into the pin where we read and send, it is
 * also define the baud rate register
 */
void initUART() {
    // assign an interrupt to the corresponding pin bits
    //TRISDbits.TRISD11 = 1;      // set RD11 as input
    RPOR0bits.RP64R = 1; // remap RD0 to RP64
    RPINR18bits.U1RXR = 75; // remap UART1 receive to RD11

    // put in the initialization
    U1BRG = (int) BRGVAL; // baud rate register

    // interrupt procedure 
    IFS0bits.U1RXIF = 0; // Flag clear Interrupt U1RX
    IEC0bits.U1RXIE = 1; // Enable the interrupt U1RX
    
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 1;
    
    // enable the UART
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART U1TX (must be after UARTEN)

    U1STAbits.URXISEL = 0; // Flag goes to zero when a char is received
}


