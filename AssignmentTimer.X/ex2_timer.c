/*
 * File:   ex2_timer.c
 * Author: aless
 *
 * Created on 15 marzo 2024, 19.14
 */

#include "xc.h"
#include "timer.h"
#include "functions.h"

int main(void) {
    initializeIO();
    while (1) {
        turnOnLed(1);
        tmr_wait_ms(TIMER1,20);
        turnOffLed(1);
        tmr_wait_ms(TIMER1,200);
    }
    return 0;
}
