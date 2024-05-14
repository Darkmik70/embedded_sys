/*
 * File:   main.c
 * Author:  Bua Odetti, Contreras, Krepa, Trovatello
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

// Define constants for SPI_READ and UART_SEND
#define SPI_READ 4
#define UART_SEND 20

// Declare global variables
int itr;
char uart_msg[50];
char uart_buffer[50];
int is_msg_ready; // Flag to create a new uart_msg

/**
 * Interrupt service routine for UART1 TX
 * @param None
 * @return None
 */
void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt()
{
    IFS0bits.U1TXIF = 0; // clear the interrupt flag
    // if the end of the message is reached
    if (uart_msg[itr] == '\0')
    {
        itr = 0;                               // reset the iterator
        memset(uart_msg, 0, sizeof(uart_msg)); // clear the message
        is_msg_ready = 0;                      // reset the flag's message
    }
    else
    {
        U1TXREG = uart_msg[itr]; // send one character
        itr++;                   // increment the iterator
    }
}

/**
 * Read the axis from the relative register, based on the type
 * @param type Axis type ('x', 'y', 'z')
 * @return Axis value
 */
double read_axis(char type)
{
    int16_t mag_LSB, mag_MSB, value;

    // START the communication with the magnetometer
    CS3 = 0;

    // select the register to read the value of the magnetometer's axis
    switch (type)
    {
    case 'x':
        spi_write(0x42 | 0x80); // access to the 0x42 register
                                // relative to the LSB of the magnetometer's x-axis
        break;
    case 'y':
        spi_write(0x44 | 0x80); // access to the 0x44 register
                                // relative to the LSB of the magnetometer's y-axis
        break;
    case 'z':
        spi_write(0x46 | 0x80); // access to the 0x46 register
                                // relative to the LSB of the magnetometer's z-axis
        break;
    }

    // read the value of the LSB and MSB from the registers previously selected
    mag_LSB = spi_write(0x00); // read the value of the LSB
    mag_MSB = spi_write(0x00); // read the value of the MSB

    // check if the type is 'z' to use a different mask w.r.t. 'x' and 'y' cases
    if (type == 'z')
    {
        // put the LSB and MSB together in a two byte signed integer
        mag_LSB = mag_LSB & 0b11111110; // clear the last least significant
                                        // bits by ANDing with a mask
    }
    // if the type is 'x' or 'y' use the following mask
    else
    {
        // put the LSB and MSB together in a two byte signed integer
        mag_LSB = mag_LSB & 0b11111000; // clear the three least significant
                                        // bits by ANDing with a mask
    }

    mag_MSB = mag_MSB << 8;    // left-shift by 8 the MSB
    value = mag_MSB | mag_LSB; // and OR with the previously found masked

    if (type == 'z')
    {
        value = value / 2; // divided by [2^(number of bits of no-data)] = (2^1)
                           // because the z-axis's value is 15 bits long
    }
    else
    {
        value = value / 8; // divided by [2^(number of bits of no-data)] = (2^3)
                           // because the x-axis and y-axis's value is 13 bits long
    }

    // END the communication with the magnetometer
    CS3 = 1;

    return value;
}

/**
 * Calculate the magnetic north based on the average x and y values
 * @param x_avg Average x value
 * @param y_avg Average y value
 * @return Magnetic north
 */
double get_magnetic_north(double x_avg, double y_avg)
{
    double north;

    north = atan2(y_avg, x_avg);
    north = north * (180.0 / M_PI);
    return north;
}

/**
 * Use to simulate an algorithm that takes 7 ms to be completed
 * @param None
 * @return None
 */
void algorithm()
{
    tmr_wait_ms(TIMER4, 7);
}

/**
 * Main function
 * @param None
 * @return int
 */
int main()
{

    int uart_cnt = 0; // counter to write on uart
    int spi_cnt = 0;  // counter to read from spi
    int spi_itr = 0;  // iterator to fill buffers

    // initialize buffers and average values
    double buffer_x[5] = {0};
    double buffer_y[5] = {0};
    double buffer_z[5] = {0};
    double x_avg = 0, y_avg = 0, z_avg = 0;

    initIO();
    initUART();
    initSPI();

    INTCON2bits.GIE = 1; // set global interrupt enable

    // Pass from the suspend mode of the magnetometer to the sleep mode
    // and then to the active mode
    setACTIVEmode('M');

    // set the frequency of the magnetometer at 25Hz
    spi_write(0x4C);
    spi_write(0b00110000);

    // initialize the variables
    memset(uart_msg, 0, sizeof(uart_msg));
    memset(uart_buffer, 0, sizeof(uart_buffer));

    itr = 0;          // init iterator for uart_msg
    is_msg_ready = 0; // set the flag to prepare uart messages

    tmr_setup_period(TIMER1, 10, 0); // set the TIMER1 to 100Hz

    while (1)
    {
        algorithm();

        // read from spi every 40ms
        if (spi_cnt == SPI_READ)
        {
            // check if itr reached its max value
            if (spi_itr == 5)
            {
                spi_itr = 0;
            }
            // fill up buffers
            buffer_x[spi_itr] = read_axis('x');
            buffer_y[spi_itr] = read_axis('y');
            buffer_z[spi_itr] = read_axis('z');

            spi_itr++;   // increment iterator
            spi_cnt = 0; // reset counter
        }

        // prepare message for uart every 200ms
        if (uart_cnt == UART_SEND)
        {
            // calculate the MAG average values
            x_avg = (buffer_x[0] + buffer_x[1] + buffer_x[2] + buffer_x[3] + buffer_x[4]) / 5;
            y_avg = (buffer_y[0] + buffer_y[1] + buffer_y[2] + buffer_y[3] + buffer_y[4]) / 5;
            z_avg = (buffer_z[0] + buffer_z[1] + buffer_z[2] + buffer_z[3] + buffer_z[4]) / 5;

            // calculate the YAW value
            double north = get_magnetic_north(x_avg, y_avg);

            // prepare the message
            sprintf(uart_buffer, "$MAG,%.f,%.f,%.f* $YAW,%.2f", x_avg, y_avg, z_avg, north);

            is_msg_ready = 1; // allow for sending message
            uart_cnt = 0;     // reset counter
        }

        // increment counters
        spi_cnt++;
        uart_cnt++;

        // if the message is ready, send it
        if (is_msg_ready == 1)
        {
            if (uart_msg[0] == '\0')
            {
                strcpy(uart_msg, uart_buffer); // copy the message
                itr++;                         // increment the iterator
                U1TXREG = uart_msg[0];         // send the first character
            }
        }

        // check if the TIMER1 has expired
        if (tmr_wait_period(TIMER1) == 1)
        {
            toggleLed(2); // toggle the LED 2
        }
    }
    return 0;
}