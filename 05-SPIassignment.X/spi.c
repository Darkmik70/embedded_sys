/*
 * File:   spi.c
 * Author: RomiC
 *
 * Created on 27 aprile 2024, 16.43
 */


#include "xc.h"
#include "function.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"

/**
 * Function that remap the MOSI MISO and SCK, also configure the master mode, the
 * prescaler and the chipset
 */
void initSPI(){
    
    // Control Register
    SPI1CON1bits.MSTEN = 1;     // enable the master mode
    SPI1CON1bits.MODE16 = 0;    // Comunication is byte-wide (8-bit)
    SPI1CON1bits.PPRE = 3;      // Primary prescaler (CERCARE PERCHè SCEGLIERE QUESTO)
    SPI1CON1bits.SPRE = 0;      // Secondary prescaler
    //SPI1CON1bits.CKP specifies the IDLE value of the clock (high)
    
    SPI1STATbits.SPIEN = 1;     // Enable the SPI peripheral
    
    
    // Set pins as input or output then remap the pins
    TRISAbits.TRISA1 = 1;   // RA1-RPI17 MISO
    TRISFbits.TRISF12 = 0;  //RF12-RP108 SCK
    TRISFbits.TRISF13 = 0;  // RF13-RP109 MOSI
    
    // Remapping the pin os sck mosi and miso into the reprogrammable pins 
    // RPI17 RP108 RP109
    
    // Periferal pin select input Register 
    RPINR20bits.SDI1R = 0b0010001;  // MISO (SDI1)-RPI17 Assign SPI1 data input 
                                    //to the corresponding RPI17 
    
    // Peripheral pin select output Regiters
    RPOR12bits.RP109R = 0b000101; // MOSI (SDO1) - RF13 - RP109;
    RPOR11bits.RP108R = 0b000110; // SCK1 - RF12 - RP108;
    
    // Mapping os the chipset, at first they are all inactive
    TRISBbits.TRISB3 = 0;   // accelerometer set as output
    TRISBbits.TRISB4 = 0;   // gyroscope set as output 
    TRISDbits.TRISD6 = 0;   // magnetometer set as output

    CS1 = 1;        // accelerometer is inactive
    CS2 = 1;        // gyroscope is inactive
    CS3 = 1;        // magnetometer is inactive
}

/**
 * Function that write the address to the SPI and return the value read from the 
 * other device.
 * EXAMPLE: Used to pass from suspend mode to sleep mode and then to active mode, the 
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
 * First thing to read the register we have to pass to sleep mode, in the case 
 * of the magnetometer we have the power control bit in the register 0x4B
 * At the beginning, the device is in SUSPEND mode. To pass in SLEEP mode
 * the register 0x4B must have the "power bit" set to 1. All the other bits can be 
 * zero, hence you can write 0x01 to the register.
 * To pass to the active mode use the register 0x4C for the magnetometer
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