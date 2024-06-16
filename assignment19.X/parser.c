/*
 * File:   parser.c
 * Author: emanu
 *
 * Created on 8 giugno 2024, 12.28
 */

#include "parser.h"
#include "uart.h"
#include "pwm.h"
#include <string.h>
#include <stdio.h>  // Add this for sscanf

#define QUEUE_SIZE 10
#define FORWARD_SPEED 1000
#define ROTATE_SPEED 500
#define BACKWARD_SPEED 1000

typedef struct {
    int command;
    int duration;
} Command;

Command command_queue[QUEUE_SIZE];
int queue_head = 0;
int queue_tail = 0;

void parse_command(char* command_str) {
    // Parse the command and add to queue
    Command cmd;
    sscanf(command_str, "$PCCMD,%d,%d*", &cmd.command, &cmd.duration);
    
    if ((queue_tail + 1) % QUEUE_SIZE == queue_head) {
        // Queue is full, send error
        send_uart("$MACK,0*\r\n");
    } else {
        // Add command to queue
        command_queue[queue_tail] = cmd;
        queue_tail = (queue_tail + 1) % QUEUE_SIZE;
        send_uart("$MACK,1*\r\n");
    }
}

void execute_command(Command cmd);

void process_commands() {
    if (queue_head != queue_tail) {
        Command current_command = command_queue[queue_head];
        execute_command(current_command);
        queue_head = (queue_head + 1) % QUEUE_SIZE;
    } else {
        stop_motors();
    }
}

void execute_command(Command cmd) {
    // Execute command based on type and duration
    switch (cmd.command) {
        case 1: // Forward motion
            set_pwm(FORWARD_SPEED, FORWARD_SPEED);
            break;
        case 2: // Counterclockwise rotation
            set_pwm(-ROTATE_SPEED, ROTATE_SPEED);
            break;
        case 3: // Clockwise rotation
            set_pwm(ROTATE_SPEED, -ROTATE_SPEED);
            break;
        case 4: // Backward motion
            set_pwm(-BACKWARD_SPEED, -BACKWARD_SPEED);
            break;
        default:
            stop_motors();
            break;
    }
}


