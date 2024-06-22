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

void ex_3()
{
        // all the analog are disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // initialized the function of the pins
    TRISAbits.TRISA0 = 0;   // set pin as output
    TRISGbits.TRISG9 = 0;   // set pin as output
    
    tmr_setup_period(TIMER1,200);
    
    int delay = 200;     // we have to try 50, 200, 2000
    int ret;
    
        while(1){
        LATAbits.LATA0 = 1;
        tmr_wait_ms(TIMER2,delay);
        LATAbits.LATA0 = 0;  
        ret = tmr_wait_period(TIMER1);
        if (ret){
            LATGbits.LATG9 = 1;
        }
        else {
            LATGbits.LATG9 = 0;
        }
    }
}

int main(void) 
{
    //    ex_1();
    //    ex_2();
    ex_3();
    return 0;
}
