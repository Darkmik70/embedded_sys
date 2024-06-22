#include "xc.h"
#include "function.h"
#include "timer.h"
#include "init.h"
#include "pwm.h"

int is_pwm_on; // Flag - 1: PWM is generated 0: NO


Interrupt routine for pressed button

void __attribute__((__interrupt__, __auto_psv__))_INT1Interrupt(void) {
    IFS1bits.INT1IF = 0; // clear the interrupt flag
    IEC1bits.INT1IE = 0; // disable interrupt

    is_pwm_on = !is_pwm_on;

    if (is_pwm_on) {
        LATAbits.LATA0 = 1;
        drive_forward();

    } else {
        LATAbits.LATA0 = 0;
        drive_stop();
    }

    tmr_setup_period(TIMER2, 20, 1);
}

// Debouncing using two interrupts
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;      // clear the flag on the interrupt 2
    IEC0bits.T2IE = 0;      // disable the interrupt of the timer 2
    
    IEC1bits.INT1IE = 1;
//    LATAbits.LATA0 = !LATAbits.LATA0;   
}

/* Task 1 */
// When button E8 is pressed, 
// start generating a PWM signal with the Output Compare peripheral at a 
// 10 kHz frequency to make the buggy move forward. 
// When the same button E8 is pressed again, stop generating the PWM signal.
// Hint : the duty cycle range to enable motion is between 40% and 100%.
int main()
{
    is_pwm_on = 0; // Flag - 1: PWM ON, 0: OFF
    
    initIO();
    
    /*Enable interrupts in INT1*/
    RPINR0bits.INT1R = 0x58;    //RPI88 in hex (88)
    INTCON2bits.GIE = 1;        // set global interrupt enable
    IFS1bits.INT1IF = 0;        // clear the interrupt flag
    IEC1bits.INT1IE = 1;        // enable interrupt

    init_pwm();

    while (1)
    {
    }
    
    return 0;
}
