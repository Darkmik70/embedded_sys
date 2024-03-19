/*
 * File:   ex1.c
 * Author: RomiC
 *
 * Created on February 27, 2024, 4:48 PM
 */


#include "xc.h"


int main(void) {
    
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
    
    // Initialized pin function
    TRISAbits.TRISA0 = 0;   // set pin as output
    TRISEbits.TRISE8 = 1;   // set pin as input
    
    // initialized  the variables 
    int T2;         // variable where we put the value of the pin
    int pinValue;   // actual value of the pin
    int state = 0;  // variable to see if the state is change
      
    pinValue = 0;   // initial value of the pin
    
    while(1){
        T2 = PORTEbits.RE8;
        if (T2 == 1){
            if (state!=T2){
                if(pinValue == 0){
                    LATAbits.LATA0 = 1;
                    pinValue = 1;
                }
                else{ 
                    LATAbits.LATA0 = 0;
                    pinValue = 0;
                }
                state= T2;
            }
        }
        else{
            state = T2;
        }
    }
    return 0;
}
    
/* Exercise n.1
int main_uno(void) {
    
    // all the analog are disabled 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
   
    TRISEbits.TRISE8 = 1; //set as input
    TRISAbits.TRISA0 = 0; // set as output  
    
    LATAbits.LATA0 = 1;
            
    while(1){}
    return 0;
}
*/
    
/* Exercise n.2
int main_due(void) {
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;
 
    // initialized the function of the pins 
    TRISEbits.TRISE8 = 1; //set as input
    TRISAbits.TRISA0 = 0; // set as output
   
    // initialized the variable 
    int T2;
    
    while(1){
        T2 = PORTEbits.RE8; // when having 0 is press
        if (T2 == 0){
            LATAbits.LATA0 = 1; // set the pin high  
        }
        else{
            LATAbits.LATA0 = 0; // set the pin low
        }
    }
    return 0;
}
 */
 
