
#include "xc.h"
#include "buttons.h"

// set buttons as inputs
void init_buttons() {
    TRISEbits.TRISE8 = 1; // Set E8 pin as input
    TRISEbits.TRISE9 = 1; // Set E9 pin as input
    
    INTCON2bits.GIE = 1; // Global interrupt enable
}

// enable button interrupts
void set_button_interrupts() {

    enable_t1_interrupt();
    enable_t2_interrupt();
}

// Enable interrupt at RE8
enable_t1_interrupt() {
    RPINR0bits.INT1R = 0x58; //88; // RPI88/RE8
    IFS1bits.INT1IF = 0; // Clear INT1 interrupt flag
    IEC1bits.INT1IE = 1; // Enable INT1 interrupt
}

// Enable interrupt at RE9
void enable_t2_interrupt() {
    RPINR1bits.INT2R = 0x59; //89; // RPI89/RE9
    IFS1bits.INT2IF = 0; // Clear INT2 interrupt flag
    IEC1bits.INT2IE = 1; // Enable INT2 interrupt
}

//// Prototype for an interrupt
//void __attribute__ ((__interrupt__, __auto_psv__)) _INT1Interrupt(void)
//{
//}

//// Prototype for T2
//void __attribute__ ((__interrupt__ , __auto_psv__)) _INT2Interrupt(void) {
//}
