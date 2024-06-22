/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on April 25, 2024, 3:26 PM
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"


// Exercise n.2
int main() {
    
    initializeIO();
    initUART();
    initSPI();
  
    // Pass from the suspend mode of the magnetometer to the sleep and then 
    // to the active mode
    setACTIVEmode('M');
    
    int16_t mag_LSB,mag_MSB,value;
    
    char buffer[20];
    
    while(1){
        CS3 = 0;
        
        // x-axis of the magnetometer register 0x42 and 0x43
        spi_write(0x42 | 0x80);     // access to the register
        tmr_wait_ms(TIMER2,2);
        mag_LSB = spi_write(0x00);  // read the value of the LSB
        mag_MSB = spi_write(0x00);  // read the value of the MSB
        
        // put the LSB and MSB together in a two byte signed integer
        
        mag_LSB = mag_LSB & 0b11111000; // clear the three least significant 
                                        // bits by ANDing with a mask
        
        mag_MSB = mag_MSB << 8;     // left-shift by 8 the MSB
        // left-shifted EXAMPLE: a = 5 binary 101; b = 2; 
        //a << b = 10100 so a<<b = 20 (not in binary)
        
        value = mag_MSB | mag_LSB;  // and OR with the previously found masked
        value = value/8;            // divided by 8 to get the correct scale
        
        sprintf(buffer, "$MAGX= %d* \0", (int16_t)value);
        for (int i = 0;buffer[i] != '\0'; i++)
        {
            while(U1STAbits.UTXBF == 1);                
            U1TXREG = buffer[i];      
        }
        
        CS3 = 1;
        tmr_wait_ms(TIMER1,50);
    }
     
    return 0;
}

/** Exercise n.1
 * This main function have to configure SPI, make the magnetometer switch to Sleep
 * mode then make it go to active mode, and acquire the Chip ID and send it to 
 * UART
 * @return None
 */
/*int main() {
    
    initializeIO();
    initUART();
    initSPI();          // Configure SPI
    mappingCHIPset();
    
    setACTIVEmode('M'); // magnetometer switch from suspend to sleep and then to active
    
    int value_from_chip;
    
    while(1){
        CS3 = 0; // change to the PORT connected to the chip select
        spi_write(0x40 | 0x80);        
        value_from_chip = spi_write(0x00);
        U1TXREG = value_from_chip;      // Non usare la nostra funzione perchè il valore non viene
        CS3 = 1;
        toggleLed(1);
        tmr_wait_ms(TIMER1,1000);
    }
     
    return 0;
}
*/
