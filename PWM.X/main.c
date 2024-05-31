/*
 * File:   main.c
 * Author: michalkrepa
 *
 * Created on May 31, 2024, 9:13 PM
 */

/* Task 1 */
// When button E8 is pressed, 
// start generating a PWM signal with the Output Compare peripheral at a 
// 10 kHz frequency to make the buggy move forward. 
// When the same button E8 is pressed again, stop generating the PWM signal.
// Hint : the duty cycle range to enable motion is between 40% and 100%.

#include "xc.h"
#include "io_led.h"
#include "buttons.h"
#include "timer.h"


// Interrupt for E8
void __attribute__ ((__interrupt__, __auto_psv__)) _INT1Interrupt(void)
{
    // Disable interrupt
    IEC1bits.INT1IE = 0;
    // Reset interrupt flag
    IFS1bits.INT1IF = 0;
    
    toggle_led(LD1_LED);
    
    // Debouncing
    tmr_setup_period(2, 20, 1); // t2, 20ms, IE enable
}

// timer 2 interrupt to stop debouncing
void __attribute__ ((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    // Reset the flag
    IFS0bits.T2IF = 0;
    // Disable Interrupt
    IEC0bits.T2IE = 0; 
    
    
    // Enable INT1 interrupt
    IEC1bits.INT1IE = 1;
}

int main(void) {
    // Disable analog pins, init t1,t2, enable GIE & T1
    init_io();
   
    init_buttons();
    enable_t1_interrupt();
    // setup PWM
    
    while(1)
    {
        
    }
    
    return 0;
}
