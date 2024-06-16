/*
 * File:   init.h
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */ 
#ifndef INIT_H
#define	INIT_H

#include <xc.h> // include processor files - each processor file is guarded.  

// UART define and functions
#define FCY  72000000
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16) - 1

// ADC define function
#define VDD 3.3     // voltage supplied to the ADC
#define ADC_RESOLUTION 1024 // ADC 10-bit resolution

void init_UART();

void init_ADC();

void init_PWM();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* INIT_H */

