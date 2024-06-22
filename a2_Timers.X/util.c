/*
 * File:   util.c
 * Author: michalkrepa
 *
 * Created on April 14, 2024, 7:52 PM
 */


#include "xc.h"
#include "util.h"

// LEDs 
#define LD1_LED LATAbits.LATA0
#define T2_BUTTON PORTEbits.RE8

void init_io_ports()
{
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // LEDs
    // '0' for Output '1'for Input
    TRISAbits.TRISA0 = 0;    // Set RA0-LD1 as an output.
    TRISEbits.TRISE8 = 1;    // Set T2 Button as input
    
}
