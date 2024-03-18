/*
 * File:   ex3_timer.c
 * Author: aless
 *
 * Created on 5 marzo 2024, 13.56
 */

#include "xc.h"
#include "timer.h"
#include "functions.h"

int main() {
    int ret;
    int delay = 50; // we have to try 50, 200, 2000
    
    initializeIO();
    tmr_setup_period(TIMER1,200);
    while (1) {
        tmr_wait_ms(TIMER2,delay);
        toggleLed(1); // led trigger (led_status = !led_status)
        ret = tmr_wait_period(TIMER1);
        if (ret) {
            turnOnLed(2); // turn on led 2
        }
        else {
            turnOffLed(2); // turn off led 2
        }
    }
    return 0;
}
