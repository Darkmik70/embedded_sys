/*
 * File:   main.c
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#include "xc.h"

#include "adc.h"
#include "circular_buffer.h"
#include "command_list.h"
#include "init.h"
#include "led.h"
#include "parser.h"
#include "pwm.h"
#include "scheduler.h"
#include "timer.h"

#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Tasks number
#define TASKS_NUM 6
// States
#define WAIT_FOR_START (0)
#define EXECUTE (1)

CircularBuffer buffer_RX;
CircularBuffer buffer_TX;
CommandList cmd_fifo;
parser_state pstate;
CurrentCommand current_command;

int dist;
int state;
int brake;

// Interrupt routine of the timer 4, use to solving the debouncing problem
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt() {
    IFS1bits.T4IF = 0; // clear the flag of the interrupt timer 4
    IEC1bits.T4IE = 0; // disable the interrupt of the timer 2
}

// Interrupt routine associated to the pressed button T2
void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt() {
    IEC1bits.INT1IE = 0; // disable the interrupt of INT1
    tmr_setup_period(TIMER4, 20, 1);
}

// U1RX Interrupt
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt() {
    IFS0bits.U1RXIF = 0; // Reset interrupt flag
    char data;
    data = U1RXREG; // Read from RX register
    write_circular_buffer(&buffer_RX, data);
}

// U1TX Interrupt
void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt() {
    IFS0bits.U1TXIF = 0; // clear the interrupt flag
}

void mapInterruptsButton() {
    // Put the button as input
    TRISEbits.TRISE8 = 1;

    // Assign an interrupt INT1 to the corresponding pin
    RPINR0bits.INT1R = 88;
    INTCON2bits.GIE = 1; // set global interrupt enable
    IFS1bits.INT1IF = 0; // clear the interrupt flag
    IEC1bits.INT1IE = 1; // enable the interrupt button
}

/*
 * A0 should blink with 1 Hz no matter the state
 */
void task_blink_A0() {
    toggleLed(1); // A0
}

/*
 * Left and right indicators should blink at 1 Hz in 'Wait for start'
 */
void task_blink_indicators() {
    toggleLed(3);
}

void task_uart_send() {
    char send;
    if (read_circular_buffer(&buffer_TX, &send) == 0) {
        U1TXREG = send; // Send to TX register
    }
}

void task_get_battery_voltage() {
    int adc_battery = ADC1BUF0;
    double v_batt = convertTo(adc_battery, 'V');
    char str[16];
    sprintf(str, "$MBATT,%.2f*", v_batt);

    for (int i = 0; str[i] != '\0'; i++) {
        write_circular_buffer(&buffer_TX, str[i]);
    }
}

void task_get_distance() {
    //AD1CON1bits.DONE = 0;
    //while(!AD1CON1bits.DONE){}
    int adc_distance = ADC1BUF1;
    dist = convertTo(adc_distance, 'M');
    char str[16];
    sprintf(str, "$MDIST,%d*", dist);

    for (int i = 0; str[i] != '\0'; i++) {
        write_circular_buffer(&buffer_TX, str[i]);
    }
}

int parsing_process() {
    char data;
    int result, cmd_type, next_index, cmd_time;
    char ack_1[] = "$MACK,1*";
    char ack_0[] = "$MACK,0*";
    
    // Read a byte from the buffer
    if (read_circular_buffer(&buffer_RX, &data) == 0) {
        // Parse the byte
        result = parse_byte(&pstate, data);
        // Check if a new message has been received
        if (result == NEW_MESSAGE) {

            // For example, if the message type is 'PCCMD', turn on LED 1
            if (strncmp(pstate.msg_type, "PCCMD", strlen("PCCMD")) == 0) {
                // Extract the first number
                cmd_type = extract_integer(pstate.msg_payload);

                // Find the start of the next number
                next_index = next_value(pstate.msg_payload, 0);

                // Extract the second number
                cmd_time = extract_integer(pstate.msg_payload + next_index);

                if (enqueue_cmd_list(&cmd_fifo, cmd_type, cmd_time) == 0) {
                    for (int i = 0; ack_1[i] != '\0'; i++) {
                        write_circular_buffer(&buffer_TX, ack_1[i]);
                    }
                } else {
                    for (int i = 0; ack_0[i] != '\0'; i++) {
                        write_circular_buffer(&buffer_TX, ack_0[i]);
                    }
                }
            }
        }

    }
    return 0;
}

