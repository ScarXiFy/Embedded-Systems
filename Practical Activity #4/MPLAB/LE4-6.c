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

unsigned long period = 0;
unsigned long overflow, curr, last;

void interrupt ISR(void){
   GIE = 0; // disable all unmasked interrupts (INTCON reg)
   if (CCP1IF == 1)
   {
       CCP1IF = 0;
       curr = (overflow * 65536) + CCPR1;
       period = curr - last;
       last = curr;
       period = period * 8; // ms
       RA0 = RA0^1;
   }
   if (TMR1IF == 1)
   {
       TMR1IF = 0;
       overflow++;
   }
   GIE = 1; // enable all unmasked interrupts (INTCON reg)
}

void instCtrl(unsigned int out){
	PORTB = out;
	PORTD = 0x02;
	__delay_ms(500);
	PORTD = 0;
	return;
}

void dataCtrl(unsigned int out){
	PORTB = out;
	PORTD = 0x03;
	__delay_ms(500);
	PORTD = 0x01;
	return;
}

void initLCD(){
	__delay_ms(1000);
	instCtrl(0b00111000);	// function set: 8-bit; dual-line
	instCtrl(0b00001000);	// display off
	instCtrl(0b00000001);   // display clear 
  	instCtrl(0b00000110);   // entry mode: increment; shift off 
  	instCtrl(0b00001100);   // display on; cursor off; blink off
	return; 
}

void main(void){
   ADCON1 = 0x06; // set all pins in PORTA as digital I/O
   TRISA = 0x00; // sets all of PORTA to output
   TRISC = 0x04; // set RC2 to input
   TRISB = 0x00;
   TRISD = 0x00;
   initLCD();
   T1CON = 0x30; // 1:8 prescaler, Timer1 off
   CCP1CON = 0x05; // capture mode: every rising edge
   CCP1IE = 1; // enable TMR1/CCP1 match interrupt (PIE1 reg)
   CCP1IF = 0; // reset interrupt flag (PIR1 reg)
   PEIE = 1; // enable all peripheral interrupt (INTCON reg)
   GIE = 1; // enable all unmasked interrupts (INTCON reg)
   TMR1ON = 1; // Turns on Timer1 (T1CON reg)
   TMR1IE = 1;
   int startpos;
   instCtrl(0xC2);
   dataCtrl('P');
   dataCtrl('e');
   dataCtrl('r');
   dataCtrl('i');
   dataCtrl('o');
   dataCtrl('d');
   dataCtrl(':');
   dataCtrl(' ');
   
   for(;;)
   {
      unsigned long sec = period / 1000000;      // whole seconds
      unsigned long frac = period % 1000000;     // fractional part
      frac = frac / 1000; 
      startpos = 0xCC;
      
      // display whole number digit of seconds
      instCtrl(startpos++);
      dataCtrl(sec + '0');
      instCtrl(startpos++);
      dataCtrl('.');

      // display zeros for fraction
      if (frac < 100){				//if < 0.100s
	 instCtrl(startpos++);
	 dataCtrl('0');
      }
      if (frac < 10){				//if < 0.010s
	 instCtrl(startpos++); 
	 dataCtrl('0');
      }
	 
      // display fraction
      char buffer[4];
      int i = 0;
      
      if (frac == 0) {
	  buffer[i++] = '0';
      } else {
	  while (frac > 0) {
	      buffer[i++] = (frac % 10) + '0';
	      frac /= 10;
	  }
      }
      
      for (int j = i - 1; j >= 0; j--) {
	 instCtrl(startpos++); 
	 dataCtrl(buffer[j]);
      }

      // display unit
      dataCtrl(' ');
      dataCtrl('s');
   }
}