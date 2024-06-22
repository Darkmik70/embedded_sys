/* Microchip Technology Inc. and its subsidiaries.  You may use this software
 * and any derivatives exclusively with Microchip products.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*
 * File:   Init
 * Comments: This are the initialization function of the project and also some
 * function use to configurate the mikro. We have to make also the circular
 * and all the interrupts thing.
 * Revision history: 1.00 Version
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef INIT_H
#define INIT_H

#include <xc.h> // include processor files - each processor file is guarded.

// UART define
#define FCY 72000000
#define BAUDRATE 9600
#define BRGVAL ((FCY / BAUDRATE) / 16) - 1

// UART function
void initUART();

// Define Chip Selectors for the SPI
#define CS1 LATBbits.LATB3
#define CS2 LATBbits.LATB4
#define CS3 LATDbits.LATD6

// SPI functions
void initSPI();
int16_t spi_write(unsigned int addr);
void setACTIVEmode(char type);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C
    // linkage so the functions can be used by the c code.

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* XC_HEADER_TEMPLATE_H */
