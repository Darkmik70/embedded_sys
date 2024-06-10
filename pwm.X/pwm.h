/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

/**
 * Remap Programmable pins for Output Compare, sets RD1 - RD4 bits as an output
 */
void init_pwm();

/**
 * Driving forward
 * Remaps pins to set OC1 as output
 * RD4 - PWM D - Motors on the right
 * RD2 - PWM B - Motors on the left
 */
void remap_pins_drive_forward();

/**
 * Driving in reverse
 * Remaps pins to set OC1 as output
 * RD3 - PWM C - Motors on the right
 * RD1 - PWM A - Motors on the left
 */
void remap_pins_drive_backward();

/**
 * Counter-clockwise rotation
 * Remaps pins to set OC1 as output
 * RD4 - PWM D - Motors on the right forward
 * RD1 - PWM A - Motors on the left in reverse
 */
void remap_pins_ccw();

/**
 * clockwise rotation
 * Remaps pins to set OC1 as output
 * RD3 - PWM C - Motors on the right in reverse
 * RD2 - PWM B - Motors on the left forward
 */
void remap_pins_cw();

/**
 * driving and turning right (movement as an arc)
 * Remaps pins to set OC1 as output
 * RD3 - PWM C - Motors on the right in reverse
 * RD2 - PWM B - Motors on the left forward
 */
void remap_pins_turning();

/**
 * Perform a counter-clockwise rotation
 * Remaps pins to set OC1 as output
 * Configures OC1
 */
void rotate_ccw();

/**
 * Perform a clockwise rotation
 * Remaps pins to set OC1 as output
 * Configures OC1
 */
void rotate_cw();

/**
 * Configures OC1 for rotating
 * Clears of the bits for safety, sets peripheral clock
 * Selected duty cycle 100% with frequency 10 KHz
 * Selects Edge aligned PWM mode
 */
void rotate();

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
void drive_forward();

/**
 * Generate PWM for OC1 and OC2 to drive right.
 * Sets up OC1 and OC2 by:
 *      1. Clear bits
 *      2. remaps bits for turning action
 *      3. Sets up OC1 (40% duty)  and OC2 (60% duty)
 *      4. Start generation.
 */
void drive_right();

/**
 *  Stops PWM Generation, by clearing the bits for OC1 and OC2
 */
void drive_stop();




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

