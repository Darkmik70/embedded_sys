/*
 * File:   function.c
 * Author: RomiC
 *
 * Created on March 23, 2024, 12:53 PM
 */


#include "xc.h"
#include "function.h"
#include "init.h"
#include <math.h>


/** Function to disabled every analog pins (always), and initialized LED pins as  
 * the output and buttons as input pins 
 * @param None
 * @return None
 */
void initializeIO() {
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // Set pin as output
    TRISAbits.TRISA0 = 0;   // set LD1 as an output pin
    TRISGbits.TRISG9 = 0;   // set LD2 as an output pin
    TRISBbits.TRISB8 = 0;   // set LDLEFT as an output pin
    TRISFbits.TRISF1 = 0;   // set LDRIGHT as an output pin

    // Set pin as input
    TRISEbits.TRISE8 = 1;   // T2 button set as input
    TRISEbits.TRISE9 = 1;   // T3 button set as input 
    
    LD1_LED = 0;
    LD2_LED = 0;
    LD_LEFT = 0;
    LD_RIGHT = 0;
}

/**
 * Function that turn on the assign LED
 * @param LED_NUMBER
 * @return None
 */
void turnOnLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_LED = 1;
    }
    if (LED_NUMBER == 2) {
        LD2_LED = 1;
    }
}

/**
 * Function that turn off the assign LED 
 * @param LED_NUMBER
 * @return None
 */
void turnOffLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_LED = 0;
    }
    if (LED_NUMBER == 2) {
        LD2_LED = 0;
    }
}

/**
 * Function  that change the state(toggle) of the LED assign 
 * @param LED_NUMBER
 * @return None
 */
void toggleLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_LED = !LD1_LED;
    }
    if (LED_NUMBER == 2) {
        LD2_LED = !LD2_LED;
    }
    if (LED_NUMBER == 3){
        LD_LEFT = !LD_LEFT;
        LD_RIGHT = !LD_RIGHT;
    }
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

void drive(char type, int time){
    // OC1        
    OC1CON1 = 0;
    OC1CON2 = 0;
    switch(type){
        case('F'):      // go forward     
            RPOR2bits.RP68R = 0b010000;   // RD4-PWMD outs OC1
            RPOR1bits.RP66R = 0b010000;   // RD2-PWMB outs OC1    
            break;
        
        case('L'):      // go counterclockwise
            RPOR2bits.RP68R = 0b010000;   
            RPOR0bits.RP65R = 0b010000;    
            break;
        
        case('R'):      // go clockwise
            RPOR1bits.RP67R = 0b010000;   // 
            RPOR1bits.RP66R = 0b010000;   // 
            break;
        
        case('B'):      // go backward
            RPOR1bits.RP67R = 0b010000;   // 
            RPOR0bits.RP65R = 0b010000;   //
            break;
            
        case('S'):      // stop
            break;

        default:
            break;
        
        
    }
    if(type != 'S'){
        OC1CON1bits.OCTSEL = 0x07;
        OC1R = 5200;
        OC1RS = 7200;
        OC1CON2bits.SYNCSEL = 0x1F;
        
        OC1CON1bits.OCM = 6;     
    }
            
}



// 5Hz is 100ms on e 100 off
// 2.5Hz is 200 ms on e 200 off

