/*
 * File:   ex2_ports.c
 * Author: aless
 *
 * Created on 15 marzo 2024, 19.08
 */


#include "xc.h"
#include "functions.h"
#include "timer.h"

int main() {
    initializeIO();
    while (1) {
        // Read the button T2
        if (T2_BUTTON == 0) { // Button is pressed, turn on the led
            while (T2_BUTTON  == 0) { // Wait for the button to be released
                turnOnLed(1);
            }
        } else { // Button is not pressed, turn off the LED
            turnOffLed(1);
        }
    }
    return 0;
}
