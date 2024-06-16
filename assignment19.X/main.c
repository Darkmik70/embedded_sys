/*
 * File:   main.c
 * Author: emanu
 *
 * Created on June 8, 2024, 10:39 AM
 */

#include <xc.h>
#include "main.h"
#include "init.h"
#include "gpio.h"
#include "parser.h"
#include "scheduler.h"
#include "timer.h"

volatile int state = WAIT_FOR_START;

void clear_command_queue() {
    // Implementation to clear the command queue
    queue_head = 0;
    queue_tail = 0;
}

int obstacle_detected() {
    // Placeholder for obstacle detection logic
    // For now, let's return 0 indicating no obstacle
    return 0;
}

int main(void) {
    // Initialize all peripherals
    init_peripherals();

    while (1) {
        switch (state) {
            case WAIT_FOR_START:
                handle_wait_for_start();
                break;
            case EXECUTE:
                handle_execute();
                break;
            default:
                // Invalid state, reset to WAIT_FOR_START
                state = WAIT_FOR_START;
                break;
        }
    }
    return 0; // Return an integer
}

void handle_wait_for_start() {
    // Blink LED A0 and indicators at 1 Hz
    blink_leds();

    // Check if button RE8 is pressed
    if (button_pressed()) {
        state = EXECUTE;
        clear_command_queue();
    }
}

void handle_execute() {
    // Blink LED A0 at 1 Hz
    blink_led();

    // Process commands in FIFO
    process_commands();

    // Check for obstacles
    if (obstacle_detected()) {
        stop_motors();
    }

    // Check if button RE8 is pressed to stop
    if (button_pressed()) {
        state = WAIT_FOR_START;
        clear_command_queue();
    }
}


