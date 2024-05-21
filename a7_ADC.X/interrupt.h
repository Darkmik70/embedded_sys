#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt();

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt();


// Timer 3 interrupt
//void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt();

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt();


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

