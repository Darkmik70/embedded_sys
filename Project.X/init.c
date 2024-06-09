/*
 * File:   init.c
 * Author: RomiC
 *
 * Created on April 28, 2024, 12:36 PM
 */

#include "xc.h"
#include "timer.h"
#include "init.h"
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This function assign the UART into the pin where we read and send, it is
 * also define the baud rate register
 */
void initUART() {
    // assign an interrupt to the corresponding pin bits
    //TRISDbits.TRISD11 = 1;      // set RD11 as input
    RPOR0bits.RP64R = 1; // remap RD0 to RP64
    RPINR18bits.U1RXR = 75; // remap UART1 receive to RD11

    // put in the initialization
    U1BRG = (int) BRGVAL; // baud rate register

    // interrupt procedure 
//    IFS0bits.U1RXIF = 0; // Flag clear Interrupt U1RX
//    IEC0bits.U1RXIE = 1; // Enable the interrupt U1RX
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXISEL1 = 0;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 1;
    
    // enable the UART
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART U1TX (must be after UARTEN)

    U1STAbits.URXISEL = 0; // Flag goes to zero when a char is received
}

void initADC(){
    // Remap the pins
    ANSELBbits.ANSB11 = 1;  // battery pin analog
    ANSELBbits.ANSB14 = 1;  // Proximity sensor pin analog
    TRISBbits.TRISB9 = 0;   // set RB9 as output, sensor of proximity
    
    // Setting the ADC converter
    AD1CON3bits.ADCS = 8;   // setting ADC conversion clock 
    AD1CON1bits.ASAM = 1;   // automatic sampling 
    AD1CON3bits.SAMC = 16;  // sampling period 16 Tad
    AD1CON1bits.SSRC = 7;   // automatic sampling, when the conversion should start  
    AD1CON2bits.CHPS = 0;   // number of channels used (1 channel)
    AD1CON2bits.CSCNA = 1;  // scan mode activated
    AD1CON2bits.SMPI = 1;   // Two convertion before the interrupt
    
    AD1CSSLbits.CSS11 = 1;
    AD1CSSLbits.CSS14 = 1;
    
    AD1CON1bits.ADON = 1;   // turn on the ADC module
    
}

/**
 * Function that convert the corrente? into the voltage or ... 
 * @param adc_value: value of the corrente?
 * @param type: what we want to compute, it could be voltage or temperature etc.
 * @return voltage or distance
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

/**
 * Remap Programmable pins motor control, set the motor as output to give them 
 * power
 */
void init_pwm(){
    // Set all PWN pin as output
    TRISDbits.TRISD1 = 0;   // RD1 
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    
}

void drive(char type, int time){
    switch(type){
        case('F'):      // go forward
            // OC1
            OC1CON1 = 0;
            OC1CON2 = 0;
            RPOR2bits.RP68R = 010000;   // RD4-PWMD outs OC1
            RPOR1bits.RP66R = 010000;   // RD2-PWMB outs OC1 (why?)   
        
        case('L'):      // go left
            // OC1
            OC1CON1 = 0;
            OC1CON2 = 0;
            // OC2
            OC2CON1 = 0;
            OC2CON2 = 0;
            
            RPOR2bits.RP68R = 010000;   // RD4-PWMD outs OC1
            RPOR1bits.RP66R = 010001;   // RD2-PWMB outs OC2 (why?) 
        
        case('R'):      // go right
            // OC1
            OC1CON1 = 0;
            OC1CON2 = 0;
            // OC2
            OC2CON1 = 0;
            OC2CON2 = 0;
            
            RPOR2bits.RP68R = 010000;   // RD4-PWMD outs OC1
            RPOR1bits.RP66R = 010001;   // RD2-PWMB outs OC2 (why?)
        
        case('B'):      // go backward
            // OC1 
            RPOR2bits.RP68R = 010000;   // RD4-PWMD outs OC1
            RPOR1bits.RP66R = 010000;   // RD2-PWMB outs OC1 (why?) 
        
        case('S'):      // stop
            // OC1
            OC1CON1 = 0;
            OC1CON2 = 0;
            // OC2
            OC2CON1 = 0;
            OC2CON2 = 0;
        default:
            break;
    }
            
            
}


