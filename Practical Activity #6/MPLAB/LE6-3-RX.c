#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

// Function to convert received character to BCD value (0-9)
// For '*' and '#', output 0x0F which will blank the 74LS48 display
unsigned char getBCDValue(unsigned char value) {
    unsigned char output;
    
    switch(value) {
        case '1': output = 0x01; break;  // BCD 0001 -> displays 1
        case '2': output = 0x02; break;  // BCD 0010 -> displays 2
        case '3': output = 0x03; break;  // BCD 0011 -> displays 3
        case '4': output = 0x04; break;  // BCD 0100 -> displays 4
        case '5': output = 0x05; break;  // BCD 0101 -> displays 5
        case '6': output = 0x06; break;  // BCD 0110 -> displays 6
        case '7': output = 0x07; break;  // BCD 0111 -> displays 7
        case '8': output = 0x08; break;  // BCD 1000 -> displays 8
        case '9': output = 0x09; break;  // BCD 1001 -> displays 9
        case '0': output = 0x00; break;  // BCD 0000 -> displays 0
        case '*': output = 0x0F; break;
        case '#': output = 0x0F; break;
        default:  output = 0x0F; break;  // BCD 1111 -> blank display (RBI function)
    }
    
    return output;
}

void main(void) 
{ 
    // USART Configuration for RECEIVE
    SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed 
    SYNC = 0;     // asynchronous mode (TXSTA reg) 
    SPEN = 1;     // enable serial port (RCSTA reg) 
    RX9 = 0;      // 8-bit reception (RCSTA reg) 
    BRGH = 1;     // asynchronous high-speed (TXSTA reg) 
    CREN = 1;     // enable continuous receive (RCSTA reg) 
    
    // PORTB configuration
    // RB0-RB3 = output to 74LS48 (A,B,C,D inputs)
    // RB4-RB7 = unused (can be output or input)
    TRISB = 0x00; // all pins in PORTB as output
    PORTB = 0x00; // initial output 0
    
    for(;;) // foreground routine 
    { 
        while(!RCIF);                       // wait until receive buffer is full 
        unsigned char received = RCREG;     // read the received character
        
        // Get BCD value and output to RB0-RB3 (74LS48 inputs)
        unsigned char bcdValue = getBCDValue(received);
        PORTB = bcdValue & 0x0F;            // mask to only lower 4 bits (RB0-RB3)
    } 
}