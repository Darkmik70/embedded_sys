/*
 * File:   main.c
 * Author: michalkrepa
 *
 * Created on April 23, 2024, 12:41 PM
 */


#include "xc.h"
#include "timer.h"

#define FCY 72000000
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1
unsigned int i;


/* EX 1
 * 1. Configure the UART1 to use the MikroBUS 2
 * 2. Remap IO pin corresponding to D11 and RD0
 * then perform premapping of uart1
 * 3. Configure UART1 to work at 9600 bps
 * Read a character from uart1 and send it back to the uart1
 * */

void set_uart_1() {

    TRISDbits.TRISD11 = 1; // Set RD11 as input
    RPINR18bits.U1RXR = 75; // Map UART1 receive to RP75/RD11

    RPOR0bits.RP64R = 1; //  U1TX transmit to RP64/RD0
    
    //    U1MODEbits.STSEL = 0; // 1 Stop bit
    //    U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
    //    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    //    U1MODEbits.BRGH = 0; // Low Speed mode

    U1BRG = BRGVAL; // BAUD Rate Setting for 9600

    // Receiver
    IFS0bits.U1RXIF = 0; // Clear the U1RX interrupt flag
    IEC0bits.U1RXIE = 1; // Enable the U1RX interrupt

    // Transmitter
    //    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
    //    U1STAbits.UTXISEL1 = 0;

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX

    U1STAbits.URXISEL = 0; // Receive Interrupt Mode Selection bit
}

void init_io() {
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    TRISAbits.TRISA0 = 0; // Set LD1 pin as output
    TRISGbits.TRISG9 = 0; // Set LD2 pin as output
    TRISEbits.TRISE8 = 1; // Set T2 pin as input
    TRISEbits.TRISE9 = 1; // Set T3 pin as input
}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _U1RXInterrupt() {
    char data;
    data = U1RXREG;         // Read from RX register
    U1TXREG = data + 1;     // Send to TX register next ASCII sign
    
    IFS0bits.U1RXIF = 0;    // Reset interrupt flag
}

int main(void) {
    
    init_io();
    
    set_uart_1();
    
    while(1)
    {
    }
    
    return 0;
}