void task_pwm_control() {
    if (state == EXECUTE) {
        if (current_command.time == 0) {
            dequeue_cmd_list(&cmd_fifo, &current_command.type, &current_command.time);
            drive(current_command.type);
            current_command.type = 0;
        }
        if (current_command.time != 0) {
            current_command.time--;
        }    
        if (dist <= 20) {
            // With brake
//            drive(4);
//            if (brake != 0) {
//                brake--;
//            } else {
//                current_command.type = 0;
//                drive(current_command.type);
//            }
            // Without brake
            current_command.type = 0;
            drive(current_command.type);
        }
    }
    if (state == WAIT_FOR_START) {
        current_command.time = 0;
        current_command.type = 0;
    }

}

int main() {
    
    init_IO();
    init_UART();
    init_ADC();
    init_PWM();
    
    mapInterruptsButton();
    
    state = WAIT_FOR_START;

    init_parser(&pstate);
    init_circular_buffer(&buffer_RX);
    init_circular_buffer(&buffer_TX);
    init_cmd_list(&cmd_fifo);
    init_command(&current_command);

    LATBbits.LATB9 = 1; // Enable IR Sensor

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
    schedInfo[1].params = NULL;
    schedInfo[1].enable = 1;

    schedInfo[2].n = 0;
    schedInfo[2].N = 1; // 1 kHz frequency, triggers every 1 runs
    schedInfo[2].f = task_uart_send;
    schedInfo[2].params = NULL;
    schedInfo[2].enable = 1;

    schedInfo[3].n = 0;
    schedInfo[3].N = 1000; // 1 Hz frequency, triggers every 1000 runs
    schedInfo[3].f = &task_get_battery_voltage;
    schedInfo[3].params = NULL;
    schedInfo[3].enable = 1;

    schedInfo[4].n = 0;
    schedInfo[4].N = 100; // 10 Hz frequency, triggers every 100 runs
    schedInfo[4].f = task_get_distance;
    schedInfo[4].params = NULL;
    schedInfo[4].enable = 1;
    
    schedInfo[5].n = 0;
    schedInfo[5].N = 1; // 1 kHz frequency, triggers every 1 runs
    schedInfo[5].f = task_pwm_control;
    schedInfo[5].params = NULL;
    schedInfo[5].enable = 1;


    // Control loop frequency is 1 kHz
    tmr_setup_period(TIMER1, 1, 0);
    while(1){
 
        if(parsing_process() == 0){}    
        
        /* switch state */
        switch (state) {
            case(WAIT_FOR_START):
                drive(0); // PWM DC of all the motors is 0
                schedInfo[0].enable = 1; // LED A0 blink at 1 Hz
                schedInfo[1].enable = 1; // Left and right indicators blink at 1 Hz
                //schedInfo[5].enable = 0; // Disable PWM control 
                
                if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
                    IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                    IEC1bits.INT1IE = 1; // Enable the interrupt of INT1
                    brake = 100;
                    state = EXECUTE;
                }
                break;
            case(EXECUTE):
                schedInfo[1].enable = 0; // Left and right indicators don't blink
                turnOffLed(3); // Turn off left and right indicators
                //schedInfo[5].enable = 1; // Enable PWM control

                if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
                    IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                    IEC1bits.INT1IE = 1; // Enable the interrupt of INT1
                    state = WAIT_FOR_START;
                }
                break;
        }

        scheduler(schedInfo, TASKS_NUM);
        if (tmr_wait_period(TIMER1) == 1) {
            toggleLed(2);
        }
    }
    return 0;
}


