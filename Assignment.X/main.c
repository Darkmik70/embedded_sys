/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on April 27, 2024, 5:42 PM
 */

#include "xc.h"
#include "function.h"
#include "timer.h"
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SPI_READ 4
#define UART_SEND 20

int itr;
char uart_msg[80];
char uart_buff[80];

int is_msg_ready; // Flag to create a new uart_msg


/**
 * Use to simulate an algorithm that takes 7 ms to be completed
 * @param None
 * @return None
 */
void algorithm(){
    tmr_setup_period(TIMER4, 7);
}


double get_magnetic_north(int x_avg, int y_avg) {
    double north;
    north = atan2(y_avg, x_avg);
    north = north * (180.0 / M_PI);
    return north;
}


int read_axis(char type){
    int16_t mag_LSB,mag_MSB,value;  
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

    CS3 = 1;
    return (int)value;
}

void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(){
    IFS1bits.INT1IF = 0;    // clear the interrupt flag
    // uart write
    if (uart_msg[itr] == '\0') {
        // message is finished, zero all values and set flag to 0
        itr = 0;
        is_msg_ready = 0;
        U1TXREG = 'd'; // Test if we are always getting null
    }
    else  {
        // send one char and increment iterator
        U1TXREG = uart_msg[itr];
        itr++;
    }

    toggleLed(1);
}

int assignment() {
    int uart_cnt = 0;   // counter to write on uart
    int spi_cnt = 0;    // counter to read from spi
    int spi_itr = 0;    // iterator to fill buffers

    int buffer_x[5] = {0};
    int buffer_y[5] = {0};
    int buffer_z[5] = {0};
    int x_avg = 0, y_avg = 0, z_avg =0;
    
    uart_msg[0] = 'a';      //  initialize message for uart with some value
    itr = 0;                // init iterator for uart_msg
    is_msg_ready = 0;       // Set the flag to prepare uart messages


    tmr_setup_period(TIMER1,10);        // 10 hz 
    
    while(1){
        algorithm();

        /* read from spi */
        if(spi_cnt == SPI_READ){       // triggers every 40ms
            // check if itr reached its max value
            if (spi_itr == 5) {
                spi_itr = 0;
            }
            // fill up buffers
            buffer_x[spi_itr] = read_axis('x');
            buffer_y[spi_itr] = read_axis('y');
            buffer_z[spi_itr] = read_axis('z');
            spi_itr++;

            spi_cnt = 0;
        }

        /* prepare message for uart */
        if(uart_cnt == UART_SEND && is_msg_ready == 0){      //ideally triggers every 200 ms
            //calculate MAG and YAW
            x_avg = (buffer_x[0] + buffer_x[1] + buffer_x[2] + buffer_x[3] + buffer_x[4]) / 5.0;
            y_avg = (buffer_y[0] + buffer_y[1] + buffer_y[2] + buffer_y[3] + buffer_y[4]) / 5.0;
            z_avg = (buffer_z[0] + buffer_z[1] + buffer_z[2] + buffer_z[3] + buffer_z[4]) / 5.0;
            int north = get_magnetic_north(x_avg,y_avg);
            // set the message        
             memset(uart_msg, 0, sizeof(uart_msg));
             sprintf(uart_msg, "$MAG,%d,%d,%d* $YAW,%d", x_avg, y_avg, z_avg, north);
            
            // allow for sending message
            is_msg_ready = 1;
            uart_cnt = 0;
        }

        //increment counters
        spi_cnt++;
        uart_cnt++;

        // check whether deadline is missed 
        while(is_msg_ready == 1 && get_timer_status(TIMER1) == 0) {
            // trigger the interrupt to write on uart
            IFS1bits.INT1IF = 1;
        }
        // wait for the remainder of time;
        tmr_wait_period(TIMER1);
    }
    return 0;
}


int main(){
    
    initializeIO();
    initUART();
    initSPI();
        
    INTCON2bits.GIE = 1;    // set global interrupt enable
    IFS1bits.INT1IF = 0;    // clear the interrupt flag
    IEC1bits.INT1IE = 1;    // enable interrupt

    // Pass from the suspend mode of the magnetometer to the sleep and then 
    // to the active mode
    setACTIVEmode('M');
    
    // 25Hz rate of the magnetometer 
    spi_write(0x4C);
    spi_write(0b00110000);
    
    assignment();
}

