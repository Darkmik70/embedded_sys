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
#include "command_list.h"

typedef struct {
    int type;
    int time;
    int cnt;
} CurrentCommand;

void init_command(CurrentCommand *cmd) {
    cmd->type = 0;
    cmd->time = 0;
    //cmd->type = 0;
}

CurrentCommand current_command;


// 1kHz is 1000 ms

#define WAIT_FOR_START (0)
#define EXECUTE (1)
#define MAX_TASK 6

parser_state pstate;

CircularBuffer buffer_Rx;
CircularBuffer buffer_Tx;
CommandList cmd_fifo;

cmd buff_cmd;

int tyme;

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


// UART Interrupt
void __attribute__ (( __interrupt__ , __auto_psv__ )) _U1RXInterrupt() {
    char data_Rx;
    data_Rx = U1RXREG;         // Read from RX register
    if(write_circular_buffer(&buffer_Rx, data_Rx) == 0){
    //toggleLed(1);
    }
    else{
        U1TXREG = '0';
    }
    IFS0bits.U1RXIF = 0;    // Reset interrupt flag
}

void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt() {
    IFS0bits.U1TXIF = 0; // clear the interrupt flag
    // if the end of the message is reached
    char send;
    if (read_circular_buffer(&buffer_Tx, &send) == 0) {
        U1TXREG = send; // Send to TX register
    }
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

void task_uart_send() {
    char send;
    if (read_circular_buffer(&buffer_Tx, &send) == 0) {
        U1TXREG = send; // Send to TX register
    }
}


void task_get_battery_voltage() {
    int adc_battery = ADC1BUF0;
    double v_batt = convertTo(adc_battery, 'V');
    char str[16];
    sprintf(str, "$MBATT,%.2f*", v_batt);
    //    strcat(str,float_str);

    for (int i = 0; str[i] != '\0'; i++) {
        write_circular_buffer(&buffer_Tx, str[i]);
    }
}

void task_get_distance() {
    //AD1CON1bits.DONE = 0;
    //while(!AD1CON1bits.DONE){}
    int adc_distance = ADC1BUF1;
    float dist = convertTo(adc_distance, 'M');
    char str[16];
    sprintf(str, "$MDIST,%d*", (int) dist);
    //    strcat(str,float_str);

    for (int i = 0; str[i] != '\0'; i++) {
        write_circular_buffer(&buffer_Tx, str[i]);
    }
}

// finally in the command control for pwm we could implement something like:
// (1) if no current command then take one 
//if (wait for start):
//	do nothing
//if(execute)
//	if (m_dist <= 20 cm):
//           (if cmd is in its execution, count time) else do nothing
//        else
//		just simply execute function adn count time.

void task_pwm_control() {
    if (state == EXECUTE) {
        if (current_command.time == 0) {
            //drive(0);
            dequeue_cmd_list(&cmd_fifo, &current_command.type, &current_command.time);
            drive(current_command.type);
            current_command.type = 0;
        }

        if (current_command.time != 0) {
            current_command.time = current_command.time - 1;
        }
        //U1TXREG = current_command.time + '0';
        U1TXREG = current_command.type + '0';
    }
}

//int parsing_process() {
//    char data;
//    int result;
//
//    // Read a byte from the buffer
//    if (read_circular_buffer(&buffer_Rx, &data) == 0) {
//        // Parse the byte
//        result = parse_byte(&pstate, data);
//        
//        // Check if a new message has been received
//        if (result == NEW_MESSAGE) {
//            turnOnLed(2);
//            // Handle the new message
//            // For example, if the message type is 'PCCMD', turn on LED 1
//            if (strncmp(pstate.msg_type, "PCCMD", strlen("PCCMD")) == 0) {
//                // Extract the first number
//                int cmd_type = extract_integer(pstate.msg_payload);
//                for (int i = 0; pstate.msg_payload[i] != '\0'; i++) {
//                    if (cmd_type < 5 && cmd_type > 0) {
//                        // Find the start of the next number
//                        int next_index = next_value(pstate.msg_payload, 0);
//                        // Extract the second number
//                        int cmd_time = extract_integer(pstate.msg_payload + next_index);
//                        if (cmd_time > 0) {
//                            char msg[9];
//                            if(enqueue_cmd_list(&cmd_fifo, cmd_type, cmd_time) == 0 ) {
//                                strcat(msg,"$MACK,1*");
////                                char *msg = ;"$MACK,1*"
//                            }
//                            else {
//                                strcat(msg,"$MACK,0*");
////                                char *msg = "$MACK,0*";
//                            }
//                            for(int i = 0; msg[i] != '\0'; i++) {
//                                write_circular_buffer(&buffer_Tx, msg[i]);
//                            }
//
//                        }
//                    }
//                }
//                
//                return 1;
//            }
//        }
//    }
//}


//            if (strncmp(pstate.msg_type, "PCCMD", strlen("PCCMD")) == 0) {
//                // Extract the first number
//                int cmd_type = extract_integer(pstate.msg_payload);
//                for (int i = 0; pstate.msg_payload[i] != '\0'; i++) {
//                    if (cmd_type < 5 && cmd_type > 0) {
//                        // Find the start of the next number
//                        int next_index = next_value(pstate.msg_payload, 0);
//                        // Extract the second number
//                        int cmd_time = extract_integer(pstate.msg_payload + next_index);
//                        if (cmd_time > 0) {
//                            char msg[9];
//                            if(enqueue_cmd_list(&cmd_fifo, cmd_type, cmd_time) == 0 ) {
//                                strcat(msg,"$MACK,1*");
////                                char *msg = ;"$MACK,1*"
//                            }
//                            else {
//                                strcat(msg,"$MACK,0*");
////                                char *msg = "$MACK,0*";
//                            }
//                            for(int i = 0; msg[i] != '\0'; i++) {
//                                write_circular_buffer(&buffer_Tx, msg[i]);
//                            }
//
//                        }
//                    }
//                }

int parsing_process() {
    char data;
    int result;
    
    // Read a byte from the buffer
    if (read_circular_buffer(&buffer_Rx, &data) == 0) {
        // Parse the byte
        result = parse_byte(&pstate, data);

        // Check if a new message has been received
        if (result == NEW_MESSAGE) {

            // For example, if the message type is 'PCCMD', turn on LED 1
            if (strncmp(pstate.msg_type, "PCCMD", strlen("PCCMD")) == 0) {
                // Extract the first number
                int cmd_type = extract_integer(pstate.msg_payload);

                // Find the start of the next number
                int next_index = next_value(pstate.msg_payload, 0);

                // Extract the second number
                int cmd_time = extract_integer(pstate.msg_payload + next_index);

                if (enqueue_cmd_list(&cmd_fifo, cmd_type, cmd_time) == 0) {
                    U1TXREG = '6';
                }
            }
        }

    }
    return 0;
}



int main() {
    
    
    initIO();
    initUART();
    initADC();
    initPWM();
    
    LATBbits.LATB9 = 1;
    TRISEbits.TRISE8 = 1;
    
    mapInterruptsButton();
    
    init_parser(&pstate);
    initBuffer(&buffer_Rx);
    initBuffer(&buffer_Tx);
    init_cmd_list(&cmd_fifo);
    init_command(&current_command);

    
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
    schedInfo[3].enable = 1;

    schedInfo[4].n = 0;
    schedInfo[4].N = 100; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[4].f = task_get_distance;
    schedInfo[4].params = NULL;
    schedInfo[4].enable = 1;
    
    schedInfo[5].n = 0;
    schedInfo[5].N = 1; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[5].f = task_pwm_control;
    schedInfo[5].params = NULL;
    schedInfo[5].enable = 1;
    
    state = WAIT_FOR_START;
    

    
    tmr_setup_period(TIMER1,1);
    
    char data_Rx;
    enable_interrupt1();
    while(1){
        
        //parsing_process(); 
        if(parsing_process() == 0){
        }    
        
        /* switch state */
        switch (state) {
            case(WAIT_FOR_START):
                //drive(1);
                if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
                    IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                    IEC1bits.INT1IE = 1; // Enable the interrupt of INT1
                    state = EXECUTE;
                }
                break;
            case(EXECUTE):
                //drive(2);
                toggleLed(2);
                if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
                    IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                    IEC1bits.INT1IE = 1; // Enable the interrupt of INT1
                    state = WAIT_FOR_START;
                }
                //state = WAIT_FOR_START;
                break;
 
        }

        scheduler(schedInfo, MAX_TASK);
        tmr_wait_period(TIMER1);
    }
    
    return 0;
}
