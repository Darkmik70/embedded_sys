/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on 6 giugno 2024, 16.32
 */


#include "xc.h"
#include "function.h"
#include "timer.h"
#include "init.h"

// Interrupt routine of the timer 4, use to solving the debouncing problem
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(){
    IFS1bits.T4IF = 0;      // clear the flag of the interrupt timer 4
    IEC1bits.T4IE = 0;      // disable the interrupt of the timer 2
    T4CONbits.TON = 0;      // stop the timer 2
}

// Interrupt routine associated to the pressed button T2
void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(){
    IEC1bits.INT1IE = 0;        // disable the interrupt of INT1
    IEC1bits.T4IE = 1;          // enable the interrupt of the TIMER4 
    tmr_setup_period(TIMER4,10);
}

void mapInterruptsButton(){
    // Put the button as input
    //TRISEbits.TRISE8 = 1; 
    
    // Assign an interrupt INT1 to the corresponding pin
    RPINR0bits.INT1R = 88;
    INTCON2bits.GIE = 1;    // set global interrupt enable
    IFS1bits.INT1IF = 0;    // clear the interrupt flag
    IEC1bits.INT1IE = 1;    // enable the interrupt button
}

/**
 * Remap Programmable pins motor control, set the motor as output to give them 
 * power
 */
void init_pwn(){
    // Set all PWN pin as output
    TRISDbits.TRISD1 = 0;   // RD1 
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    
}

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

/* Exercise n.1
 * When button E8 is pressed, start generating a PWN signal with the Output 
 * Compare peripheral at a 10 kHz frequency to make the buggy move foward. When
 * the buggy, stop generating the PWN signal.
 * HINT: the duty cycle range to enable motion is between 40% and 100% 
 */
int main() {
    
    initializeIO();
    init_pwn();
    mapInterruptsButton();
    int pwm_on = 0;
    
    
    while(1){
        //drive('F',100);
        if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1){
            pwm_on = !pwm_on;
            IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
            IEC1bits.INT1IE = 1; // Enable the interrupt of INT1   
            if (pwm_on){
                drive('F',100);
            }
            else{
                drive('S',0);
            }
        }
    }
    return 0;
}
