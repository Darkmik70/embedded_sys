#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 


#define T1_BUTTON PORTEbits.RE8
#define T2_BUTTON PORTEbits.RE9


// set buttons as inputs
void init_buttons();

// enable button interrupts
void set_button_interrupts();

// Enable interrupt at RE8
void enable_t2_interrupt();

// Enable interrupt at RE9
void enable_t3_interrupt();



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

