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
    ANSELBbits.ANSB11 = 1; // AN11 analog pin for battery
}
void setADC() {
    AD1CON3bits.ADCS = 8; // Setting ADC conversion clock
    AD1CON1bits.ASAM = 0; // Manual sampling start
    AD1CON1bits.SSRC = 0; // Manual sampling end
    //AD1CON3bits.SAMC = 16; // Sampling period     (ONLY FOR AUTOMATIC SAMPLING)
    AD1CON2bits.CHPS = 0; // Number of channel used (1 channel)
    AD1CHS0bits.CH0SA = 0b1011; // CH0 positive input is AN11

    AD1CON1bits.ADON = 1; // Turn on ADC
}

/***

 */
float convertTo(int adc_value, char type) {
    float value = ((float) adc_value / ADC_RESOLUTION) * VDD; // Convert ADC value to voltage
    float v;
    float meter;

    v = value/3; //  voltage divider
    switch (type) {
        case 'V': // convert to Voltage
            return v;
        case 'M': // convert to Meters
            meter = 2,34-4,74*v+4,06*v*v-1,60*v*v*v+0,24*v*v*v*v;
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
        battery_voltage = convertTo(adc_value, 'V');

        // Send voltage value over UART
        sprintf(out_msg, "Battery Voltage: %.4f V", battery_voltage);
        
        // Convert ADC value to meter
        //meter = convertTo(adc_value, 'M');

        // Send voltage value over UART
        //sprintf(out_msg, "Meter: %.2f m", meter);

        // Send to UART
        UART1_WriteString(out_msg); 

        // Wait for some time before the next conversion
        tmr_wait_ms(TIMER1, 1000);
    }
    return 0;
}

//void es1(){
//    
//    // Configure analog and digital pins
//    ANSELA = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
//    ANSELBbits.ANSB0 = 1; // Set ANSELB0 to analog mode
//    
//    // Set up the UART
//    RPOR0bits.RP64R = 1;
//    RPINR18bits.U1RXR = 0x4b;    
//    U1BRG = 468; // Baud rate for 9600 bps at 72 MHz
//    U1MODEbits.UARTEN = 1; // Enable UART
//    U1STAbits.UTXEN = 1; // Enable UART TX
//
//    TRISBbits.TRISB4 = 0; // Set RB11 as input
//    
//    // Configure ADC
//    AD1CON1bits.ADON = 0;
//    
//    AD1CON3bits.ADCS = 8; // Tad = 8*(ADCS + 1)*(Tpb)
//    AD1CON1bits.ASAM = 0; // Enable Manual sampling                     -               MANUAL START
//    AD1CON1bits.SSRC = 0; // Enable Manual conversion                   -               MANUAL END
//    AD1CON2bits.CHPS = 0; // Select CH0 for sampling                    -               SELECT THE CHANNELS
//    AD1CHS0bits.CH0SA = 0b1011;                                                         // Select AN11 for CH0 positive input
//    
//    AD1CON1bits.ADON = 1; // Turn on ADC module
//    
//    char toPrint[20];
//    while(1) {
//        
//        tmr_wait_ms(TIMER1, 1000);
//        
//        AD1CON1bits.SAMP = 1; // Start sampling
//        tmr_wait_ms(TIMER1, 1); // Sampling time
//        AD1CON1bits.SAMP = 0; // Stop sampling, start conversion
//        
//        while (!AD1CON1bits.DONE); // Wait for conversion to complete
//        int value = ADC1BUF0; // Read ADC buffer
//        AD1CON1bits.DONE = 0;
//        
//        float value_f = ((float)value *5 ) / 1024;
//        value_f /= 3;       
//        
//        sprintf(toPrint, "$V=%f  \0", value_f);
//        for (int i = 0;toPrint[i] != '\0'; i++)
//        {
//            while(U1STAbits.UTXBF);                
//            U1TXREG = toPrint[i];
//
//        }       
//    }
//   
//}
//
//int main(void) {
//    es1();
//}




