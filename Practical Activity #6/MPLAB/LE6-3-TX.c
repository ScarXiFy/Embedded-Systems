#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

char checkInput(void) {
    char data, output;
    data = PORTB & 0x0F; // mask input (RB0-RB3 from 74C922)
    
    if (data == 0x00) output = '1';
    else if (data == 0x01) output = '2';
    else if (data == 0x02) output = '3';
    else if (data == 0x04) output = '4';
    else if (data == 0x05) output = '5';
    else if (data == 0x06) output = '6';
    else if (data == 0x08) output = '7';
    else if (data == 0x09) output = '8';
    else if (data == 0x0A) output = '9';
    else if (data == 0x0C) output = '*';
    else if (data == 0x0D) output = '0';
    else if (data == 0x0E) output = '#';
    else output = ' '; // default for invalid/unexpected input
    
    return output;
}

void main(void) 
{ 
    // USART Configuration for TRANSMIT
    SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed 
    SYNC = 0;     // asynchronous mode (TXSTA reg) 
    SPEN = 1;     // enable serial port (RCSTA reg) 
    TX9 = 0;      // 8-bit transmission (TXSTA reg) 
    BRGH = 1;     // asynchronous high-speed (TXSTA reg) 
    TXEN = 1;     // transmit enable (TXSTA reg) 

    TRISB = 0xFF; // sets PORTB as input (RB0-RB3 for encoder, RB4 for DA)
    
    for(;;) // foreground routine 
    { 
        if(RB4 == 1) {           // check Data Available pin from 74C922
            while(!TRMT);        // wait until transmit shift register is empty 
            TXREG = checkInput(); // send character via TX pin (RC6)
            
            // debounce: wait for key to be released
            while(RB4 == 1);
            
            // optional: small delay after release
            for(int x = 0; x < 50; x++);
        }
    } 
}