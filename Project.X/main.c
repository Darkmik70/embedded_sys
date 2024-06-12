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


// 1kHz is 1000 ms

#define WAIT_FOR_START (0)
#define EXECUTE (1)
#define MAX_TASK 5

int state;

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


void enable_interrupt1(){
    IFS1bits.INT1IF = 0;
    IEC1bits.INT1IE = 1;
}

// funzione da RIVEDERE!!!!
void task_readIR(){
    AD1CON1bits.DONE = 0;
    int adc_IRsensor;
    while (!AD1CON1bits.DONE);
    adc_IRsensor = ADC1BUF1;
    float cm = convertTo(adc_IRsensor,'M')*100;
    //return cm;
}

void task_blinkA0(){
    toggleLed(1);
}

void task_blinkLED(){
    toggleLed(3);
}

int buffer_full(){
    int qualcosa;
    if(qualcosa){
        return 1;
    }
    return 0;    
}

void task_sendUART(){
    char buffer[8];
    buffer[0] = '$';
    buffer[1] = 'M';
    buffer[2] = 'A';
    buffer[3] = 'C';
    buffer[4] = 'K';
    buffer[5] = ',';
    if(buffer_full){
        buffer[6] = '0';
    }
    else{
        buffer[6] = '1';
    }    
    buffer[7] = '*';
    
    
}

void task_updatePWM(){
    if (state == WAIT_FOR_START){
        drive('S',0);
    }
    else{
        
    }
}

//void configuration_scheduler(){
//    heartbeat schedInfo[MAX_TASK];
//    
//    schedInfo[0].n = 0;
//    schedInfo[0].N = 1;
//    schedInfo[0].f = task_readIR;
//    //schedInfo[0].params = (void*)(&controlData.setpoint);
//    schedInfo[0].enable = 1;
//    
//    schedInfo[1].n = 0;
//    schedInfo[1].N = 1;
//    schedInfo[1].f = task_updatePWM;
//    // da gestire anche il timer in questa task
//    //schedInfo[1].params = (void*)(&controlData);
//    schedInfo[1].enable = 1;
//    
//    schedInfo[2].n = 0;
//    schedInfo[2].N = 1000;
//    schedInfo[2].f = task_blinkA0;
//    //schedInfo[2].params = NULL;
//    schedInfo[2].enable = 1;
//    
//    schedInfo[3].n = 0;
//    schedInfo[3].N = 1000;
//    schedInfo[3].f = task_blinkLED;
//    //schedInfo[3].params = (void*)(&controlData);
//    schedInfo[3].enable = 1;
//    
//    schedInfo[4].n = 0;
//    schedInfo[4].N = 4;
//    schedInfo[4].f = task_button;
//    //schedInfo[4].params = (void*)(&controlData);
//    schedInfo[4].enable = 1;
//}


int main() {
    initializeIO();
    initUART();
    initADC();
    initPWM();
    
    mapInterruptsButton();
    
    heartbeat schedInfo[MAX_TASK];
    
    schedInfo[0].n = 0;
    schedInfo[0].N = 1;
    schedInfo[0].f = task_readIR;
    //schedInfo[0].params = (void*)(&controlData.setpoint);
    schedInfo[0].enable = 1;
    
    schedInfo[1].n = 0;
    schedInfo[1].N = 1;
    schedInfo[1].f = task_updatePWM;
    // da gestire anche il timer in questa task
    //schedInfo[1].params = (void*)(&controlData);
    schedInfo[1].enable = 1;
    
    schedInfo[2].n = 0;
    schedInfo[2].N = 1000;
    schedInfo[2].f = task_blinkA0;
    //schedInfo[2].params = NULL;
    schedInfo[2].enable = 1;
    
    schedInfo[3].n = 0;
    schedInfo[3].N = 1000;
    schedInfo[3].f = task_blinkLED;
    //schedInfo[3].params = (void*)(&controlData);
    schedInfo[3].enable = 1;
    
    schedInfo[4].n = 0;
    schedInfo[4].N = 4;
    //schedInfo[4].f = task_button;
    //schedInfo[4].params = (void*)(&controlData);
    schedInfo[4].enable = 1;
    
    state = WAIT_FOR_START;
    
    
    tmr_setup_period(TIMER1,1);
    while(1){
        if(T2_BUTTON == 1 && IFS1bits.INT1IF == 1){
            enable_interrupt1();
            if(state == WAIT_FOR_START){
                state = EXECUTE;
            }
            else if(state == EXECUTE){
                state = WAIT_FOR_START;
            }
        }
        scheduler(schedInfo, MAX_TASK);
        tmr_wait_period(TIMER1);
    }
    
    return 0;
}
