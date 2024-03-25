/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on March 21, 2024, 2:21 PM
 */


#include "xc.h"
#include "function.h"
#include "interrupts.h"
#include "timer.h"
#include <string.h>

#define FCY  72000000
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16) - 1



// Exercise n.2

/**
 * Use to simulate an algorithm that takes 7 ms to be completed
 * @param None
 * @return None
 */
void algorithm(){
    tmr_wait_ms(TIMER1, 7);
}

/**
 * This function assign the button pins to the external interrupts,  
 */
void mapInterruptsButton(){
    // Assign an interrupt INT1 to the corresponding pin (we know is in this 
    // register beacuse is written in the datasheet)
    RPINR0bits.INT1R = 88;    //RPI88 in hex (88)
    IFS1bits.INT1IF = 0;        // clear the interrupt flag
    IEC1bits.INT1IE = 1;        // enable interrupt
    
    // Assign an interrupt INT2 to the corresponding pin
    RPINR1bits.INT2R = 89;    //RPI88 in hex (88)
    IFS1bits.INT2IF = 0;        // clear the interrupt flag INT2
    IEC1bits.INT2IE = 1;        // enable external interrupt iNT2
}


/**
 * This function assign the UART into the pin where we read and send
 */
void assignUART(){
    // assign an interrupt to the corresponding pin bits
    TRISDbits.TRISD11 = 1;       // set RD11 as input
    RPINR18bits.U1RXR = 0x4b;   // remap UART1 receive to RD11
    RPOR0bits.RP64R = 1;        // remap RD0 to RP64
    
    // put in the initialization
    U1BRG = BRGVAL;             // baud rate register
    
    // interrupt procedure 
    IFS0bits.U1RXIF = 0;        // Flag clear Interrupt U1RX
    IEC0bits.U1RXIE = 1;        // Enable the interrupt U1RX
    
    // At
    U1MODEbits.UARTEN = 1;      // Enable UART
    U1STAbits.UTXEN = 1;        // Enable UART U1TX (must be after UARTEN)
    
    U1STAbits.URXISEL = 0;      // Flag goes to zero when a char is received
}



// Exercise n.1
int main(void){
    
    // Inizialize the input and output pins 
    initializeIO();
    
    // assign an interrupt to the corresponding pin bits
    assignUART();
    
    while(1){
    }
    return 0;
}





