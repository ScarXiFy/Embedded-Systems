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

unsigned int overflow_count;
int count = 1;
unsigned int startingcount = 1;
unsigned int time = 30;
unsigned char pause = 1;

void instCtrl (unsigned char INST)
{
	PORTC = INST;
	RA0 = 0;		// for instCtrl set RS to 0
	RA1 = 1;
	__delay_ms(1000);
	RA1 = 0;
}

void dataCtrl (unsigned char DATA)
{
	PORTC = DATA;
	RA0 = 1;		// for dataCtrl set RS to 1
	RA1 = 1;
	__delay_ms(1000);
	RA1 = 0;
}

void initLCD()
{
	__delay_ms(1000);	// LCD Startup
	instCtrl(0x38);		// function set: 8-bit; dual-line
	instCtrl(0x08);		// display off
	instCtrl(0x01);		// display clear
	instCtrl(0x06);		// entry mode: increment; shift off 
	instCtrl(0x0C);		// display on; cursor off; blink off
}

void delay (unsigned int overflow)
{
	overflow_count = 0;
	while (overflow_count < overflow)
	{
	}
}

void interrupt ISR()
{
	GIE = 0;
	if (INTF)
	{
		INTF = 0;
		unsigned int val = PORTD & 0x0F;
		  switch (val)
		  {
		     case 0x00:
			time = 6;
			break;
		     case 0x01:
			time = 30;
			break;
		     case 0x02:
			time = 61;
			break;
		     case 0x04:
			time = 122;
			break;
		     case 0x05:
			if (pause)
			      pause = 0;
			else
			      pause = 1;
			break;
		  }
	}
	if (T0IF)
	{
		T0IF = 0;
		overflow_count++;
	}
	GIE = 1;
}

void main ()
{
	ADCON1 = 0x06;
	TRISA = 0x00;
	TRISB = 0x01;
	TRISC = 0x00;
	TRISD = 0xFF;
	initLCD();

	OPTION_REG = 0x44;	// PS2:PS0 - prescaler 1:32
						// PSA - prescaler assignment to TMR0
						// T0CS - internal instruction cycle clock
						// T0SE - not used since clock is internal
	INTE = 1;
	T0IE = 1;
	T0IF = 0;
	GIE = 1;

	instCtrl(0xC8);
	dataCtrl('T');
	dataCtrl('I');
	dataCtrl('M');
	dataCtrl('E');
	dataCtrl(':');
	while(1)
	{
		while(!pause)
		{
		}
		  
		delay(time);
		switch (count)
		{
		case 1:
			instCtrl(0xCD);
			dataCtrl('1');
			break;
		case 2:
			instCtrl(0xCD);
			dataCtrl('2');
			break;
		case 3:
			instCtrl(0xCD);
			dataCtrl('3');
			break;
		case 4:
			instCtrl(0xCD);
			dataCtrl('4');
			break;
		case 5:
			instCtrl(0xCD);
			dataCtrl('5');
			break;
		case 6:
			instCtrl(0xCD);
			dataCtrl('6');
			break;
		case 7:
			instCtrl(0xCD);
			dataCtrl('7');
			break;
		case 8:
			instCtrl(0xCD);
			dataCtrl('8');
			break;
		case 9:
			instCtrl(0xCD);
			dataCtrl('9');
			break;
		}
		count++;
		if (count > 9)
		{
			count = startingcount;
		}
	}
}