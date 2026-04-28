#include<xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF
#define _XTAL_FREQ 400000
unsigned int val;
unsigned int overflow_count;
unsigned int cnt;

void delay(unsigned int overflow)
{
   overflow_count=0;
   while(overflow_count < overflow){
   }
}

void interrupt ISRT()
{
	GIE = 0;		// disables all unmasked interrupts to prevent overlap
	if (INTF)		// check the interrupt flag
	{
		INTF = 0;	// clears the interrupt flag	
		val = PORTD & 0x0F;
		switch(val){		//identify key pressed
			case 0x00:
				cnt=1;
				break;
			case 0x01:
				cnt=2;
				break;
			case 0x02:	
				cnt=3;
				break;
			case 0x04:
				cnt=4;
			    break;
			case 0x05:
			    cnt=5;
				break;
			case 0x06:
				cnt=6;
			    break;
			case 0x08:
				cnt=7;
			    break;
			case 0x09:
				cnt=8;
			    break;
			case 0x0A:
			    cnt=9;
				break;
			case 0x0D:
			    cnt=0;
				break;
		}
		
	}
	if (T0IF)
	{
		T0IF = 0;
		overflow_count++;
	}
	GIE = 1;		// enable interrupt again
}

int main ()
{
	TRISB = 0x01;  	// set RB0 as input
  	TRISC = 0x00;	// set all of PORTC as output
 	TRISD = 0x0F;  	// set RD0:RD3 as input 
	 
  	OPTION_REG = 0x44; // PS2:PS0 - prescaler 1:32
		     // PSA - prescaler assignment to TMR0
		     // T0CS - internal instruction cycle clock
		     // T0SE - not used since clock is internal
	INTE = 1;
   	T0IE = 1; // enable Timer0 overflow interrupt
   	T0IF = 0; // clears the interrupt flag
   	GIE = 1; // enables all unmasked interrupt

	PORTC = 0x00;
	cnt = 0;
	while (1)
	{
		if(cnt>9)
			cnt=0;

		delay(98);
		switch(cnt){
		case 0:
			PORTC = 0x00;
			break;
		case 1:
			PORTC = 0x01;
			break;
		case 2:
			PORTC = 0x02;
			break;
		case 3:
			PORTC = 0x03;
			break;
		case 4:
			PORTC = 0x04;
			break;
		case 5:
			PORTC = 0x05;
			break;
		case 6:
			PORTC = 0x06;
			break;
		case 7:
			PORTC = 0x07;
			break;
		case 8:
			PORTC = 0x08;
			break;
		case 9:
			PORTC = 0x09;
			break;
		}
		cnt++;
	}
}