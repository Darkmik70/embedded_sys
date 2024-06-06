/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on April 18, 2024, 1:04 PM
 */


#include "xc.h"
#include "function.h"
#include "timer.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define VDD 3.3     // voltage supplied to the ADC
#define ADC_RESOLUTION 1024 // ADC 10-bit resolution


/**
 * Function that convert the corrente? into the voltage or ... 
 * @param adc_value: value of the corrente?
 * @param type: what we want to compute, it could be voltage or temperature etc.
 * @return voltage| ...
 */
float convertTo(int adc_value, char type) {
    float value = ((float)adc_value / ADC_RESOLUTION) * VDD; // Convert ADC value to voltage
    
    switch(type){
        case'V':
            return value*3;     // Voltage divider
        case'M':
            return 2.34 - 4.74*value + 4.06*pow(value,2) -1.60*pow(value,3) + 0.24*pow(value,4);
        default:
            break;
    }
    return 0;
}

/* Exercise n.3
 * Plug the IR Distance Sensor and the RS232 UART shield in the MIKRObus sockets
 * of the Clicker board. Read both values using scan mode automatic sampling and
 * conversion at 1 kHz. Send the values to the UART at 10 Hz using $SENS, x,y*
 * where x is the IR in cm, and y is the battery voltage in Volt
 */
void remapPIN(){
    ANSELBbits.ANSB11 = 1;  // battery pin analog
    ANSELBbits.ANSB14 = 1;   // Proximity sensor pin analog
    TRISBbits.TRISB9 = 0;   // set RB9 as output, sensor of proximity
}

void initADC(){
    
    remapPIN();
    AD1CON3bits.ADCS = 8;   // setting ADC conversion clock 
    AD1CON1bits.ASAM = 1;   // automatic sampling 
    AD1CON3bits.SAMC = 16;  // sampling period 16 Tad
    AD1CON1bits.SSRC = 7;   // automatic sampling, when the conversion should start  
    AD1CON2bits.CHPS = 0;   // number of channels used (1 channel)
    AD1CON2bits.CSCNA = 1;  // scan mode activated
    //AD1CON1bits.SIMSAM = 0; // Sample in sequence
    AD1CON2bits.SMPI = 1;   // Two convertion before the interrupt
    
    AD1CSSLbits.CSS11 = 1;
    AD1CSSLbits.CSS14 = 1;
    
    AD1CON1bits.ADON = 1;   // turn on the ADC module
    
}


int main(void) {
    
    initializeIO();
    initUART();
    initADC();
    
    char buffer[50];
    int adc_IRsensor, adc_battery;
    float cm, battery_voltage;
    
    LATBbits.LATB9 = 1;     // Put the pin RB9 oto high
    
    while (1) {
        AD1CON1bits.DONE = 0;
        
        // Start ADC conversion
        while (!AD1CON1bits.DONE); // Wait for conversion to complete
        
        // Read ADC value
        adc_battery = ADC1BUF0;
        adc_IRsensor = ADC1BUF1;
        
        // Convert ADC value to distance
        cm = convertTo(adc_IRsensor,'M')*100;   // *100 because 
        
        // Convert ADC value to voltage
        battery_voltage = convertTo(adc_battery,'V');
        
        // Send voltage value over UART
        sprintf(buffer, "$SENS,%.2f,%.2f*", cm, battery_voltage);
        for (int j = 0; buffer[j] != '\0'; j++) {
            while (U1STAbits.UTXBF == 1);
            U1TXREG = buffer[j];
        }
       
        // Wait for some time before the next conversion
        tmr_wait_ms(TIMER1, 1000); 
    }
    return 0;
}

/* Exercise n.2
 * Plug the IR Distance Sensro and the RS232 UART shield in the MIKRObus sockets
 * of the Clicker board. Read the value of the distance sensor and send it on the 
 * UART. Use manual sampling and automatic conversion
 */

