/*
 * File:   function.c
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 * Created on March 23, 2024, 12:53 PM
 */

#include "xc.h"
#include "function.h"

/** Function to disabled every analog pins (always), and initialized LED pins as
 * the output and buttons as input pins
 * @param None
 * @return None
 */
void initIO()
{
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // Set pin as output
    TRISAbits.TRISA0 = 0; // set LD1 as an output pin
    TRISGbits.TRISG9 = 0; // set LD2 as an output pin

    // Set pin as input
    TRISEbits.TRISE8 = 1; // T2 button set as input
    TRISEbits.TRISE9 = 1; // T3 button set as input
}

/**
 * Function that turn on the assign LED
 * @param LED_NUMBER
 * @return None
 */
void turnOnLed(int LED_NUMBER)
{
    if (LED_NUMBER == 1)
    {
        LD1_LED = 1;
    }
    if (LED_NUMBER == 2)
    {
        LD2_LED = 1;
    }
}

/**
 * Function that turn off the assign LED
 * @param LED_NUMBER
 * @return None
 */
void turnOffLed(int LED_NUMBER)
{
    if (LED_NUMBER == 1)
    {
        LD1_LED = 0;
    }
    if (LED_NUMBER == 2)
    {
        LD2_LED = 0;
    }
}

/**
 * Function that change the state(toggle) of the LED assign
 * @param LED_NUMBER
 * @return None
 */
void toggleLed(int LED_NUMBER)
{
    if (LED_NUMBER == 1)
    {
        LD1_LED = !LD1_LED;
    }
    if (LED_NUMBER == 2)
    {
        LD2_LED = !LD2_LED;
    }
}