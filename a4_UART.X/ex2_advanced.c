/*
 * File:   ex2_advanced.c
 * Author: aless
 *
 * Created on 23 marzo 2024, 12.15
 */

#include "xc.h"
#include "timer.h"
#include "functions.h"
//#include "interrupts_advanced.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>

void algorithm() {
    tmr_wait_ms(TIMER2, 7);
}

void mapInterruptsButton() {
    RPINR0bits.INT1R = 88; // RPI88/RE8
    IFS1bits.INT1IF = 0; // Clear INT1 interrupt flag
    IEC1bits.INT1IE = 1; // Enable INT1 interrupt

    RPINR1bits.INT2R = 89; // RPI89/RE9
    IFS1bits.INT2IF = 0; // Clear INT2 interrupt flag
    IEC1bits.INT2IE = 1; // Enable INT2 interrupt
}

int main() {
    int missedTimer = 0;
    int ret, data;
    int countChar = 0;
    char buffer[3] = {0};
    char toSendC[54] = " CARATTERE=";
    char toSendD[54] = " D=";

    initializeIO();
    set_UART1();
    mapInterruptsButton();

    INTCON2bits.GIE = 1;

    tmr_setup_period(TIMER3, 200);
    tmr_setup_period(TIMER1, 10);

    while (1) {
        algorithm();

        if (IFS1bits.T4IF == 1) {
            // Send the number of character or the number of time the timer expired
            if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
                data = sendNumberChar('C');
                // da mettere il while(UISTAbits.UTXBF==0) manda
                for (int i = 0; i < strlen(toSendC); i++) {
                    while (U1STAbits.UTXBF == 1) {}
                    U1TXREG = toSendC[i];
                }
                sendIntAsChars(data);
                
                IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                IEC1bits.INT1IE = 1; // Enable the interrupt of INT1

            }
            //IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
            //IEC1bits.INT1IE = 1; // Enable the interrupt of INT1

            if (T3_BUTTON == 1 && IFS1bits.INT2IF == 1) {
                data = sendNumberChar('D');

               for (int i = 0; i < strlen(toSendD); i++) {
                    while (U1STAbits.UTXBF == 1) {}
                    U1TXREG = toSendD[i];
                }
                sendIntAsChars(data);
                IFS1bits.INT2IF = 0; // Clear the interrupt flag of INT2
                IEC1bits.INT2IE = 1; // Enable the interrupt of INT1
            }
            //IFS1bits.INT2IF = 0; // Clear the interrupt flag of INT2
            //IEC1bits.INT2IE = 1; // Enable the interrupt of INT1

            IFS1bits.T4IF = 0;
        }

        if (U1STAbits.URXDA == 1) {
            if (countChar < 3) {
                buffer[countChar] = U1RXREG;
            }
            if (countChar > 2) {
                buffer[0] = buffer[1];
                buffer[1] = buffer[2];
                buffer[2] = U1RXREG;
            }
            countChar++;
            receiveMsg(countChar, 'C');
            if (buffer[0] == 'L' && buffer[1] == 'D') {
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
        if (ret == 1) {
            missedTimer++;
            receiveMsg(missedTimer, 'D');
        }
    }
    return 0;
}
