/*
 * File:   timer.h
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>

#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define TIMER4 4


int get_timer_status(int timer);

// set the prescaler
int set_prescaler(int ms);

// number of the prescaler
int number_prescaler(int ms);

// set up the timer to count for the specified amount of milliseconds
void tmr_setup_period(int timer, int ms, int enable_interrupt);

// use the timer flag to wait until it has expired
int tmr_wait_period(int timer);

// use the timer to wait for a given number of millisecond
void tmr_wait_ms(int timer, int ms);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
