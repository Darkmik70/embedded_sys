/*
 * File:   main.c
 * Author: aless
 *
 * Created on April 18, 2024, 3:21 PM
 */


//#include "xc.h"
//#include "functions.h"
//#include "timer.h"
//#include "uart.h"

//void setADC() {
//    AD1CON3bits.ADCS = 8; // Setting ADC time
//    AD1CON1bits.ASAM = 0; // Manual sampling start
//    AD1CON1bits.SSRC = 7; // Automatic sampling end
//    AD1CON3bits.SAMC = 16; // Sampling period
//    AD1CON2bits.CHPS = 00; // Number of channel used
//    AD1CHS0bits.CH0SA = 5; // CH0 positive input is AN11
//    //AD1CHS123bits.CH123SA = 1;  // CH3 positive input is AN5
//    initializeIO();
//    ANSELBbits.ANSB11 = 1;
//    TRISBbits.TRISB11 = 1;
//    AD1CON1bits.ADON = 1;
//}
//
//int main(void) {
//    char out_msg[50];
//    int dist_value;
//    set_UART1();
//    setADC();
//    
//    while (1) {
//
//    }
//    return 0;
//}

#include "xc.h"
#include "functions.h"
#include "timer.h"
#include "uart.h"
#include <stdio.h>

#define VDD 3.3 // Voltage supplied to the ADC
#define ADC_RESOLUTION 1024 // ADC 10-bit resolution

void setADC() {
    AD1CON3bits.ADCS = 8; // Setting ADC conversion clock
    AD1CON1bits.ASAM = 0; // Manual sampling start
    AD1CON1bits.SSRC = 7; // Manual sampling end
    AD1CON3bits.SAMC = 16; // Sampling period
    AD1CON2bits.CHPS = 0; // Number of channel used (1 channel)
    AD1CHS0bits.CH0SA = 11; // CH0 positive input is AN11
    initializeIO();
    ANSELBbits.ANSB11 = 1; // Set RB11 (AN11) as analog input
    TRISBbits.TRISB11 = 1; // Set RB11 as input
    TRISBbits.TRISB4 = 0;
    
    AD1CON1bits.ADON = 1; // Turn on ADC
}

/***

 */
float convertTo(int adc_value, char type) {
    float value = ((float)adc_value / ADC_RESOLUTION) * VDD; // Convert ADC value to voltage
    
    switch (type){
        case 'V':
            int voltage = value*3; //  voltage divider
            return voltage;    
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
    double battery_voltage;
    
    set_UART1();
    setADC();
    
    
//    tmr_wait_ms(TIMER2,1);
//    AD1CON1bits.SAMP = 0;
    
    while (1) {
        AD1CON1bits.DONE = 0;
        
        AD1CON1bits.SAMP = 1; // Start sampling
        // Start ADC conversion
        while (!AD1CON1bits.DONE); // Wait for conversion to complete
        
        // Read ADC value
        adc_value = ADC1BUF0; 
        
        // Convert ADC value to voltage
        battery_voltage = convertToVoltage(adc_value);
        
        // Send voltage value over UART
        sprintf(out_msg, "Battery Voltage: %.2f V", battery_voltage);
        
        // Send to UART
        UART1_WriteString(out_msg);
        
        // Wait for some time before the next conversion
        tmr_wait_ms(TIMER1, 1000); 
    }
    return 0;
}


