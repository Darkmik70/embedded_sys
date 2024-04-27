/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on April 25, 2024, 3:26 PM
 */

#include "xc.h"
#include "function.h"
#include "timer.h"
#include "uart.h"


// Magnetometer
#define CS3 LATDbits.LATD6

// Gyroscope
#define CS2 LATBbits.LATB4

// Accelerometers
#define CS1 LATBbits.LATB3


/**
 * Function that write the address to the SPI and return the value read from the 
 * other device.
 * Used to pass from suspend mode to sleep mode and then to active mode, the 
 * first time is called is for setting the MSB then the second call have to be 
 * 1 to read and 0 to write
 * @param addr
 * @return value
 */
int16_t spi_write(unsigned int addr){
    
    int16_t value;
    
    while(SPI1STATbits.SPITBF == 1);
    SPI1BUF = addr;
    while(SPI1STATbits.SPIRBF == 0);
    value = SPI1BUF;
    
    return value;
}

/**
 * Function that remap the MOSI MISO and SCK, also configure the master mode, the
 * prescaler 
 */
void initSPI(){
    
    // Control Register
    SPI1CON1bits.MSTEN = 1;     // enable the master mode
    SPI1CON1bits.MODE16 = 0;    // Comunication is byte-wide (8-bit)
    SPI1CON1bits.PPRE = 0;      // Primary prescaler (CERCARE PERCHè SCEGLIERE QUESTO)
    SPI1CON1bits.SPRE = 6;      // Secondary prescaler
    //SPI1CON1bits.CKP specifies the IDLE value of the clock (high)
    
    SPI1STATbits.SPIEN = 1;     // Enable the SPI peripheral
    
    
    // Set pins as input or output then remap the pins
    TRISAbits.TRISA1 = 1; // RA1-RPI17 MISO
    TRISFbits.TRISF12 = 0; //RF12-RP108 SCK
    TRISFbits.TRISF13 = 0; // RF13-RP109 MOSI
    
    // Remapping the pin os sck mosi and miso into the reprogrammable pins 
    // RPI17 RP108 RP109
    
    // Periferal pin select input Register 
    RPINR20bits.SDI1R = 0b0010001;  // MISO (SDI1)-RPI17 Assign SPI1 data input 
                                    //to the corresponding RPI17 
    
    // Peripheral pin select output Regiters
    RPOR12bits.RP109R = 0b000101; // MOSI (SDO1) - RF13 - RP109;
    RPOR11bits.RP108R = 0b000110; // SCK1 - RF12 - RP108;
}

void mappingCHIPset(){
    
    //TRISBbits.TRISB3 = 0;   // accelerometer set as output
    //TRISBbits.TRISB4 = 0;   // gyroscope set as output 
    TRISDbits.TRISD6 = 0;   // magnetometer set as output

    CS1 = 1;        // accelerometer is inactive
    CS2 = 1;        // gyroscope is inactive
    CS3 = 1;        // magnetometer is inactive

    // This function must be initialized, if not the behaviour can go crazy
    // this function will be inside the init SPI
}

/**
 * First thing to read the register we have to pass to sleep mode, in the case 
 * of the magnetometer we have the power control bit in the register 0x4B
 * At the beginning, the device is in SUSPEND mode. To pass in SLEEP mode
 * the register 0x4B must have the "power bit" set to 1. All the other bits can be 
 * zero, hence you can write 0x01 to the register
 */
void setACTIVEmode(char type){
    switch(type){
        //case 'A':
        //    break;
        
        //case 'G':
        //    break;
        
        case 'M':
            CS3 = 0;
            // Pass to SLEEP mode
            spi_write(0x4B | 0x80); // register where is the power bit
            spi_write(0x01);        // write in that bit 
            CS3 = 1;
            
            tmr_wait_ms(TIMER2, 2);
            
            CS3 = 0;
            //Pass to ACTIVE mode
            spi_write(0x4C | 0x80); // register where is the Opmode
            spi_write(0x00);        // write in that register to pass in active
            CS3 = 1;
            break;
        
        default:
            break;
            
    }
}

// Exercise n.2
int main() {
    
    initializeIO();
    initUART();
    initSPI();
    mappingCHIPset();
    
    setACTIVEmode('M');
    
    int16_t mag_LSB,mag_MSB,value;
    
    char buffer[20];
    
    while(1){
        CS3 = 0;
        
        // x-axis of the magnetometer register 0x42 and 0x43
        spi_write(0x42 | 0x80);     // access to the register
        mag_LSB = spi_write(0x00);  // read the value of the LSB
        mag_MSB = spi_write(0x00);  // read the value of the MSB
        
        // put the LSB and MSB together in a two byte signed integer
        
        mag_LSB = mag_LSB & 0b11111000; // clear the three least significant 
                                        // bits by ANDing with a mask
        
        mag_MSB = mag_MSB << 8;     // left-shift by 8 the MSB
        // left-shifted EXAMPLE: a = 5 binary 101; b = 2; 
        //a << b = 10100 so a<<b = 20 (not in binary)
        
        value = mag_MSB | mag_LSB;  // and OR with the previosly found masked
        value = value/8;            // divided by 8 to get the correct scale
        
        sprintf(buffer, "$MAGX= %d* \0", (int16_t)value);
        for (int i = 0;buffer[i] != '\0'; i++)
        {
            while(U1STAbits.UTXBF == 1);                
            U1TXREG = buffer[i];
            
        }
        
        CS3 = 0;
        tmr_wait_ms(TIMER1,1000);
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
