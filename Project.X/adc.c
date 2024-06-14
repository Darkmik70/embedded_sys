#include "adc.h"
#include <xc.h>
#include <math.h>



//TODO Add comments
void init_adc() {
    // Remap the pins
    ANSELBbits.ANSB11 = 1;  // battery pin analog
    ANSELBbits.ANSB14 = 1;  // Proximity sensor pin analog
    TRISBbits.TRISB9 = 0;   // set RB9 as output, sensor of proximity
    
    // Setting the ADC converter
    AD1CON3bits.ADCS = 8;   // setting ADC conversion clock 
    AD1CON1bits.ASAM = 1;   // automatic sampling 
    AD1CON3bits.SAMC = 16;  // sampling period 16 Tad
    AD1CON1bits.SSRC = 7;   // automatic sampling, when the conversion should start  
    AD1CON2bits.CHPS = 0;   // number of channels used (1 channel)
    AD1CON2bits.CSCNA = 1;  // scan mode activated
    AD1CON2bits.SMPI = 1;   // Two convertion before the interrupt
    
    AD1CSSLbits.CSS11 = 1;
    AD1CSSLbits.CSS14 = 1;
    
    AD1CON1bits.ADON = 1;   // turn on the ADC module
}


/**
 * Function that convert the corrente? into the voltage or ... 
 * @param adc_value: value of the corrente?
 * @param type: what we want to compute, it could be voltage or temperature etc.
 * @return voltage| ...
 */
float convertTo(int adc_value, char type) {
    float value = ((float)adc_value / ADC_RESOLUTION) * VDD; // Convert ADC value to voltage
    
    switch(type){
        case'V':
            return value*3;     // Voltage divider
        case'M':
            return 2.34 - 4.74*value + 4.06*pow(value,2) -1.60*pow(value,3) + 0.24*pow(value,4);
        default:
            break;
    }
    return 0;
}

float get_battery_voltage(int adc_value) {
        // Convert ADC value to voltage
        float value = ((float)adc_value / ADC_RESOLUTION) * VDD;
        // Voltage divider
        return value*3;
}

float get_ir_distance(int adc_value) {
        // Convert ADC value to voltage
        float value = ((float)adc_value / ADC_RESOLUTION) * VDD; // Convert ADC value to voltage
        return 2.34 - 4.74*value + 4.06*pow(value,2) -1.60*pow(value,3) + 0.24*pow(value,4);
}

