/*
 * File:   main.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:41 PM
 */
#include "xc.h"
#include "timer.h"

// blinking led
void ex_1() 
{
    // Set T2 Button as input
    TRISEbits.TRISE8 = 1;
    // Set LD1 LED as output
    TRISAbits.TRISA0 = 0;

    tmr_setup_period(TIMER1, 200);

    while (1) {
        // Toggle LD1
        LATAbits.LATA0 = !LATAbits.LATA0;
        tmr_wait_period(TIMER1);
    }
}

void ex_2()
{
    // Set T2 Button as input
    TRISEbits.TRISE8 = 1;
    // Set LD1 LED as output
    TRISAbits.TRISA0 = 0;
 
    
    while(1)
    {
        LATAbits.LATA0 = 1;
        tmr_wait_ms(TIMER1, 20);
        
        LATAbits.LATA0 = 0;
        tmr_wait_ms(TIMER1, 400);
    }
}

int main(void) 
{
    //    ex_1();
    ex_2();
    return 0;
}
