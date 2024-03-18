/*
 * File:   main.c
 * Author: aless
 *
 * Created on 5 marzo 2024, 13.56
 */

#include "xc.h"
#include "timer.h"
//#include "functions.h"

//int main(void) {
//    int delay = 200;     // we have to try 50, 200, 2000
//    int ret;
//    
//    initializeIO();
//    tmr_setup_period(TIMER1,delay);
//    
//    while(1){
//        turnOnLed(1);
//        tmr_wait_ms(TIMER2,delay);
//        turnOffLed(1);  
//        ret = tmr_wait_period(TIMER1);
//        if (ret){
//            toggleLed(2);
//        }
//    }
//    return 0;
//}
// Assignement 2
int main() {
    initializeIO();
    tmr_setup_period(TIMER1,200);
    while (1) {
        // code to blink LED
        toggleLed(1);
        tmr_wait_period(TIMER1); 
    }
}

// Assignment 1
//int main() {
//    initializeIO();
//
//    while (1) {
//
//        // Read the button T2
//        if (T2_BUTTON == 0) {
//            // Button is pressed, turn on the led
//            while (T2_BUTTON  == 0) {
//                // Wait for the button to be released
//                turnOnLed(1);
//            }
//        } else {
//            // Button is not pressed, turn off the LED
//            turnOffLed(1);
//        }
//
//        // Whenever the button T2 is pressed, toggle the status of the LED LD1
////        if (T2_BUTTON == 0) {
////            toggleLed(1);
////            while (T2_BUTTON == 0) {
////                // Wait for the button to be released
////            }
////        }
//    }
//    return 0;
//}
