 #ifndef LED_H
#define	LED_H

#include <xc.h> 

#define LD1_LED LATAbits.LATA0
#define LD2_LED LATGbits.LATG9

#define LD_LEFT LATBbits.LATB8
#define LD_RIGHT LATFbits.LATF1

// define BUTTONs
#define T2_BUTTON PORTEbits.RE8
#define T3_BUTTON PORTEbits.RE9

void initializeIO();

void turnOffLed(int LED_NUMBER);

void turnOnLed(int LED_NUMBER);

void toggleLed(int LED_NUMBER);

void drive(char type, int time);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* FUNCTION_H */

