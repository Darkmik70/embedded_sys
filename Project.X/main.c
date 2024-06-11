/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on 9 giugno 2024, 22.19
 */

#include "xc.h"
#include "function.h"
#include "timer.h"
#include "init.h"
#include "parser.h"
#include "scheduler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define WAIT_FOR_START (0)
#define EXECUTE (1)

// Interrupt routine of the timer 4, use to solving the debouncing problem
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(){
    IFS1bits.T4IF = 0;      // clear the flag of the interrupt timer 4
    IEC1bits.T4IE = 0;      // disable the interrupt of the timer 2
}

// Interrupt routine associated to the pressed button T2
void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(){
    IEC1bits.INT1IE = 0;        // disable the interrupt of INT1
    IEC1bits.T4IE = 1;          // enable the interrupt of the TIMER4 
    tmr_setup_period(TIMER4,20);
}

void mapInterruptsButton(){
    // Put the button as input
    TRISEbits.TRISE8 = 1; 
    
    // Assign an interrupt INT1 to the corresponding pin
    RPINR0bits.INT1R = 88;
    INTCON2bits.GIE = 1;    // set global interrupt enable
    IFS1bits.INT1IF = 0;    // clear the interrupt flag
    IEC1bits.INT1IE = 1;    // enable the interrupt button
}


int main() {
    initializeIO();
    //initUART();
    //initADC();
    //initPWM();
    mapInterruptsButton();
    int state = WAIT_FOR_START;
    
    
    while(1){
        switch(state){
            case(WAIT_FOR_START):
                turnOnLed(1);
                if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1){
                    IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                    IEC1bits.INT1IE = 1; // Enable the interrupt of INT1 
                    state = EXECUTE;
                }
            case(EXECUTE):
                if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1){
                    IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                    IEC1bits.INT1IE = 1; // Enable the interrupt of INT1 
                    state = WAIT_FOR_START;
                }
                turnOffLed(1);
        }
    }
    return 0;
}
