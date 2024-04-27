/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on April 27, 2024, 5:42 PM
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"
#include <math.h>

double magneticNORTH(unsigned int x_avg, unsigned int y_avg){
    double north;
    north = atan2(y_avg,x_avg);
    return north;
}

int axis(char type){
    int16_t mag_LSB,mag_MSB,value;
    //char buffer[20];
    
    CS3 = 0;
    
    switch(type){
        case 'x':
            spi_write(0x42 | 0x80);     // access to the register
            break;
        case 'y':
            spi_write(0x44 | 0x80);     // access to the register
            break;
        case 'z':
            spi_write(0x46 | 0x80);
            break;
    }
    mag_LSB = spi_write(0x00); // read the value of the LSB
    mag_MSB = spi_write(0x00); // read the value of the MSB

    // put the LSB and MSB together in a two byte signed integer

    mag_LSB = mag_LSB & 0b11111000; // clear the three least significant 
    // bits by ANDing with a mask

    mag_MSB = mag_MSB << 8; // left-shift by 8 the MSB

    value = mag_MSB | mag_LSB; // and OR with the previously found masked
    value = value / 8; // divided by 8 to get the correct scale

    //sprintf(buffer, "$MAGX= %d* \0", (int16_t) value);
    //for (int i = 0; buffer[i] != '\0'; i++) {
    //    while (U1STAbits.UTXBF == 1);
    //    U1TXREG = buffer[i];
    //}

    CS3 = 1;
    tmr_wait_ms(TIMER1, 200);
    return (int)value;
}


int main() {
    
    initializeIO();
    initUART();
    initSPI();
  
    // Pass from the suspend mode of the magnetometer to the sleep and then 
    // to the active mode
    setACTIVEmode('M');
    
    //int16_t x,y,z;
    int16_t x_avg,y_avg,z_avg;
    int16_t north;
    
    char buffer[50];
    
    while(1){
        
        x_avg = 0;
        y_avg = 0;
        z_avg = 0;
        for(int p = 0; p<5; p++){
            x_avg = axis('x') + x_avg;
            y_avg = axis('y') + y_avg;
            z_avg = axis('z') + z_avg;
        }
        
        x_avg = x_avg/5;
        y_avg = y_avg/5;
        z_avg = z_avg/5;
        
        sprintf(buffer, "$VALORE %d, %d, %d  \0", (int16_t)x_avg,(int16_t)y_avg,(int16_t)x_avg);
        for (int i = 0; buffer[i] != '\0'; i++) {
            while (U1STAbits.UTXBF == 1);
            U1TXREG = buffer[i];
        }
        
        north = magneticNORTH(x_avg,y_avg);
        north = (int)north * (180.0/ M_PI);
        sprintf(buffer, "$YAW, %d* \0", (int)north);
        for (int j = 0; buffer[j] != '\0'; j++) {
            while (U1STAbits.UTXBF == 1);
            U1TXREG = buffer[j];
        }
        
    }
     
    return 0;
}