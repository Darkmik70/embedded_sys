/*
 * File:   uart.c
 * Author: emanu
 *
 * Created on 8 giugno 2024, 12.26
 */


#include <xc.h>
#include <stdio.h>
#include "uart.h"

void init_uart() {
    // Configure UART pins and settings
    U1MODEbits.UARTEN = 0; // Disable UART before configuration

    U1MODEbits.BRGH = 0;   // Standard speed mode
    U1BRG = 25;            // Baud rate setting for 9600 bps with Fosc = 16MHz

    U1MODEbits.PDSEL = 0;  // 8-bit data, no parity
    U1MODEbits.STSEL = 0;  // 1 stop bit

    U1STAbits.URXISEL = 0; // Interrupt after one RX character is received
    IEC0bits.U1RXIE = 1;   // Enable UART RX interrupt
    IPC2bits.U1RXIP = 5;   // UART RX interrupt priority

    U1STAbits.UTXEN = 1;   // Enable UART transmitter
    U1MODEbits.UARTEN = 1; // Enable UART
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    // Handle UART receive interrupt
    char received_char = U1RXREG; // Read received character
    // Add character to a buffer or process as needed
    IFS0bits.U1RXIF = 0; // Clear UART RX interrupt flag
}

void send_uart(const char *message) {
    while (*message != '\0') {
        while (U1STAbits.UTXBF); // Wait while UART TX buffer is full
        U1TXREG = *message++;    // Send character
    }
}

void send_battery_status(float v_batt) {
    char buffer[20];
    sprintf(buffer, "$MBATT,%.2f*\r\n", (double)v_batt);
    send_uart(buffer);
}

void send_distance_status(int distance) {
    char buffer[20];
    sprintf(buffer, "$MDIST,%d*\r\n", distance);
    send_uart(buffer);
}