//void remapPIN(){
//    //ANSELBbits.ANSB11 = 1;  // battery pin analog
//    ANSELBbits.ANSB14 = 1;   // Proximity sensor pin analog
//    TRISBbits.TRISB9 = 0;   // set RB9 as output, sensor of proximity
//}
//
//void initADC(){  
//    remapPIN();
//    AD1CON3bits.ADCS = 8;   // setting ADC conversion clock 
//    AD1CON1bits.ASAM = 0;   // manual sampling start
//    AD1CON1bits.SSRC = 7;   // automatic convertion start
//    AD1CON3bits.SAMC = 16;  // Before how many Tad start the conversion
//    AD1CON2bits.CHPS = 0;   // number of channels used (1 channel)
//    AD1CHS0bits.CH0SA = 14; // CH0 positive input is AN14
//    AD1CON1bits.ADON = 1;   // Turn on ADC   
//}
//
//int main(void) {
//    
//    initializeIO();
//    initUART();
//    initADC();
//    
//    char buffer[50];
//    int adc_value;
//    float meter;
//    
//    LATBbits.LATB9 = 1;     // Put the pin RB9 oto high (DA RICORDARE))
//    
//    while (1) {
//        AD1CON1bits.DONE = 0;
//        
//        AD1CON1bits.SAMP = 1; // Start sampling
//        // Start ADC conversion
//        while (!AD1CON1bits.DONE); // Wait for conversion to complete
//        
//        // Read ADC value
//        adc_value = ADC1BUF0; 
//        
//        // Convert ADC value to voltage
//        meter = convertTo(adc_value,'M');
//        
//        // Send voltage value over UART
//        sprintf(buffer, "How far: %.2f m", meter);
//        for (int j = 0; buffer[j] != '\0'; j++) {
//            while (U1STAbits.UTXBF == 1);
//            U1TXREG = buffer[j];
//        }
//       
//        // Wait for some time before the next conversion
//        tmr_wait_ms(TIMER1, 1000); 
//    }
//    return 0;
//}

/* Exercise n.1
 * Read the value of the battery, considering the voltage divider circuit, and 
 * send it on the UART. Use manual sampling and manual conversion
 */

//void remapPIN(){
//    ANSELBbits.ANSB11 = 1;  // set RB11 (AN11) as analog input battery
//}

//void initADC(){
//    
//    remapPIN();
//    AD1CON3bits.ADCS = 8;   // setting ADC conversion clock 
//    AD1CON1bits.ASAM = 0;   // manual sampling start
//    AD1CON1bits.SSRC = 0;   // manual sampling end
//    AD1CON2bits.CHPS = 0;   // number of channels used (1 channel)
//    AD1CHS0bits.CH0SA = 11; // CH0 positive input is AN11
//    AD1CON1bits.ADON = 1;   // Turn on ADC
//    
//}



//int main(void){
//    initializeIO(); // We don't put the ANSELB, doesn't work if i put it 
//    initUART();     // Initialize the UART
//    initADC();      // Initialize the ADC
//    int adc_value;
//    float battery_voltage;
//    
//    while(1){
//        AD1CON1bits.DONE = 0;   // Tell us if the A/D convertion is done
//        
//        AD1CON1bits.SAMP = 1;   // Sample enable bit
//        tmr_wait_ms(TIMER1,1);
//        AD1CON1bits.SAMP = 0;   // Stop sampling
//        
//        while(!AD1CON1bits.DONE);
//        adc_value = ADC1BUF0;       // Take the value of the ADC
//        
//        battery_voltage = convertTo(adc_value,'V');
//        
//        // Send voltage value over UART
//        sprintf(buffer, "Battery Voltage: %.4f V", battery_voltage);
//        for (int j = 0; buffer[j] != '\0'; j++) {
//            while (U1STAbits.UTXBF == 1);
//            U1TXREG = buffer[j];
//        }
//       
//        // Wait for some time before the next conversion
//        tmr_wait_ms(TIMER1, 1000); 
//    }
//}