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

float voltage;
int whole, frac;
int d_value = 0;

void display(){
   PORTB = whole & 0x0F;
   PORTC = 0x02;
   __delay_ms(50);
   PORTB = frac & 0x0F;
   PORTC = 0x01;
   return;
}

void interrupt ISR()
{
  int period = 0;
  GIE = 0;
  if (ADIF)
  {
    ADIF = 0;
      /* read result register */
      d_value = (ADRESH << 8) | ADRESL;
      /* extracting the digits */
      voltage = (float)d_value * (float)50 / (float)1023.0;
      frac = ((int)voltage)%10;
      whole = ((int)voltage/10)%10;
      /* setting the LEDs */
      display(); 
  }
  GO = 1;
  GIE = 1;
}

void main(void)
{
   TRISC = 0x00;
   PORTC = 0x00; // enable cathode 7-segment LEDs
   TRISB = 0x00; // set all PORTB as output
   PORTB = 0x00; // all LEDs are off
   ADCON1 = 0x80; // result: right Justified, clock: FOSC/2
   ADCON0 = 0x41; // clock: FOSC/32, analog channel: AN0,
   ADIE=1;
   ADIF=0;
   PEIE=1;
   GIE=1;
   GO=1;
   for(;;) // foreground routine
   {
      
   }
}