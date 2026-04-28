#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

void setPWM(unsigned char pr2, unsigned char duty_percent)
{
    PR2 = pr2;

    unsigned int duty_val;
    duty_val = ((unsigned long)duty_percent * 4 * (PR2 + 1)) / 100;

    CCPR1L = duty_val >> 2;     // upper 8 bits
    CCP1CON = (CCP1CON & 0xCF) | ((duty_val & 0x03) << 4);      // lower 2 bits
}

void main()
{
    TRISB = 0xFF;       // set PORTB as input  
    TRISC = 0x00;       // set PORTC as output

    // PWM setup
    CCP1CON = 0x0C;     // PWM mode
    T2CON = 0x06;       // Timer2 ON, prescaler 1:16

    unsigned char freq = 0;
    unsigned char duty = 0;
    unsigned char pr2_vals[3]  = {249, 124, 61};
    unsigned char duty_vals[5] = {10, 25, 50, 75, 95};

    // Initial PWM
    setPWM(pr2_vals[freq], duty_vals[duty]);

    __delay_ms(200);

    while (1)
    {
        if (RB0 == 1)
        {
            __delay_ms(200);

            freq++;
            if (freq >= 3) freq = 0;

            setPWM(pr2_vals[freq], duty_vals[duty]);

            while(RB0 == 1);
        }

        if (RB1 == 1)
        {
            __delay_ms(200);

            duty++;
            if (duty >= 5) duty = 0;

            setPWM(pr2_vals[freq], duty_vals[duty]);

            while(RB1 == 1);
        }
    }
}