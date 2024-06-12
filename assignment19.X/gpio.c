/*
 * File:   gpio.c
 * Author: emanu
 *
 * Created on 8 giugno 2024, 12.26
 */


#include <xc.h>
#include "gpio.h"

void init_gpio() {
    // Initialize button input (RE8)
    TRISEbits.TRISE8 = 1; // Set RE8 as input (button)

    // Initialize LEDs
    TRISAbits.TRISA0 = 0; // Set RA0 as output (LED A0)
    TRISBbits.TRISB8 = 0; // Set RB8 as output (Left indicator)
    TRISFbits.TRISF1 = 0; // Set RF1 as output (Right indicator)
}

int button_pressed() {
    return PORTEbits.RE8; // Return the state of the button
}

void blink_leds() {
    static int state = 0;
    static unsigned int count = 0;

    if (count++ >= 500) { // Assuming 1 kHz loop, toggle every 500 ms for 1 Hz blink
        count = 0;
        state = !state;
        LATAbits.LATA0 = state; // Toggle LED A0
        LATBbits.LATB8 = state; // Toggle left indicator
        LATFbits.LATF1 = state; // Toggle right indicator
    }
}

void blink_led() {
    static int state = 0;
    static unsigned int count = 0;

    if (count++ >= 500) { // Assuming 1 kHz loop, toggle every 500 ms for 1 Hz blink
        count = 0;
        state = !state;
        LATAbits.LATA0 = state; // Toggle LED A0
    }
}
