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
#include "buffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// 1kHz is 1000 ms

#define WAIT_FOR_START (0)
#define EXECUTE (1)
#define MAX_TASK 5

parser_state pstate;


//extern CircularBuffer uartBufferRx;
//extern CircularBuffer uartBufferTx;

int state;
char data;

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


// UART Interrupt
void __attribute__ (( __interrupt__ , __auto_psv__ )) _U1RXInterrupt() {
    char data;
    data = U1RXREG;         // Read from RX register
    U1TXREG = data;         // Send to TX register
    
    parse_byte(&pstate, (char)data);
    
    
    IFS0bits.U1RXIF = 0;    // Reset interrupt flag
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


void task_blink_A0(){
    toggleLed(1);
}

void task_blink_indicators(){
    toggleLed(3);
}

void task_uart_send(){
//    char send;
//    readBuffer(uartBufferTx,)
}

void task_get_battery_voltage() {
    int adc_battery = ADC1BUF0;
    double v_batt = convertTo(adc_battery, 'V');
    //    char str[16] = "$MBATT,";
    //    char float_str[10];
    char str[16];
    sprintf(str, "$MBATT,%.2f*", v_batt);
    //    strcat(str,float_str);

    for (int i = 0; str[i] != '\0'; i++) {
        //addToBuffer(&uartBufferTx, str[i]);
    }
}

void task_get_distance() {
    //AD1CON1bits.DONE = 0;
    //while(!AD1CON1bits.DONE){}
    int adc_distance = ADC1BUF1;
    float dist = convertTo(adc_distance, 'M');
    //    char str[16] = "$MBATT,";
    //    char float_str[10];
    char str[16];
    sprintf(str, "$MDIST,%d*", (int) dist);
    //    strcat(str,float_str);

    for (int i = 0; str[i] != '\0'; i++) {
        //addToBuffer(&uartBufferTx, str[i]);
    }
}


int main() {
    initializeIO();
    initUART();
    initADC();
    initPWM();
    
    LATBbits.LATB9 = 1;
    
    //initCircularBuffer(&uartBufferRx);
    //initCircularBuffer(&uartBufferTx);
    
    mapInterruptsButton();
    
    heartbeat schedInfo[MAX_TASK];
    
    schedInfo[0].n = 0;
    schedInfo[0].N = 1000;
    schedInfo[0].f = task_blink_A0;
    schedInfo[0].params = NULL;
    schedInfo[0].enable = 0;
    
    schedInfo[1].n = 0;
    schedInfo[1].N = 1000;
    schedInfo[1].f = task_blink_indicators;
    schedInfo[1].params = NULL;
    schedInfo[1].enable = 1;
    
    schedInfo[2].n = 0;
    schedInfo[2].N = 1000;
    schedInfo[2].f = task_uart_send;
    schedInfo[2].params = NULL;
    schedInfo[2].enable = 1;
    
    schedInfo[3].n = 0;
    schedInfo[3].N = 1000; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[3].f = task_get_battery_voltage;
    schedInfo[3].params = NULL;
    schedInfo[3].enable = 0;

    schedInfo[4].n = 0;
    schedInfo[4].N = 100; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[4].f = task_get_distance;
    schedInfo[4].params = NULL;
    schedInfo[4].enable = 0;
    
    state = WAIT_FOR_START;
    //parser_state pstate;
    
    init_parser(&pstate);
    
    tmr_setup_period(TIMER1,1);
    while(1){
        
//        int c = readByte();
//        if (c !=-1){
//            int ret = parse_byte(&pstate, (char)c);
//        }
        
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
