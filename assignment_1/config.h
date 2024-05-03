// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define TIMER4 4

#define MAX_VALUE_16_BITS 65535
#define FCY_PROP 72000  // Proportional value: FCY/1000
#define BAUDRATE 9600
#define ARRAY_SIZE 5


void tmr_wait_ms(int timer, int ms);
void tmr_setup_period(int timer, int ms, int has_interrupt);
int tmr_wait_period(int timer);
void tmr_set_register_values(int timer, int pr_value, int prescaler_bits, int has_interrupt);

void spi_setup();
void uart_setup();
void magnetometer_config();

int get_axis_mag_value(int addr);
double calculate_average(int values[], int size);

int spi_write(int addr);


#endif