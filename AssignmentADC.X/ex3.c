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
#include "math.h"

#define VDD 3.3 // Voltage supplied to the ADC
#define ADC_RESOLUTION 1024 // ADC 10-bit resolution

void remapPIN() {
    ANSELBbits.ANSB11 = 1; // AN11 is analogic pin of IR sensor
    ANSELBbits.ANSB14 = 1; // AN14 is analogic pin of battery
    TRISBbits.TRISB9 = 0; // Set RB11 as output
}
void setADC() {
    AD1CON3bits.ADCS = 8;
    AD1CON1bits.ASAM = 1; // Automatic sampling start
    AD1CON3bits.SAMC = 16; // Sampling period     (ONLY FOR AUTOMATIC SAMPLING)
    AD1CON1bits.SSRC = 7; // Automatic conversion start
    AD1CON2bits.CHPS = 0; // 1-channel
    AD1CON2bits.CSCNA = 1; // scan mode
    AD1CON2bits.SMPI = 1; // 2 conversion between interrupt
    
    AD1CSSLbits.CSS11 = 1; // AN11 for battery
    AD1CSSLbits.CSS14 = 1; // AN14 for IR sensor
    
    AD1CON1bits.ADON = 1; // Turn on ADC
}

/***

 */
float convertTo(int adc_value, char type) {
    float value = ((float) adc_value / ADC_RESOLUTION) * VDD; // Convert ADC value to voltage
    float meter;
    
    switch (type) {
        case 'V': // convert to Voltage
            return value*3;
        case 'M': // convert to Meters
            meter = 2.34-4.74*pow(value,1)+4.06*pow(value,2)-1.60*pow(value,3)+0.24*pow(value,4);
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
    int adc_battery, adc_IRsensor;
    float battery_voltage, cm;
    
    initializeIO();
    remapPIN();
    set_UART1();
    setADC();

    LATBbits.LATB9 = 1; // set EN pin high to work properly

    while (1) {
        AD1CON1bits.DONE = 0;
        
        while (!AD1CON1bits.DONE); // Wait for conversion to complete

        // Read ADC value
        adc_battery = ADC1BUF0; // AN11
        adc_IRsensor = ADC1BUF1; // AN14
        
        // Convert ADC value to voltage
        battery_voltage = convertTo(adc_battery, 'V');
        
        // Convert ADC value to meter
        cm = convertTo(adc_IRsensor, 'M')*100;

        // Send voltage value over UART
        sprintf(out_msg, "$SENS,%.2fcm,%.2fv*", cm, battery_voltage);

        UART1_WriteString(out_msg);

        // Wait for some time before the next conversion
        tmr_wait_ms(TIMER1, 1000);
    }
    return 0;
}