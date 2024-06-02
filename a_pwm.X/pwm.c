
#include <xc.h>
#include "pwm.h"

/**
 * Remap Programmable pins for Output Compare, sets RD1 - RD4 bits as an output
 */
void init_pwm() {
    /* Reprogram the pins  for output */
    TRISDbits.TRISD1 = 0; // RD1 Goes to PWM A
    TRISDbits.TRISD2 = 0; // RD2 Goes to PWM B
    TRISDbits.TRISD3 = 0; // RD3 Goes to PWM C
    TRISDbits.TRISD4 = 0; // RD4 Goes to PWM D
}

/**
 * Remaps pins to set OC1 for Output
 */
void remap_pins_drive_forward() {
    /* map out for  output compare */
    RPOR2bits.RP68R = 0b010000;         // RD4-PWMD outs OC1
    //    RPOR1bits.RP67R =0b010101     // RD3
    RPOR1bits.RP66R = 0b010000;         // RD2-PWMB outs OC1
    //    RPOR0bits.RP65R =             // RD1
}

/**
 * Remaps pins to set OC1 to RD4 and OC2 to RD2
 */
void remap_pins_turning() {
    /* map out for  output compare */
    RPOR2bits.RP68R = 0b010000;         // RD4-PWMD outs OC1
    //    RPOR1bits.RP67R =0b010101     // RD3
    RPOR1bits.RP66R = 0b010001;         // RD2-PWMB outs OC2
    //    RPOR0bits.RP65R =             // RD1
}


/**
 * Generates PWM to go forward.
 * Sets up Output Compare OC1 by: 
 * 1. Clearing the bits
 * 2. remaps the output pins to send OC1 signal
 * 3. Selects Peripheral Clock TCY = 72 Mhz
 * 4. User defined OC1R -> Duty cycle from 40% to 100% for driving
 * 5. User defined OC1Rs -> Period Count for PWM - right now 7200, which gives
 *    1/Period = 10KHz
 */
void drive_forward() {
    // Clear of bits
    OC1CON1 = 0; 
    OC1CON2 = 0;
    
    remap_pins_drive_forward();
    
    OC1CON1bits.OCTSEL = 0x07;      // Select peripheral clock
    
    /* OCxR selects duty cycle*/
    OC1R = 3600;                    // Duty Cycle
    OC1RS = 7200;                   // Determines the Period
    OC1CON2bits.SYNCSEL = 0x1F;     // Source as itself
    
    /* Select and start the Edge Aligned PWM mode*/
    OC1CON1bits.OCM = 6;
}

/**
 * Generate PWM for OC1 and OC2 to drive right.
 * Sets up OC1 and OC2 by:
 *      1. Clear bits
 *      2. remaps bits for turning action
 *      3. Sets up OC1 (40% duty)  and OC2 (60% duty)
 *      4. Start generation.
 */
void drive_right() {
    /* clear bits at the beginning */
    OC1CON1 = 0; OC1CON2 = 0;
    OC2CON1 = 0; OC2CON2 = 0;
    
    remap_pins_turning();
    
    /* OC1 setup */
    OC1CON1bits.OCTSEL = 0x07;
    OC1R = 2900; 
    OC1RS = 7200;
    OC1CON2bits.SYNCSEL = 0x1F;
    
    /* OC2 setup */
    OC2CON1bits.OCTSEL = 0x07; 
    OC2R = 4800;
    OC2RS = 7200;
    OC2CON2bits.SYNCSEL = 0x1F;
    
    /* Select mode (Edge Aligned PWM) and start */
    OC1CON1bits.OCM = 6; 
    OC2CON1bits.OCM = 6;
}


/**
 *  Stops PWM Generation, by clearing the bits for OC1 and OC2
 */
void drive_stop()
{
    // OC1
    OC1CON1 = 0;
    OC1CON2 = 0;
    // OC2
    OC2CON1 = 0;
    OC2CON2 = 0;
}
