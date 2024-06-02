/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 


void init_pwm();

void remap_pins_drive_forward();

void remap_pins_turning();


void drive_forward();

void drive_right();

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

