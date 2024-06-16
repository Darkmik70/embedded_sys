/*
 * File:   init.c
 * Author: emanu
 *
 * Created on 8 giugno 2024, 12.46
 */


#include "init.h"
#include "pwm.h"
#include "uart.h"
#include "timer.h"
#include "gpio.h"

void init_peripherals() {
    // Initialize GPIOs
    init_gpio();

    // Initialize PWM
    init_pwm();

    // Initialize UART
    init_uart();

    // Initialize Timer
    init_timer();

    // Enable Global Interrupts
    __builtin_enable_interrupts();
}

