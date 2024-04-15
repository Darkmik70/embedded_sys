#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define TIMER1 1
#define TIMER2 2

// 
/**
 * Setup the timer period. Must be only used once in the code
 * @param timer
 * @param ms
 */
void tmr_setup_period(int timer, int ms);

void tmr_wait_period(int timer);




#endif	/* XC_HEADER_TEMPLATE_H */

