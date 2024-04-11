/*
 * File:   ex3_ports.c
 * Author: aless
 *
 * Created on 15 marzo 2024, 19.09
 */

#include "xc.h"
#include "functions.h"
#include "timer.h"

int main() {
    initializeIO();
    while (1) {
        // Whenever the button T2 is pressed, toggle the status of the LED LD1
        if (T2_BUTTON == 0) {
            tmr_wait_ms(TIMER1,20); // Wait to avoid bouncing
            toggleLed(1);
            while (T2_BUTTON == 0) {
            }    // Wait for the button to be released
            tmr_wait_ms(TIMER1,20); // Wait to avoid bouncing
        }
    }
    return 0;
}
