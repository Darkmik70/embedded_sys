/*
 * File:   UART functions
 * Author: RomiC
 *
 * Created on 1 aprile 2024, 21.39
 */


#include "xc.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>


// Global variable use to 
int countChar, missedTimer;


/**
 * This function assign the UART into the pin where we read and send, it is
 * also define the baud rate register
 */
void initUART(){
    // assign an interrupt to the corresponding pin bits
    TRISDbits.TRISD11 = 1;      // set RD11 as input
    RPINR18bits.U1RXR = 75;     // remap UART1 receive to RD11
    RPOR0bits.RP64R = 1;        // remap RD0 to RP64
    
    // put in the initialization
    U1BRG = BRGVAL;             // baud rate register
    
    // interrupt procedure 
    IFS0bits.U1RXIF = 0;        // Flag clear Interrupt U1RX
    IEC0bits.U1RXIE = 1;        // Enable the interrupt U1RX
    
    // enable the UART
    U1MODEbits.UARTEN = 1;      // Enable UART
    U1STAbits.UTXEN = 1;        // Enable UART U1TX (must be after UARTEN)
    
    U1STAbits.URXISEL = 0;      // Flag goes to zero when a char is received
}


/**
 * This function save the number of character that are receive when call
 * @param num
 */
void receiveMsg(int num, char type){
    if(type == 'C'){
        countChar = num;
    }
    else if(type == 'D'){
        missedTimer = num;
    }
}

/**
 * Function that returns the number of character receive by the main 
 * @return countChar: Number of character receive in the comunication 
 * @return missedTimer: Number of times the timer expire 
 */
int sendNumberChar(char type){
    if(type == 'C'){    
        return countChar;
    }
    else if(type == 'D'){
        return missedTimer;
    }
    return 0;
}


/**
 * This function convert an integer number into a sequence of characters, then
 * the number is send in the UART trasmitter 
 * @param num
 * @return None
 */
void sendIntAsChar(int16_t num) {
    char buffer[10];            // Assume the integer has at most 10 digits
    sprintf(buffer, "%d", num);
    
    // Send each character of the sequence and also control if the fifo is full
    for (int i = 0; i < strlen(buffer); i++) {
        while (U1STAbits.UTXBF == 1) {} // this fflag see if the fifo is full
        U1TXREG = buffer[i];
        }
}
