#include "config.h"


// Global Variables
int init_UART_tx = 0;
char buffer_tx[40];
int buffer_index = 0;

int x_values[ARRAY_SIZE];
int y_values[ARRAY_SIZE];
int z_values[ARRAY_SIZE];
int array_index = 0;


/////////////////////////////////////
// TODO: Use TX Interrupts for UART
////////////////////////////////////


void __attribute__ ((__interrupt__ , __auto_psv__)) _T1Interrupt(void) {
    // Reset interrupt flag
    IFS0bits.T1IF = 0;  
    
    // Get values from SPI
    int x_value = get_axis_mag_value(0x42);
    x_values[array_index] = x_value;
    int y_value = get_axis_mag_value(0x44);
    y_values[array_index] = y_value;
    int z_value = get_axis_mag_value(0x44);
    z_values[array_index] = z_value;
    // Change index dynamically
    array_index = (array_index + 1) % ARRAY_SIZE;
    
    
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _T2Interrupt(void) {
    // Clear interrupt flag
    IFS0bits.T2IF = 0; 
    // Change global variable
    init_UART_tx = 1;
}

void __attribute__ ((__interrupt__ , __auto_psv__)) _U1TXInterrupt(void){
    // Clear TX interrupt flag 
    IFS0bits.U1TXIF = 0;
    
    char current_char = buffer_tx[buffer_index];
    if (current_char == '\0'){return;}  // String already sent entirely
    U1TXREG = current_char;
    buffer_index++;
    
}


void algorithm(){
    tmr_wait_ms(TIMER4, 7);
}


int main(void) {
    
    // For testing
    TRISGbits.TRISG9 = 0;   // pin G9 set as output (LED 2)
    
    // Setup and configuration
    uart_setup();
    spi_setup();
    magnetometer_config();

    // Acquire magnetometer data frequency (25Hz)
    tmr_setup_period(TIMER1, 40, 1);
    // Send to UART frequency (5Hz)
    tmr_setup_period(TIMER2, 200, 1);
    // Loop frequency (100Hz)
    tmr_setup_period(TIMER3, 10, 0);
    
    while(1){

        // Main algorithm (takes 7 ms)
        algorithm();

        // Rest of code
        if (init_UART_tx){
            // Calculate the averages
            double x_mag = calculate_average(x_values, ARRAY_SIZE);
            double y_mag = calculate_average(y_values, ARRAY_SIZE);
            double z_mag = calculate_average(z_values, ARRAY_SIZE);

            // Calculate the magnetic north
            double angle = atan2(y_mag, x_mag);
            
            // Update the string to the buffer
            memset(buffer_tx, 0, sizeof(buffer_tx)); // Clear buffer
            sprintf(buffer_tx, "$MAG=%.2f,%.2f,%.2f*$YAW,%.2f*", x_mag, y_mag, z_mag, angle);

            // Begin the transmission of the first character
            buffer_index = 0;
            U1TXREG = buffer_tx[buffer_index];
            buffer_index++;

            // Zero-out variable
            init_UART_tx = 0;
        }

        // Wait for timer to reach desired frequency (100Hz)
        int ret = tmr_wait_period(TIMER4); 
        if (ret) {LATGbits.LATG9 = 1;}  // Turn on LED if flag was already up.

    }

    return 0;
    
}


void uart_setup(){
    
    // Remapping of UART1 to pins of MicroBus 2
    RPINR18bits.U1RXR = 0x4B;
    RPOR0bits.RP64R = 1;
    IEC0bits.U1RXIE = 1;
    
    // UART Configuration
    U1BRG = 468;                // Obtained from (FCY/(16*baud_rate) - 1)
    U1STAbits.UTXISEL0 = 0;     // Interrupt after one TX Character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 1;        // Enable UART TX Interrupt
    IFS0bits.U1TXIF = 0;        // clear TX interrupt flag
    U1MODEbits.UARTEN = 1;      // Enable UART
    U1STAbits.UTXEN = 1;        // Enable U1TX
}


void spi_setup(){
    // Port configuration: CS Lines
    TRISBbits.TRISB3 = 0;   // CS1: Accelerometer
    TRISBbits.TRISB4 = 0;   // CS2: Gyroscope
    TRISDbits.TRISD6 = 0;   // CS3: Magnetometer
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATDbits.LATD6 = 1;
    
    // Port configuration: SPI Lines
    TRISAbits.TRISA1 = 1; // RA1?RPI17 MISO
    TRISFbits.TRISF12 = 0; //RF12?RP108 SCK
    TRISFbits.TRISF13 = 0; // RF13?RP109 MOSI
    
    // Remapping of SPI1 to pins of Microbus1
    RPINR20bits.SDI1R = 0b0010001; // MISO (SDI1) ? RPI17
    RPOR12bits.RP109R = 0b000101; // MOSI (SDO1) ? RF13;
    RPOR11bits.RP108R = 0b000110; // SCK1;
    
    // SPI Configutation
    SPI1CON1bits.MSTEN = 1;      // Master mode
    SPI1CON1bits.MODE16 = 0;     // 8-bit mode
    SPI1CON1bits.CKP = 1;        // Idle value of clock
    // [Prescaler] FSCK = (FCY)/(PPR * SPR) = 72M/(4*3) = 6 MHz
    SPI1CON1bits.PPRE = 0b10;    // Primary prescaler 4:1
    SPI1CON1bits.SPRE = 0b101;   // Secondary prescaler 3:1
    SPI1STATbits.SPIEN = 1;     // Enable SPI
    SPI1STATbits.SPIROV = 0;    // Clear overflow flag
    
}


int spi_write(int addr){
    // Transmit segment
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = addr;
    // Receive segment
    while (SPI1STATbits.SPIRBF == 0);
    int response = SPI1BUF;
    return response;
}


void magnetometer_config(){

    int sleep_mode_addr = 0x4B;
    int active_mode_addr = 0x4C;

    // Make magnetometer switch to Sleep Mode
    LATDbits.LATD6 = 0;
    spi_write(sleep_mode_addr);
    spi_write(0x01);
    LATDbits.LATD6 = 1;
    // Wait 2 ms
    tmr_wait_ms(TIMER1, 2);

    // Make magnetometer go to active mode
    LATDbits.LATD6 = 0;
    spi_write(active_mode_addr);
    spi_write(0x00);
    LATDbits.LATD6 = 1;

    // Configure the data rate
    LATDbits.LATD6 = 0;
    spi_write(0x4C);
    spi_write(0b00110000);  // To set it to 25Hz
    LATDbits.LATD6 = 1;

}

int get_axis_mag_value(int addr){

    LATDbits.LATD6 = 0;
    spi_write(addr | 0x80);
    int LSB_byte = spi_write(0x00);
    int MSB_byte = spi_write(0x00);
    LATDbits.LATD6 = 1;
    
    // Define mask to clear three least significant bits
    int mask = 0b11111000;
    // Clear three least significant bits of LSB_byte
    int masked_LSB = LSB_byte & mask;
    // Left-shift MSB_byte by 8 and OR with masked LSB_byte
    int combined_byte = (MSB_byte << 8) | masked_LSB;
    // Divide by 8 to get correct scale
    int final_byte = combined_byte / 8;

    return final_byte;
}


void tmr_wait_ms(int timer, int ms){

    // List of prescaler values: {ratio, bits}
    int prescaler[][2] = {
        {1, 0}, {8, 1}, {64, 2}, {256, 3}
    };
    
    // CONSTANTS FOR PRESCALER AND TIMER PR2 VALUE
    int prescaler_bits;
    long int pr_value;
    long int temp_pr_value;

    // OBTAINING THE PR VALUE
    for (int i = 0; i < 4; i++) {
        temp_pr_value = (FCY_PROP * ms) / (prescaler[i][0]);
        prescaler_bits = prescaler[i][1];
        if (temp_pr_value < MAX_VALUE_16_BITS) {break;}
    }
    
    // SETUP & WAITING
    while (temp_pr_value > 0){
        
        if(temp_pr_value >= MAX_VALUE_16_BITS) {pr_value = MAX_VALUE_16_BITS;}
        else {pr_value = temp_pr_value;}

        temp_pr_value = temp_pr_value - MAX_VALUE_16_BITS;
            
        // SETUP THE APPROPRIATE TIMER REGISTER VALUES
        tmr_set_register_values(timer, pr_value, prescaler_bits, 0);
        
        // BUSY WAITING FOR FLAG
        if (timer == 1){
            while(!IFS0bits.T1IF);      // busy waiting
            IFS0bits.T1IF = 0;          // Clear the interrupt flag
        }
        else if (timer == 2){
            while(!IFS0bits.T2IF);   
            IFS0bits.T2IF = 0;      
        }
        else if (timer == 3){
            while(!IFS0bits.T3IF);  
            IFS0bits.T3IF = 0;        
        }
        else if (timer == 4){
            while(!IFS1bits.T4IF);    
            IFS1bits.T4IF = 0;          
        }
    }
}



void tmr_set_register_values(int timer, int pr_value, int prescaler_bits,
                             int has_interrupt){
    if (timer == 1){
        TMR1 = 0;                                   // Reset timer counter
        PR1 = pr_value;                             // Timer will count up to this value
        T1CONbits.TCKPS = prescaler_bits;           // Prescaler bits (0,1,2,3)
        IFS0bits.T1IF = 0;                          // Clear the interrupt flag
        if (has_interrupt) {IEC0bits.T1IE = 1;}     // Enable timer interrupt
        T1CONbits.TON = 1;                          // Start the timer
    }
    else if (timer == 2){
        TMR2 = 0;                   
        PR2 = pr_value;                   
        T2CONbits.TCKPS = prescaler_bits;   
        IFS0bits.T2IF = 0;
        if (has_interrupt) {IEC0bits.T2IE = 1;}     
        T2CONbits.TON = 1;          
    }
    else if (timer == 3){
        TMR3 = 0;                   
        PR3 = pr_value;                   
        T3CONbits.TCKPS = prescaler_bits;   
        IFS0bits.T3IF = 0;
        if (has_interrupt) {IEC0bits.T3IE = 1;} 
        T3CONbits.TON = 1;          
    }
    else if (timer == 4){
        TMR4 = 0;                   
        PR4 = pr_value;                   
        T4CONbits.TCKPS = prescaler_bits;   
        IFS1bits.T4IF = 0;      
        if (has_interrupt) {IEC1bits.T4IE = 1;}  
        T4CONbits.TON = 1;          
    }
}

void tmr_setup_period(int timer, int ms, int has_interrupt){

    // List of prescaler values: {ratio, bits}
    int prescaler[][2] = {
        {1, 0}, {8, 1}, {64, 2}, {256, 3}
    };
    
    // CALCULATE THE PRESCALER & PR VALUES
    int prescaler_bits;
    long int pr_value;
    
    for (int i = 0; i < 4; i++) {
        pr_value = (FCY_PROP * ms) / (prescaler[i][0]);
        prescaler_bits = prescaler[i][1];
        if (pr_value < MAX_VALUE_16_BITS) {break;}
    }
    
    tmr_set_register_values(timer, pr_value, prescaler_bits, has_interrupt);
    
}


int tmr_wait_period(int timer){
    
    if (timer == 1){
        if (IFS0bits.T1IF){
            IFS0bits.T1IF = 0; 
            return 1;
        }
        else{
            while(!IFS0bits.T1IF); 
            IFS0bits.T1IF = 0; 
            return 0;
        }
    }

    if (timer == 2){
        if (IFS0bits.T2IF) {
            IFS0bits.T2IF = 0; 
            return 1;
        }
        else{
            while(!IFS0bits.T2IF);   
            IFS0bits.T2IF = 0;          
            return 0;
        }
    }

    if (timer == 3){
        if (IFS0bits.T3IF){
            IFS0bits.T3IF = 0;
            return 1;
        }
        else{
            while(!IFS0bits.T3IF) {};  
            IFS0bits.T3IF = 0;         
            return 0;
        }
    }

    if (timer == 4){
        if (IFS1bits.T4IF){
            IFS1bits.T4IF = 0;
            return 1;
        }
        else{
            while(!IFS1bits.T4IF) {};     
            IFS1bits.T4IF = 0;         
            return 0;
        }
    }
    
    return -1;
}

double calculate_average(int values[], int size){

    double sum = 0.0;

    for(int i=0; i<size; i++){
        sum += values[i];
    }

    return sum/size;

}