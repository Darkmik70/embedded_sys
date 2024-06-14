
#include <xc.h>
#include "pwm.h"

/**
 * Remap Programmable pins for Output Compare, sets RD1 - RD4 bits as an output
 */
void init_pwm() {
    /* Reprogram the pins  for output */
    TRISDbits.TRISD1 = 0; // RD1 Goes to PWM A
    TRISDbits.TRISD2 = 0; // RD2 Goes to PWM B
    TRISDbits.TRISD3 = 0; // RD3 Goes to PWM C
    TRISDbits.TRISD4 = 0; // RD4 Goes to PWM D
}

/**
 * Configures OC1 for rotating
 * Clears of the bits for safety, sets peripheral clock
 * Selected duty cycle 100% with frequency 10 KHz
 * Selects Edge aligned PWM mode
 */
void drive(char type, int time){
    // OC1        
    OC1CON1 = 0;
    OC1CON2 = 0;
    switch(type){
        case('F'):      // go forward     
            RPOR2bits.RP68R = 0b010000;   // RD4-PWMD outs OC1
            RPOR1bits.RP66R = 0b010000;   // RD2-PWMB outs OC1 (why?)   
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
    OC1CON1bits.OCTSEL = 0x07;
    OC1R = 5200;
    OC1RS = 7200;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON1bits.OCM = 6;      
}