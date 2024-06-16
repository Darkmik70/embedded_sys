/*
 * File:   pwm.h
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

/**
 * Configures OCs for:
 * 1) Forward motion
 * 2) Counterclockwise rotation
 * 3) Clockwise rotation
 * 4) Backward motion
 * Clears of the bits
 * Selects duty cycle with frequency 10 KHz
 * Selects Edge aligned PWM mode
 */
void drive(int type);




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

