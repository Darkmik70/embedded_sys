/*
 * File:   main.c
 * Author: aless
 *
 * Created on April 18, 2024, 3:21 PM
 */

#include "xc.h"
#include "functions.h"
#include "timer.h"
#include "uart.h"
#include <stdio.h>

#define VDD 3.3 // Voltage supplied to the ADC
#define ADC_RESOLUTION 1024 // ADC 10-bit resolution

void remapPIN() {
    ANSELBbits.ANSB0 = 1;
    TRISBbits.TRISB4 = 0; // Set RB4 as output
    //TRISBbits.TRISB11 = 1; // Set RB11 as input
}
void setADC() {
    AD1CON3bits.ADCS = 8; // Setting ADC conversion clock
    AD1CON1bits.ASAM = 0; // Manual sampling start
    AD1CON1bits.SSRC = 7; // Automatic conversion start
    AD1CON3bits.SAMC = 16; // Sampling period     (ONLY FOR AUTOMATIC SAMPLING)
    AD1CON2bits.CHPS = 0; // Number of channel used (1 channel)
    AD1CHS0bits.CH0SA = 0b101; // CH0 positive input is AN5
    
    AD1CON1bits.ADON = 1; // Turn on ADC
}

/***

 */
float convertTo(int adc_value, char type) {
    float value = ((float) adc_value / ADC_RESOLUTION) * VDD; // Convert ADC value to voltage
    float v;
    float meter;

    v = value; //  voltage divider
    switch (type) {
        case 'V': // convert to Voltage
            v = value*3;
            return v;
        case 'M': // convert to Meters
            meter = 2.34-4.74*v+4.06*v*v-1.60*v*v*v+0.24*v*v*v*v;
            return meter;
    }
    return 0;
}

void UART1_WriteString(char *str) {
    while (*str != '\0') {
        // Send each character of the string
        while (U1STAbits.UTXBF); // Wait while transmit buffer is full
        U1TXREG = *str; // Put the character to be transmitted into the transmit buffer
        str++; // Move to the next character
    }
}

int main(void) {
    char out_msg[50];
    int adc_value;
    float battery_voltage, meter;
    
    initializeIO();
    remapPIN();
    set_UART1();
    setADC();

    LATBbits.LATB4 = 1; // set EN pin high to work properly

    while (1) {
        AD1CON1bits.DONE = 0;
        AD1CON1bits.SAMP = 1; // Start sampling
        // Start ADC conversion
        tmr_wait_ms(TIMER1, 1);
        AD1CON1bits.SAMP = 0;
        
        while (!AD1CON1bits.DONE); // Wait for conversion to complete

        // Read ADC value
        adc_value = ADC1BUF0;
        
        // Convert ADC value to voltage
        //battery_voltage = convertTo(adc_value, 'V');

        // Send voltage value over UART
        //sprintf(out_msg, "Volt: %.4f V", battery_voltage);
        
        // Convert ADC value to meter
        meter = convertTo(adc_value, 'M');

        // Send voltage value over UART
        sprintf(out_msg, "Meter: %.4f m", meter);

        UART1_WriteString(out_msg);

        // Wait for some time before the next conversion
        tmr_wait_ms(TIMER1, 1000);
    }
    return 0;
}




