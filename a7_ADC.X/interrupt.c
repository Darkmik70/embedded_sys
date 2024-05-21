#include <xc.h>
#include "interrupt.h"
#include "timer.h"



/* TIMERS */
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt() {
    IFS0bits.T1IF = 0; // Clear T1 interrupt flag
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt() {
    IFS0bits.T2IF = 0; // Clear T2 interrupt flag
}

//int cnt_t3;
//void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt() {
//    // Reset the flag
//    IFS0bits.T3IF = 0;
//
//    // Toggle LD2 1 for 2.5 Hz
//    if (cnt_t3 == 1) {
//        // 1 full second should have passed, or circa.
//        cnt_t3 = 0;
//        LATGbits.LATG9 = !LATGbits.LATG9;
//    }
//    
//    cnt_t3++;
//}

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt() {
    IEC1bits.T4IE = 0; // Disable the interrupt of the timer 4
    T4CONbits.TON = 0; // Stop timer 4
    //IFS1bits.T4IF = 0;
}