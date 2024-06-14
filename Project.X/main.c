#include "xc.h"
#include "led.h"
#include "timer.h"
#include "adc.h"
#include "init.h"
#include "parser.h"
#include "scheduler.h"
#include "circular_buffer.h"
//#include <string.h>
#include <stdio.h>
//#include <stdlib.h>

#define TASKS_NUM 5

#define WAIT_FOR_START (0)
#define EXECUTE (1)

CircularBuffer buff_receive;
CircularBuffer buff_send;

// Interrupt routine of the timer 4, use to solving the debouncing problem
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(){
    IFS1bits.T4IF = 0;      // clear the flag of the interrupt timer 4
    IEC1bits.T4IE = 0;      // disable the interrupt of the timer 2
}

// Interrupt routine associated to the pressed button T2
void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(){
    IEC1bits.INT1IE = 0;        // disable the interrupt of INT1
    tmr_setup_period(TIMER4,20,1);
}

// U1RX Interrupt
void __attribute__ (( __interrupt__ , __auto_psv__ )) _U1RXInterrupt() {
    while (U1STAbits.URXDA) {
        char data;
        data = U1RXREG;         // Read from RX register
        if (write_circular_buffer(&buff_receive, data)){
            // Some action when buffer is not full and is full
        }
        else 
        {
            U1TXREG = '0';
        }
//        if (read_circular_buffer(&buffer_uart, &send))
//        {
//            U1TXREG = send;         // Send to TX register
//        }
//        else  U1TXREG = 'b';
                
        
    }
    IFS0bits.U1RXIF = 0;    // Reset interrupt flag
}

void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt()
{
    IFS0bits.U1TXIF = 0; // clear the interrupt flag
    // if the end of the message is reached
    char send;
    if (read_circular_buffer(&buff_send, &send)) {
        U1TXREG = send; // Send to TX register
    }
//    if (uart_msg[itr] == '\0')
//    {
//        itr = 0;                               // reset the iterator
//        memset(uart_msg, 0, sizeof(uart_msg)); // clear the message
//        is_msg_ready = 0;                      // reset the flag's message
//    }
//    else
//    {
//        U1TXREG = uart_msg[itr]; // send one character
//        itr++;                   // increment the iterator
//    }
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

/*
 * A0 should blink with 1 Hz no matter the state
 */
void task_blink_A0()
{ 
    toggleLed(1);     // A0
}

/*
 * Left and right indicators should blink at 1 Hz in 'Wait for start'
 */
void task_blink_indicators(void* ptr)
{
    int* state = (int*) ptr;
    int value = *state;
    
    if ( value == WAIT_FOR_START)
        toggleLed(3);
    if (value == EXECUTE)
        turnOffLed(3);
}

void task_uart_send() {
    char send;
    if (read_circular_buffer(&buff_send, &send)) {
        U1TXREG = send; // Send to TX register
    }
}

void task_get_battery_voltage() {
    int adc_battery = ADC1BUF0;
    float v_batt = get_battery_voltage(adc_battery);
//    char str[16] = "$MBATT,";
//    char float_str[10];
    char str[16];
    sprintf(str, "$MBATT,%.2f*",v_batt);
//    strcat(str,float_str);
    
    for(int i = 0; str[i] != '\0' ; i++) {
        write_circular_buffer(&buff_send, str[i]);
    }   
}

void task_get_distance() {
    AD1CON1bits.DONE = 0;
    while(!AD1CON1bits.DONE){}
    int adc_distance = ADC1BUF1;
    float dist = get_ir_distance(adc_distance);
//    char str[16] = "$MBATT,";
//    char float_str[10];
    char str[16];
    sprintf(str, "$MDIST,%d*",(int)dist);
//    strcat(str,float_str);
    
    for(int i = 0; str[i] != '\0' ; i++) {
        write_circular_buffer(&buff_send, str[i]);
    }   
}
    


int main() {
    init_IO();
    init_UART();
    init_ADC();
    init_PARSER();
    //init_PWM();
    mapInterruptsButton();
    int state = WAIT_FOR_START;
    
    init_circular_buffer(&buff_receive);
    init_circular_buffer(&buff_send);
    
    LATBbits.LATB9 = 1;     // Enable IR Sensor
   
        
    /*scheduler configuration*/
    heartbeat schedInfo[TASKS_NUM];
    
    schedInfo[0].n = 0;
    schedInfo[0].N = 1000; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[0].f = task_blink_A0;
    schedInfo[0].params = NULL;
    schedInfo[0].enable = 1;
    
    schedInfo[1].n = 0;
    schedInfo[1].N = 1000; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[1].f = task_blink_indicators;
    schedInfo[1].params = (void*)&state; //TODO set pointer to state 
    schedInfo[1].enable = 1;
    
    schedInfo[2].n = 0;
    schedInfo[2].N = 1; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[2].f = task_uart_send;
    schedInfo[2].params = NULL; 
    schedInfo[2].enable = 1;
    
    schedInfo[3].n = 0;
    schedInfo[3].N = 1000; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[3].f = task_get_battery_voltage;
    schedInfo[3].params = NULL; 
    schedInfo[3].enable = 1;
    
    schedInfo[4].n = 0;
    schedInfo[4].N = 100; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[4].f = task_get_distance;
    schedInfo[4].params = NULL; 
    schedInfo[4].enable = 1;
     

    // Control loop frequency is 1 kHz
    tmr_setup_period(TIMER1, 1, 0);
    while(1){ 
        
        /* switch state */
        if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
            IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
            IEC1bits.INT1IE = 1; // Enable the interrupt of INT1 
            switch(state){
                case(WAIT_FOR_START):
                    state = EXECUTE;
                    break;
                case(EXECUTE):
                    state = WAIT_FOR_START;
                    break;
            }
        }
        
        scheduler(schedInfo, TASKS_NUM);
        
        if (tmr_wait_period(TIMER1) == 1)
        {
            toggleLed(2);
        };
    }
    return 0;
}


