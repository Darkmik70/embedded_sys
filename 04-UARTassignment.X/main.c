/*
 * File:   main.c
 * Author: RomiC
 *
 * Created on April 10, 2024, 11:39 PM
 */


#include "xc.h"
#include "function.h"
#include "timer.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>

// Exercise n.2

/**
 * Use to simulate an algorithm that takes 7 ms to be completed
 * @param None
 * @return None
 */
void algorithm(){
    tmr_wait_ms(TIMER2, 7);
}

/**
 * This function assign the pins of the button to the external interrupts
 * @param None
 * @return None  
 */
void mapInterruptsButton(){
    // Assign an interrupt INT1 to the corresponding pin 
    RPINR0bits.INT1R = 88;  //RPI88 in hex (88)
    INTCON2bits.GIE = 1;    // set global interrupt enable
    IFS1bits.INT1IF = 0;    // clear the interrupt flag
    IEC1bits.INT1IE = 1;    // enable interrupt
    
    // Assign an interrupt INT2 to the corresponding pin
    RPINR1bits.INT2R = 89;  //RPI88 in hex (89)
    INTCON2bits.GIE = 1;    // set global interrupt enable
    IFS1bits.INT2IF = 0;    // clear the interrupt flag INT2
    IEC1bits.INT2IE = 1;    // enable external interrupt iNT2
}



// Exercise n.2
int main(){
    int ret, data; 
    int missedTimer = 0; 
    int countChar = 0;
    char buffer[3] = {0};
    char toSendC[54] = "CHARACTER=", toSendD[54] = " D=";
    
    initializeIO(); 
    initUART();
    mapInterruptsButton();
   
    //INTCON2bits.GIE = 1;    // set global interrupt enable
    
    tmr_setup_period(TIMER3, 200);  // timer set to have a blinking every 200 ms
    tmr_setup_period(TIMER1, 10);
    
    
    while(1){

        algorithm();
        
        if (IFS1bits.T4IF == 1) {
            // Send the number of character or the number of time the timer expired
            if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
                data = sendNumberChar('C');
                for (int i = 0; i < strlen(toSendC); i++) {
                    while (U1STAbits.UTXBF == 1) {}
                    U1TXREG = toSendC[i];
                }
                sendIntAsChar(data);
                
                IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                IEC1bits.INT1IE = 1; // Enable the interrupt of INT1

            }
           
            if (T3_BUTTON == 1 && IFS1bits.INT2IF == 1) {
                data = sendNumberChar('D');

               for (int i = 0; i < strlen(toSendD); i++) {
                    while (U1STAbits.UTXBF == 1) {}
                    U1TXREG = toSendD[i];
                }
                sendIntAsChar(data);
                IFS1bits.INT2IF = 0; // Clear the interrupt flag of INT2
                IEC1bits.INT2IE = 1; // Enable the interrupt of INT2
            }
            
            IFS1bits.T4IF = 0;
        }
        
        if(U1STAbits.URXDA == 1){
            if(countChar < 3){
                buffer[countChar] = U1RXREG;
            }
            if  (countChar > 2 ){
                buffer[0] = buffer[1];
                buffer[1] = buffer[2];
                buffer[2] = U1RXREG;
            }
            /* if(countChar > 2){
                for (int i = 0; i < 3; i++) {
                    U1TXREG = buffer[i];
                }
            }
             */
            countChar++;
            receiveMsg(countChar, 'C');
            if ( buffer[0] == 'L' && buffer[1] == 'D') {
                // Perform action based on the third character
                switch (buffer[2]) {
                    case '1':
                        toggleLed(1);
                        break;
                    case '2':
                        IEC0bits.T3IE = !IEC0bits.T3IE;
                        break;
                    default:
                        // Handle unknown command or error
                        break;
                }
            }
        }
        
        ret = tmr_wait_period(TIMER1);
        if (ret==1){
            missedTimer++;
            receiveMsg(missedTimer, 'D');
        }
    }
    return 0;
}


/* // Exercise n.1
int main(void){
    
    // Inizialize the input and output pins 
    initializeIO();
    
    // assign an interrupt to the corresponding pin bits
    assignUART();
    
    while(1){
    }
    return 0;
}
*/ 
