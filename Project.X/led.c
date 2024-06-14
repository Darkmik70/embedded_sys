#include "xc.h"
#include "led.h"
#include "init.h"
#include <math.h>


/** Function to disabled every analog pins (always), and initialized LED pins as  
 * the output and buttons as input pins 
 * @param None
 * @return None
 */
void init_IO() {
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // Set pin as output
    TRISAbits.TRISA0 = 0;   // set LD1 as an output pin
    TRISGbits.TRISG9 = 0;   // set LD2 as an output pin
    TRISBbits.TRISB8 = 0;   // set LDLEFT as an output pin
    TRISFbits.TRISF1 = 0;   // set LDRIGHT as an output pin
    
    // Set pin as input
    TRISEbits.TRISE8 = 1;   // T2 button set as input
    TRISEbits.TRISE9 = 1;   // T3 button set as input 
    
    LD1_LED = 0;
    LD2_LED = 0;
    LD_LEFT = 0;
    LD_RIGHT = 0;
}

/**
 * Function that turn on the assign LED
 * @param LED_NUMBER
 * @return None
 */
void turnOnLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_LED = 1;
    }
    if (LED_NUMBER == 2) {
        LD2_LED = 1;
    }
    if (LED_NUMBER == 3) {
        LD_LEFT = 1;
        LD_RIGHT = 1;
    }
}

/**
 * Function that turn off the assign LED 
 * @param LED_NUMBER
 * @return None
 */
void turnOffLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_LED = 0;
    }
    if (LED_NUMBER == 2) {
        LD2_LED = 0;
    }
    if (LED_NUMBER == 3)
    {
        LD_LEFT = 0;
        LD_RIGHT = 0;
    }
}

/**
 * Function  that change the state(toggle) of the LED assign 
 * @param LED_NUMBER
 * @return None
 */
void toggleLed(int LED_NUMBER) {
    if (LED_NUMBER == 1) {
        LD1_LED = !LD1_LED;
    }
    if (LED_NUMBER == 2) {
        LD2_LED = !LD2_LED;
    }
    if (LED_NUMBER == 3){
        LD_LEFT = !LD_LEFT;
        LD_RIGHT = !LD_RIGHT;
    }
}


// 5Hz is 100ms on e 100 off
// 2.5Hz is 200 ms on e 200 off

