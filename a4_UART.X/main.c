/* EX 2
 * 
 * 1. Simulate a function call that needs 7 ms for its execution, and
 * needs to work at 100 Hz
 * 
 * Make LD2 blink at 2.5 Hz
 * 
 * Read characters from UART1. 
 * Keep a counter of how many chars have you received.
 * If a string "LD1" is received toggle the status of LD1.
 * IF "LD2" is received stop or resume the blinking of LD2
 * 
 * Whenever button T2 is pressed, send "C=xx" 
 * where xx is the current number of chars received to the UART1
 * 
 * Whenever button T3 is pressed send "D=yy"
 * where yy is the number of times the deadline of the tmr_wait_period()
 * was missed
 * */

#include "xc.h"
#include "timer.h"
#include "interrupt.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

// define leds
#define LD1_LED LATAbits.LATA0
#define LD2_LED LATGbits.LATG9

// define BUTTONs
#define T2_BUTTON PORTEbits.RE8
#define T3_BUTTON PORTEbits.RE9

// Uart
#define FCY 72000000
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1

int cnt_c, missedTimer;
void send_int_as_char(int num) {
    // Convert the integer into a sequence of characters
    char buffer[10]; // Assume the integer has at most 10 digits
    sprintf(buffer, "%d", num);

    // Send each character of the sequence
    for (int i = 0; i < strlen(buffer);) {
        
        if (U1STAbits.UTXBF == 0) {
            U1TXREG = buffer[i];
            i++;
        } else {
            while (U1STAbits.UTXBF == 1) {}
        }
    }
}

void set_uart_1() {

    TRISDbits.TRISD11 = 1; // Set RD11 as input
    RPINR18bits.U1RXR = 75; // Map UART1 receive to RP75/RD11

    RPOR0bits.RP64R = 1; //  U1TX transmit to RP64/RD0
    
    //    U1MODEbits.STSEL = 0; // 1 Stop bit
    //    U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
    //    U1MODEbits.ABAUD = 0; // Auto-Baud Disabled
    //    U1MODEbits.BRGH = 0; // Low Speed mode

    U1BRG = BRGVAL; // BAUD Rate Setting for 9600

    // Receiver
    IFS0bits.U1RXIF = 0; // Clear the U1RX interrupt flag
    IEC0bits.U1RXIE = 1; // Enable the U1RX interrupt

    // Transmitter
    //    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX Character is transmitted
    //    U1STAbits.UTXISEL1 = 0;

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX

    U1STAbits.URXISEL = 0; // Receive Interrupt Mode Selection bit
}


void receive_msg(int num, char type) {
    if (type == 'C') {
        cnt_c = num;
    } else if (type == 'D') {
        missedTimer = num;
    }
}

int send_msg(char type) {
    if (type == 'C') {
        return cnt_c;
    } else if (type == 'D') {
        return missedTimer;
    }
    return 1;
}


int cnt_t3;
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt() {
    // Reset the flag
    IFS0bits.T3IF = 0;

    // Toggle LD2 1 for 2.5 Hz
    if (cnt_t3 == 1) {
        // 1 full second should have passed, or circa.
        cnt_t3 = 0;
        LATGbits.LATG9 = !LATGbits.LATG9;
    }
    
    cnt_t3++;
}

//void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt() {
//    IEC1bits.INT1IE = 0; // Disable INT1 interrupt
//    IEC1bits.T4IE = 1; // Enable T4 interrupt
//
//    //Debouncing
//    tmr_setup_period(TIMER4, 10);
//    
//}

void __attribute__ (( __interrupt__ , __auto_psv__ )) _U1RXInterrupt() {
    char data;
    data = U1RXREG;         // Read from RX register
    U1TXREG = 's';         // Send to TX register
    
    IFS0bits.U1RXIF = 0;    // Reset interrupt flag
}


void init_io() {
    // all analog pins disabled
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    TRISAbits.TRISA0 = 0; // Set LD1 pin as output
    TRISGbits.TRISG9 = 0; // Set LD2 pin as output
    TRISEbits.TRISE8 = 1; // Set T2 pin as input
    TRISEbits.TRISE9 = 1; // Set T3 pin as input
}

void set_button_interrupts() {
    RPINR0bits.INT1R = 88; // RPI88/RE8
    IFS1bits.INT1IF = 0; // Clear INT1 interrupt flag
    IEC1bits.INT1IE = 1; // Enable INT1 interrupt

    RPINR1bits.INT2R = 89; // RPI89/RE9
    IFS1bits.INT2IF = 0; // Clear INT2 interrupt flag
    IEC1bits.INT2IE = 1; // Enable INT2 interrupt
}

void algorithm() {
    tmr_wait_ms(TIMER2, 7);
}

int main() {
    int data;
    int buffer[3];
    int cnt_chars_received = 0;     // How many chars are received
    int cnt_missed_deadline = 0;
    cnt_t3 = 0;
    char send_c[54] = "C==", send_d[54] = "D==" ;
    
    
    init_io();
    set_button_interrupts();
    
    //init LD2 as 0
    LATGbits.LATG9 = 0; 
    

    tmr_setup_period(TIMER3, 200);      // Used to make LD2 blink at 2.5hz
    tmr_setup_period(TIMER1, 10);

    while (1) {
        algorithm(); // wait 7ms
        
        // Timer 4 interrupt handling
        if (IFS1bits.T4IF == 1) {
            // Send the number of character or the number of time the timer expired
            if (T2_BUTTON == 1 && IFS1bits.INT1IF == 1) {
                data = send_msg('C');
                for (int i = 0; i < strlen(send_c); i++) {
                    while (U1STAbits.UTXBF == 1) {
                    }
                    U1TXREG = send_c[i];
                }
                send_int_as_char(data);

                IFS1bits.INT1IF = 0; // Clear the interrupt flag of INT1
                IEC1bits.INT1IE = 1; // Enable the interrupt of INT1

            }

            if (T3_BUTTON == 1 && IFS1bits.INT2IF == 1) {
                data = send_msg('D');

               for (int i = 0; i < strlen(send_d); i++) {
                    while (U1STAbits.UTXBF == 1) {}
                    U1TXREG = send_d[i];
                }
                send_int_as_char(data);
                IFS1bits.INT2IF = 0; // Clear the interrupt flag of INT2
                IEC1bits.INT2IE = 1; // Enable the interrupt of INT1
            }
            IFS1bits.T4IF = 0;
        }

        // UART receives something
        if (U1STAbits.URXDA == 1) {
            if (cnt_chars_received < 3) {
                buffer[cnt_chars_received] = U1RXREG;
            }
            if (cnt_chars_received > 2) {
                buffer[0] = buffer[1];
                buffer[1] = buffer[2];
                buffer[2] = U1RXREG;
            }
            cnt_chars_received++;
            receive_msg(cnt_chars_received, 'C');
            if (buffer[0] == 'L' && buffer[1] == 'D') {
                // Perform action based on the third character
                switch (buffer[2]) {
                    case '1':
                        // Toggle LD1
                        LATAbits.LATA0 = !LATAbits.LATA0;
                        break;
                    case '2':
                        // stop or 
                        IEC0bits.T3IE = !IEC0bits.T3IE;
                        if (LATGbits.LATG9 == 1) 
                            LATGbits.LATG9 = 0;
                        break;
                    default:
                        // Handle unknown command or error
                        break;
                }
            }
        }
        int ret = tmr_wait_period(TIMER1);
        //every time ret != 0 the counter will increment
        if (ret == 1) {
            cnt_missed_deadline++;
            receive_msg(cnt_missed_deadline, 'D');
        }
    }
}