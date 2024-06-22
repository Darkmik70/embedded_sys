/*
 * File:   adc.h
 * Author: Bua Odetti, Contreras, Krepa, Trovatello
 *
 * Created on 30 maggio 2024, 12.00
 */
#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

#define VDD 3.3     // voltage supplied to the ADC
#define ADC_RESOLUTION 1024 // ADC 10-bit resolution

//TODO Add comments
void initADC();

/**
 * Function that convert the corrente? into the voltage or ... 
 * @param adc_value: value of the corrente?
 * @param type: what we want to compute, it could be voltage or temperature etc.
 * @return voltage| ...
 */
float convertTo(int adc_value, char type);

float get_battery_voltage(int adc_value);

float get_ir_distance(int adc_value);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ADC_H */

