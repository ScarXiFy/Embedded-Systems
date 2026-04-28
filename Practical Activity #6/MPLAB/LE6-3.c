#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 400000

void display(unsigned int val){
   TXREG = 'Y';
   __delay_ms(10);
   TXREG = 'o';
   __delay_ms(10);
   TXREG = 'u';
   __delay_ms(10);
   TXREG = ' ';
   __delay_ms(10);
   TXREG = 'p';
   __delay_ms(10);
   TXREG = 'r';
   __delay_ms(10);
   TXREG = 'e';
   __delay_ms(10);
   TXREG = 's';
   __delay_ms(10);
   TXREG = 's';
   __delay_ms(10);
   TXREG = 'e';
   __delay_ms(10);
   TXREG = 'd';
   __delay_ms(10);
   TXREG = ' ';
   __delay_ms(10);
   switch(val)            //identify key pressed
   {        
    case 0x00:
        TXREG = '1';
        __delay_ms(10);
        break;
    case 0x01:
       TXREG = '2';
        __delay_ms(10);
        break;
    case 0x02:
        TXREG = '3';
        __delay_ms(10);
        break;
    case 0x04:
        TXREG = '4';
        __delay_ms(10);
        break;
    case 0x05:
        TXREG = '5';
        __delay_ms(10);
        break;
    case 0x06:
        TXREG = '6';
        __delay_ms(10);
        break;
    case 0x08:
        TXREG = '7';
        __delay_ms(10);
        break;
    case 0x09:
        TXREG = '8';
        __delay_ms(10);
        break;
    case 0x0A:
        TXREG = '9';
        __delay_ms(10);
        break;
    case 0x0C:
        TXREG = '*';
        __delay_ms(10);
        break;
    case 0x0D:
        TXREG = '0';
        __delay_ms(10);
        break;
    case 0x0E:
        TXREG = '#';
        __delay_ms(10);
        break;
   }
   TXREG = '.';
   __delay_ms(10);
   TXREG = '\r';
   __delay_ms(10);
   TXREG = '\n';
   __delay_ms(10);
}

void main(void)
{
   TRISB = 0xFF;      // set all of PORTD as input
   SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
   SYNC = 0; // asynchronous mode (TXSTA reg)
   SPEN = 1; // enable serial port (RCSTA reg)
   TX9 = 0; // 8-bit transmission (TXSTA reg)
   BRGH = 1; // asynchronous high-speed (TXSTA reg)
   TXEN = 1; // transmit enable (TXSTA reg)
   unsigned int val;
      for(;;) // foreground routine
      {
     while(!TRMT);         // wait until transmit shift register is empty
     if(RB4 == 1)        //if key is pressed
     {
        val = PORTB;        //store input in variable
        val = val & 0x0F;        //extract keypad input only and ignore other values
        display(val);
        while(RB4==1);    
      }
   }
}