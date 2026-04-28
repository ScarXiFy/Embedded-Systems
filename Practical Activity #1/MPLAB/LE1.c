/*
 * File:   LE1.c
 * Author: jelauron
 *
 * Created on February 19, 2026, 11:22 AM
 */


#include <xc.h>

#define _XTAL_FREQ 4000000

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void main() 
 { 
    
      ADCON1 = 0x06;
      int cnt, i;
      TRISA = 0x01;
      TRISB = 0x00;
      PORTB = 0x00;
    for(;;) 
    {
      if(PORTA & 0x01)
      {
	for (int i = 1; i <= 3; i++)
	{ 
      for(cnt=0;cnt<10000;cnt++); // delay 
      PORTB = 0x01; // set RB0 to 1 (LED ON)
    
      for(cnt=0;cnt<10000;cnt++); // delay 
      PORTB = 0x00; // set RB0 to 0 (LED OFF)
      }
   }
   } 
 } 
