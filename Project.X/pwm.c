/*
 * File:   pwm.c
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#include <xc.h>
#include "pwm.h"
#include "led.h"

#define DUTY_CICLE 3400
/**
 * Configures OCs for:
 * 1) Forward motion
 * 2) Counterclockwise rotation
 * 3) Clockwise rotation
 * 4) Backward motion
 * Clears of the bits
 * Selects duty cycle with frequency 10 KHz
 * Selects Edge aligned PWM mode
 */
void drive(int type) {
    // OC1        
    OC1CON1 = 0;
    OC1CON2 = 0;
    // OC2
    OC2CON1 = 0;
    OC2CON2 = 0;
    // OC3
    OC3CON1 = 0;
    OC3CON2 = 0;
    // OC4
    OC4CON1 = 0;
    OC4CON2 = 0;

    RPOR2bits.RP68R = 0b000000; // Right PWM forward motion
    RPOR1bits.RP67R = 0b000000; // Right PWM backward motion
    RPOR1bits.RP66R = 0b000000; // Left PWM forward motion
    RPOR0bits.RP65R = 0b000000; // Left PWM backward motion

    switch (type) {
        case(1): // Forward
            toggleLed(1);
            RPOR2bits.RP68R = 0b010011; // Right PWM forward motion
            RPOR1bits.RP66R = 0b010001; // Left PWM forward motion
            break;

        case(2): // Counterclockwise
            RPOR2bits.RP68R = 0b010011; // Right PWM forward motion
            RPOR0bits.RP65R = 0b010000; // Left PWM backward motion
            break;

        case(3): // Clockwise
            RPOR1bits.RP67R = 0b010010; // Right PWM backward motion
            RPOR1bits.RP66R = 0b010001; // Left PWM forward motion
            break;

        case(4): // Backward
            RPOR1bits.RP67R = 0b010010; // Right PWM backward motion
            RPOR0bits.RP65R = 0b010000; // Left PWM backward motion
            break;

        default: // Stop
            break;
    }

    // OC1: Left PWM backward motion
    OC1CON1bits.OCTSEL = 0x07;
    OC1R = DUTY_CICLE;
    OC1RS = 7200;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON1bits.OCM = 6;
    
    // OC2: Left PWM forward motion
    OC2CON1bits.OCTSEL = 0x07;
    OC2R = DUTY_CICLE;
    OC2RS = 7200;
    OC2CON2bits.SYNCSEL = 0x1F;
    OC2CON1bits.OCM = 6;
    
    // OC3: Right PWM backward motion
    OC3CON1bits.OCTSEL = 0x07;
    OC3R = DUTY_CICLE;
    OC3RS = 7200;
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3CON1bits.OCM = 6;
    
    // OC4: Right PWM forward motion
    OC4CON1bits.OCTSEL = 0x07;
    OC4R = DUTY_CICLE;
    OC4RS = 7200;
    OC4CON2bits.SYNCSEL = 0x1F;
    OC4CON1bits.OCM = 6;
}