#include <xc.h> // include file for the XC8 compiler

#define _XTAL_FREQ 4000000
 
#pragma config FOSC = XT 
#pragma config WDTE = OFF 
#pragma config PWRTE = ON 
#pragma config BOREN = ON 
#pragma config LVP = OFF 
#pragma config CPD = OFF 
#pragma config WRT = OFF 
#pragma config CP = OFF

void delay (int cnt) {
	int i, j;
	for (i = cnt; i != 0; i--)
	       for (j = 0; j < 5000; j++);
}

void instCtrl (unsigned char INST) {
	PORTB = INST;	 // load instruction to PORTB
	RC0 = 0;	// set RS to 0 (instruction reg)
	RC2 = 0;	// set RW to 0 (write)
	RC1 = 1;	// set E to 1
	delay(1);	// call delay
	RC1 = 0;	// set E to 0 (strobe)
}

void dataCtrl( unsigned char DATA) {
	PORTB = DATA; // load data to PORTB
	RC0 = 1;	// set RS to 1 (data reg)
	RC2 = 0;	// set RW to 0 (write)
	RC1 = 1;	// set E to 1
	delay(1);	// call delay
	RC1 = 0;	// set E to 0 (strobe)
}
		
void initLCD() {
	delay(1);		// LCD startup
	instCtrl(0x38); // function set: 8-bit; dual-line
	instCtrl(0x08); // Display off
	instCtrl(0x01); // Display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0E); // display on; cursor on; blink off
}	

void main() {
	unsigned char data;
	TRISB = 0x00;	// sets PORTB as output
	TRISC = 0x00;	// sets PORTC as output
	TRISD = 0xFF;	// sets PORTD as input
	initLCD();	// go to initialize lcd function
	int i = 0;
	while(1) {
		if (RD4==1) {
			data = PORTD & 0x0F; // mask input

			if(i==20) {	
				instCtrl(0xC0); 
			}
			else if(i==40) {	
				instCtrl(0x94); 
			}
			else if(i==60) {	
				instCtrl(0xD4); 
			}
			else if(i>=80) {	
				initLCD(); 
				i=0;
			}
			
			if (data == 0x00) dataCtrl('1');
			else if (data == 0x01) dataCtrl('2');
			else if (data == 0x02) dataCtrl('3');
			else if (data == 0x04) dataCtrl('4');
			else if (data == 0x05) dataCtrl('5');
			else if (data == 0x06) dataCtrl('6');
			else if (data == 0x08) dataCtrl('7');
			else if (data == 0x09) dataCtrl('8');
			else if (data == 0x0A) dataCtrl('9');
			else if (data == 0x0C) dataCtrl('*');
			else if (data == 0x0D) dataCtrl('0');
			else if (data == 0x0E) dataCtrl('#');
			i++;
		}
	}

}