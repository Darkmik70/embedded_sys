/*
 * File:   main.c
 * Author: aless
 *
 * Created on April 10, 2024, 4:19 PM
 */


#include "xc.h"
#include "functions.h"
#include "timer.h"
#include "uart.h"

/**
 * This main function have to configure SPI, make the magnetometer switch to Sleep
 * mode then make it go to active mode, and acquire the Chip ID and send it to 
 * UART
 * @return 
 */

int spi_write(unsigned int addr) {
    char trash;
    char value_from_chip = 'x';

    TRISBbits.TRISB3 = 0; // change to the PORT connected to the chip select

    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = addr | 0x80; // setting the MSB to 1
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF; // read to prevent buffer overrun
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00; // clocking out zeros so that the other chip can send the value
    while (SPI1STATbits.SPIRBF == 0);
    value_from_chip = SPI1BUF; // get the value from the register

    TRISBbits.TRISB3 = 1;
}

int main() {

    initializeIO();

    set_UART1();

    // Control Register
    SPI1CON1bits.MSTEN = 1; // Master mode
    SPI1CON1bits.MODE16 = 0; // Comunication is byte-wide (8-bit)
    SPI1CON1bits.PPRE = 2; // Primary prescaler (CERCARE PERCH? SCEGLIERE QUESTO)
    SPI1CON1bits.SPRE = 6; // Secondary prescaler
    SPI1STATbits.SPIEN = 1; // Enable the SPI 


    //    char data;    
    //    while(SPI1STATbits.SPIRBF == 1);    // wait until the tx buffer is not full
    //    SPI1BUF = 'x';      // send the 'x' character
    //    while(SPI1STATbits.SPIRBF == 0);    //wait until data has arrived
    //    data = SPI1BUF;

    while (1) {
        turnOnLed(1);

        U1TXREG = value_from_chip;

    }

    return 0;
}
