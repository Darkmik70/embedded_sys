/*
 * File:   functions.c
 * Author: aless
 *
 * Created on March 7, 2024, 11:57 AM
 */


#include "xc.h"
#include "functions.h"

void initializeIO() {
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // Set LD1 pin as output
    TRISAbits.TRISA0 = 0;
    
    // Set LD2 pin as output
    TRISGbits.TRISG9 = 0;

    // Set T2 pin as input
    TRISEbits.TRISE8 = 1;
    
    // Set T3 pin as input
    TRISEbits.TRISE9 = 1;
}

// turn on LEDs
void turnOnLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_PIN = 1;
    }
    if (LED_NUMBER == 2) {
        LD2_PIN = 1;
    }
}

// turn off LEDs
void turnOffLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_PIN = 0;
    }
    if (LED_NUMBER == 2) {
        LD2_PIN = 0;
    }
}

// toggle LEDs
void toggleLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_PIN = !LD1_PIN;
    }
    if (LED_NUMBER == 2) {
        LD2_PIN = !LD2_PIN;
    }
}



