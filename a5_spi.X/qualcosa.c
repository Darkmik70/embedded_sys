
#include <p33EP512MU810.h>

#include "xc.h"
#include "function.h"
#include "timer.h"
#include "uart.h"

    
    int read_addr;
    int value_from_chip;
    
while (1) {
         
        LATDbits.LATD6 = 0; // change to the PORT connected to the chip select

        read_addr = 0x40;

        while (SPI1STATbits.SPITBF == 1);
        SPI1BUF = read_addr ;//| 0x80; // setting the MSB to 1
        
        while (SPI1STATbits.SPIRBF == 0);
        int trash = SPI1BUF; // read to prevent buffer overrun

        while (SPI1STATbits.SPITBF == 1); // clocking out zeros so that the other chip can send the value
        SPI1BUF = 0x00;
        
        while (SPI1STATbits.SPIRBF == 0);
        value_from_chip = SPI1BUF; // get the value from the register

        sendIntAsChar(value_from_chip);
        //U1TXREG = value_from_chip;
        
        LATDbits.LATD6 = 1;
        tmr_wait_ms(TIMER1,200);
        toggleLed(1);
    }