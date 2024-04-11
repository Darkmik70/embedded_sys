/*
 * File:   ex1_timer.c
 * Author: aless
 *
 * Created on 15 marzo 2024, 19.13
 */

#include "xc.h"
#include "timer.h"
#include "functions.h"

int main(void) {
    initializeIO();
    tmr_setup_period(TIMER1,200);
    while (1) {
        toggleLed(1);
        tmr_wait_period(TIMER1);
    }
    return 0;
}
