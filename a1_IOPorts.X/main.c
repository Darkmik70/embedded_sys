/*
 * File:   main.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 12:53 PM
 */
#include <xc.h>


// Turn on the led LD1
void ex_1()
{
    // all analog pins disabled
    //ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // Set RA0-LD1 as an output.
    TRISAbits.TRISA0 = 0; // '0' for Output '1'for Input
    // set value of Latch of RA0-LD1 to 1. This will make it on.
    LATAbits.LATA0 = 1;

    // Enter loop
    while(1);
}

// Read the button T2, while the button is pressed turn on the led LD1
void ex_2() 
{
    /* THIS DOES NOT SAVE FROM BOUNCING OF THE SWITCH */
    // Set T2 Button as input
    TRISEbits.TRISE8 = 1;
    // Set LD1 LED as output
    TRISAbits.TRISA0 = 0;
   
    while(1)
    {
        // while Button is pressed turn on the led. Otherwise turn it off.
        if (PORTEbits.RE8 == 0) // Read Port for T2 Button 0 is for pressed
        {
            LATAbits.LATA0 = 1; // While button is pressed turn on the led
        }
        else
        {
            LATAbits.LATA0 = 0;
        }  
    }
}

// Toggle LED when button is pressed.
void ex_3()
{
    // Set T2 Button as input
    TRISEbits.TRISE8 = 1;
    // Set LD1 LED as output
    TRISAbits.TRISA0 = 0;
   
    while(1)
    {
        // whenever Button is pressed toggle the status of the led. 
        if (PORTEbits.RE8 == 0) // Read Port for T2 Button 0 is for pressed
            /*This is not bounce-proof*/
        {
            // Toggle the status of LD1
            LATAbits.LATA0 = !LATAbits.LATA0;   
        }
    }
}

int main(void)
{
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
   //ex_1();
   //ex_2();
   //ex_3();
    
    return 0;
}
