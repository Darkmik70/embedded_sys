/*
 * File:   newmainXC16.c
 * Author: RomiC
 *  ESERCIZIO PILOTA, guisto per capire come funzionano gli SPI
 * Created on 26 aprile 2024, 11.25
 */


#include "xc.h"
#include "function.h"
#include "time.h"
#include "uart.h"
#include "timer.h"


// This are the accelerometer, gyroscope and magnetometer chipset (in order)
#define CS1 LATBbits.LATB3
#define CS2 LATBbits.LATB4
#define CS3 LATDbits.LATD6


/**
 * Common notion (we need to know the register buffer and what they are doing
 * SPIRBF tells if the receiver buffer is full
 * SPITBF tells if the transmit buffer is full
 * MSB is the Most Significant Bit
 * 
 * The idle value for the clock is 1. A singngle byte exchange is perform by 
 * setting the MSB (R/W) to 1 for a read, 0 for a write operation. The other 
 * bits inside the byte sent to the IMU represent the address.
 * EXAMPLE: in a read operation, the MCU must send 0x00 to enable the generation
 *          of the clock to get the answer, AFTER sending the address.
 * EXAMPLE: in a write operation, the MCU must send the value to be written after
 *          the address
 */

void initSPI(){
    // Set pins as input or output then remap the pins
    TRISAbits.TRISA1 = 1; // RA1?RPI17 MISO
    TRISFbits.TRISF12 = 0; //RF12?RP108 SCK
    TRISFbits.TRISF13 = 0; // RF13?RP109 MOSI
    
    // Remapping the pin os sck mosi and miso into the reprogrammable pins 
    // RPI17 RP108 RP109
    
    // Periferal pin select input Register 
    RPINR20bits.SDI1R = 0b0010001;  // MISO (SDI1) ? RPI17 Assign SPI1 data input 
                                    //to the corresponding RPI17 
    
    // Peripheral pin select output Regiters
    RPOR12bits.RP109R = 0b000101; // MOSI (SDO1) ? RF13 - RP109;
    RPOR11bits.RP108R = 0b000110; // SCK1 - RF12 - RP108;
    
    
    // Control Register
    SPI1CON1bits.MSTEN = 1;     // enable the master mode
    SPI1CON1bits.MODE16 = 0;    // Comunication is byte-wide (8-bit)
    SPI1CON1bits.PPRE = 2;      // Primary prescaler (CERCARE PERCHè SCEGLIERE QUESTO)
    SPI1CON1bits.SPRE = 6;      // Secondary prescaler
    //SPI1CON1bits.CKP specifies the IDLE value of the clock (high)
    
    SPI1STATbits.SPIEN = 1;     // Enable the SPI peripheral
    
}

void mappingCHIPset(){
    
    TRISBbits.TRISB3 = 0;   // accelerometer set as output
    TRISBbits.TRISB4 = 0;   // gyroscope set as output 
    TRISDbits.TRISD6 = 0;   // magnetometer set as output

    CS1 = 1;        // accelerometer is inactive
    CS2 = 1;        // gyroscope is inactive
    CS3 = 1;        // magnetometer is inactive

    
    // This function must be initialized, if not the behaviour can go crazy
    // this function then will be inside the init SPI
}

void spi_write(unsigned int addr){
 
    while(SPI1STATbits.SPITBF == 1);
    SPI1BUF = addr;
    while(SPI1STATbits.SPIRBF == 0);
    int trash = SPI1BUF;
    
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
            spi_write(0x4B);    // register where is the power bit
            spi_write(0x01);    // write in that bit 
            
            tmr_wait_ms(TIMER1, 2);
            
            //Pass to ACTIVE mode
            spi_write(0x4C);    // register where is the Opmode
            spi_write(0x00);    // write in that register to pass in active
            
            CS3 = 1;
            break;
        
        default:
            break;
            
    }
}

/**
 * We want to write in the buffer 
 * RESULT: Is not working why? I dont know D:
 */
void es_pilot_1(){
    
    int data;
    CS3 = 0;        // this is to use this slave, first we have to activate the clock
    while(SPI1STATbits.SPITBF == 1);    // wait until the TX buffer is not full
    SPI1BUF = 22;                      // we write on the buffer the character x
    
    while(SPI1STATbits.SPIRBF == 0);    // wait until the data arrived
    data = SPI1BUF;
    
    sendIntAsChar(data);        // We use this only to see what is is the buffer
    CS3 = 1;        // magnetometer is inactive
}

/**
 * We want to read one register in this case the magnetometer
 * RESULT: it gives me 255 that in hex is 32 35 35 this is because when reading,
 * the IMU will automatically read the next consecutive register. So we can send
 * an addres, then send 0x00 how many times we want, and read all the registers 
 * starting from the addressed one.
 */
void es_pilot_2(){
    
    int read_addr = 0x40;   // this is the address of the magnetometer 
    CS3 = 0;        // Activate the slave magnetometer
    
    while(SPI1STATbits.SPITBF == 1);    // wait until the TX buffer is not full
    SPI1BUF = read_addr|0x80;           // read this address reading bit by bit
    
    while(SPI1STATbits.SPIRBF == 0);    // wait until the data arrived
    int trash = SPI1BUF;                // read to prevent buffer overrun
    
    while(SPI1STATbits.SPITBF == 1);    // wait until the TX buffer is not full
    SPI1BUF = 0x00;                     // colocking out zeros so that the other chip can send the value
    
    while(SPI1STATbits.SPIRBF == 0);
    int value_from_chip =  SPI1BUF;
    
    sendIntAsChar(value_from_chip);
    
    CS3 = 0;    
}


int main(void) {
    
    initializeIO();
    initUART();
    initSPI();
    mappingCHIPset();
    
    setACTIVEmode('M');
    
    while(1){
        es_pilot_2();
        tmr_wait_ms(TIMER1, 1000);
        toggleLed(1);
    }
    
    return 0;
}
