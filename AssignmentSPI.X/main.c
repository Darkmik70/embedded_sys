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

int spi_write(unsigned int addr, unsigned int data) {
    char trash;
    int value_from_chip;
    
    TRISDbits.TRISD6 = 0; // change to the PORT connected to the chip select

    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = addr | 0x80; // setting the MSB to 0
    while (SPI1STATbits.SPIRBF == 0);
    trash = SPI1BUF; // read to prevent buffer overrun
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = data; // clocking out zeros so that the other chip can send the value
    while (SPI1STATbits.SPIRBF == 0);
    value_from_chip = SPI1BUF; // get the value from the register

    TRISDbits.TRISD6 = 1;
    
    return value_from_chip;
}

void mapSPI() {
    // Control Register
    SPI1CON1bits.MSTEN = 1; // Master mode
    SPI1CON1bits.MODE16 = 0; // Comunication is byte-wide (8-bit)
    SPI1CON1bits.PPRE = 2; // Primary prescaler 
    SPI1CON1bits.SPRE = 6; // Secondary prescaler
    SPI1STATbits.SPIEN = 1; // Enable the SPI
}

int main() {
    char data;
    initializeIO();
    set_UART1();
    //mapSPI();
    SPI1CON1bits.MSTEN = 1; // master mode
    SPI1CON1bits.MODE16 = 0; // 8?bit mode
    SPI1CON1bits.PPRE = 3; // 1:1 primary prescaler
    SPI1CON1bits.SPRE = 3; // 5:1 secondary prescaler
    SPI1STATbits.SPIEN = 1; // enable SPI

    while (1) {
        data = spi_write(0x40, 0x00);
        sendIntAsChars(data);
        //U1TXREG = data;
    }
    return 0;
}
