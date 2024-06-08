/*
 * File:   pwm.c
 * Author: emanu
 *
 * Created on 8 giugno 2024, 12.27
 */


#include <xc.h>
#include "pwm.h"

void init_pwm() {
    // Configure PWM outputs
    // RD1, RD2, RD3, RD4 as PWM outputs
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;

    // Configure Timer for PWM frequency (10 kHz)
    T2CONbits.TCKPS = 0;    // Prescaler 1:1
    PR2 = 1599;             // Period register for 10 kHz with Fosc = 16 MHz
    TMR2 = 0;               // Clear timer register
    T2CONbits.TON = 1;      // Start Timer2

    // Initialize Output Compare Modules
    OC1CON1bits.OCM = 6;    // Edge-aligned PWM mode
    OC2CON1bits.OCM = 6;
    OC3CON1bits.OCM = 6;
    OC4CON1bits.OCM = 6;
}

void set_pwm(int left_pwm, int right_pwm) {
    // Set PWM for left wheels
    if (left_pwm > 0) {
        OC1RS = 0; // Stop backward
        OC2RS = left_pwm; // Start forward
    } else {
        OC1RS = -left_pwm; // Start backward
        OC2RS = 0; // Stop forward
    }

    // Set PWM for right wheels
    if (right_pwm > 0) {
        OC3RS = 0; // Stop backward
        OC4RS = right_pwm; // Start forward
    } else {
        OC3RS = -right_pwm; // Start backward
        OC4RS = 0; // Stop forward
    }
}

void stop_motors() {
    // Stop all motors
    OC1RS = 0;
    OC2RS = 0;
    OC3RS = 0;
    OC4RS = 0;
}

