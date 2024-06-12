/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on March 12, 2024, 4:48 PM
 */

#include "xc.h"
#include "timer.h"
#include "interrupts.h"

void blinkLED(int timer, int ms){
    tmr_wait_ms(timer,ms);
    LATAbits.LATA0 = !LATAbits.LATA0;
}

// Remap the external interrupt
void mapINT1(){
    // Assign an interrupt to the corresponding pin bits
    RPINR0bits.INT1R = 0x58;    //RPI88 in hex (88)
    
    // we know is in this register beacuse is written in the datasheet
    INTCON2bits.GIE = 1;    // set global interrupt enable
    //INTCON2bits.INT1EP = 1; // Interrupt on negative edge
    
    IFS1bits.INT1IF = 0;        // clear the interrupt flag
    IEC1bits.INT1IE = 1;        // enable interrupt
}

//Exercise n.2
int main(void){
    
    // all the analog are disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // initialized the function of the pins
    TRISAbits.TRISA0 = 0;   // set pin as output
    TRISGbits.TRISG9 = 0;   // set pin as output
    TRISEbits.TRISE8 = 1;   // set pin as input
    
    // clear the output pins
    LATAbits.LATA0 = 0;
    LATGbits.LATG9 = 0;
    
    // Mapping te INT1 to RE8 pin left button
    mapINT1();
    
    int delay = 200;
    int state = 0;
    
    while(1){
        state = PORTEbits.RE8;
        tmr_wait_ms(TIMER1,delay);
        LATAbits.LATA0 = !LATAbits.LATA0;        // Blink LED1 without using interrups
    }
    return 0;
}


/* // Exercise n.1
int main(void) {
    
    // all the analog are disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // initialized the function of the pins
    TRISAbits.TRISA0 = 0;   // set pin as output
    TRISGbits.TRISG9 = 0;   // set pin as output
    
    // set the pins low
    LATAbits.LATA0 = 0;
    LATGbits.LATG9 = 0;
    
    while(1){
        blinkLED(TIMER1, 200);      // Blink LED1 without using interrups
        tmr_wait_ms(TIMER2, 100);   // Blink LED2 using interrups
    }
    return 0;
}
*/