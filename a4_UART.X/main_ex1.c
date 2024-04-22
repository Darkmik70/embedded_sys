/*
 * File:   main.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:41 PM
 */
#include "xc.h"
#include "timer.h"



/// Ex 2
// Blink LD2 at 2.5 Hz (200ms on, 200 ms off) without using interrupts
// Every time T2 is pressed toggle the led LD2 using interrupts


// Blink LD1 at 2.5 Hz (200ms on, 200 ms off) without using interrupts
// Then make LD2 blink at 1Hz frequency using interrupts
void ex_1() 
{
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    // Set LD1 LED as output
    TRISAbits.TRISA0 = 0;
    // Set LD2 LED as output
    TRISGbits.TRISG9 = 0;

    // Start with LED1 being off
    LATAbits.LATA0 = 0;  
    // Setup LD 2
    LATGbits.LATG9 = 0;  

    // setup Timer1 for 200 ms.
    tmr_setup_period(TIMER1, 200);
    
    tmr_setup_period(TIMER2, 1000);


    while(1)
    {  
        // Toggle LD1
        tmr_wait_period(TIMER1);
        LATAbits.LATA0 = !LATAbits.LATA0;  

 
    }
}

int main(void) 
{
    //    ex_1();
    //    ex_2();
    ex_1();
    return 0;
}
