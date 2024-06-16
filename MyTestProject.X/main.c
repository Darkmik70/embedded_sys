/*
 * File:   main.c
 * Author: aless
 *
 * Created on 5 marzo 2024, 13.56
 */

#include "xc.h"
#include "timer.h"
#include "functions.h"


// EXERCISE 2
int main() {
    initializeIO();
    tmr_setup_period(TIMER1,500);
    while (1) {
        // code to blink LED
        toggleLed(1);
        tmr_wait_period(TIMER1);
    }
}

// EXERCISE 1
//int main() {
//    initializeIO();
//
//    while (1) {
//
//        // Read the button T2
//        if (T2_PIN == 0) {
//            // Button is pressed, turn on the led
//            while (T2_PIN  == 0) {
//                // Wait for the button to be released
//                turnOnLed(1);
//            }
//        } else {
//            // Button is not pressed, turn off the LED
//            turnOffLed(1);
//        }
//
//        // Whenever the button T2 is pressed, toggle the status of the LED LD1
////        if (T2_PIN == 0) {
////            toggleLed(1);
////            while (T2_PIN == 0) {
////                // Wait for the button to be released
////            }
////        }
//    }
//
//    return 0;
//}
