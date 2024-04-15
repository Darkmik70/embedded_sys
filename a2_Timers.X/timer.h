#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define TIMER1 1
#define TIMER2 2

/**
 * @brief Setup the timer for given period
 * 
 * @param timer - Timer (1 or 2)
 * @param ms - period given in miliseconds
 */
void tmr_setup_period(int timer, int ms);

/**
 * @brief Wait until the given timer raises the flag
 *
 * @param timer - TIMER1 or TIMER2
 * @return int - Returns 1 if flag is already raised, otherwise 0;
 */
void tmr_wait_period(int timer);

/**
 * @brief Set up the timer for given period and immediately wait for that period
 * 
 * @param timer 
 * @param ms - the period
 */
void tmr_wait_ms(int timer, int ms)

#endif	/* XC_HEADER_TEMPLATE_H */

