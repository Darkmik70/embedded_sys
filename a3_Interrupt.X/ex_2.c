/*
 * File:   main.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:41 PM
 */
#include "xc.h"
#include "timer.h"



/// Ex 2
// Blink LD1 at 2.5 Hz (200ms on, 200 ms off) without using interrupts
// Every time T2 is pressed toggle the led LD2 using interrupts
void __attribute__ ((__interrupt__, __auto_psv__)) _INT1Interrupt(void)
{
    // reset flag
    IFS1bits.INT1IF = 0;
    
    // Toggle LD2
    LATGbits.LATG9 = !LATGbits.LATG9;
}


int main(void) 
{
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    // Set LD1 LED as output
    TRISAbits.TRISA0 = 0;
    // Set LD2 LED as output
    TRISGbits.TRISG9 = 0;
    
    /* Button configuration */
    TRISEbits.TRISE9 = 1;
    RPINR0bits.INT1R = 0x59; // 0x59 is 89 in hex -> refers to RE9/RPI89
    
    INTCON2bits.GIE = 1;    // enable global interrupt
    IFS1bits.INT1IF = 0;    // clear flag
    IEC1bits.INT1IE = 1;    // enable the interrupt
    
    // Start with LED1 being off
    LATAbits.LATA0 = 0;  
    // Setup LD 2
    LATGbits.LATG9 = 0;  

    // setup Timer1 for 200 ms.
    tmr_setup_period(TIMER1, 200);
   

    while(1)
    {  
        tmr_wait_period(TIMER1);
        // Toggle LD1
        LATAbits.LATA0 = !LATAbits.LATA0;  
    }
    
    return 0;
}
