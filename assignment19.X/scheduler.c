/*
 * File:   scheduler.c
 * Author: emanu
 *
 * Created on 8 giugno 2024, 12.28
 */

#include "scheduler.h"
#include "gpio.h"
#include "pwm.h"

void scheduler() {
    // Schedule tasks at 1 kHz
    blink_leds();
    // Add other tasks here, e.g., read sensors, update motors
}
