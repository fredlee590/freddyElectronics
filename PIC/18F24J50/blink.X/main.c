/* 
 * File:   main.c
 * Author: Freddy
 *
 * Created on November 24, 2015, 12:09 AM
 */
#include <p18f24j50.h>
#include <delays.h>

#pragma config WDTEN = OFF
#pragma config OSC = INTOSC
#pragma config DEBUG = ON
#pragma config XINST = OFF

/*
 * 
 */
#define LED LATAbits.LATA0

void delay(unsigned int duration)
{
    unsigned int i;
    
    for(i = 0; i < duration; i++)
    {
        // delay 100,000 cycles
        Delay10KTCYx(10);   // assuming 1 cycle period = 4 x oscillation period
                            // 100,000 cycles = 400,000 oscillations of clock
                            // 8MHz = 8,000,000 oscillations per second
                            // 400,000 / 8,000,000 = 0.05 seconds = 50 ms
        
                            // 1 us = 0.000001 s = 10^-6 s = x / 8,000,000
                            // x = 8 oscillations = 2 cycles
    }
}

delay_us(unsigned int duration)
{
    unsigned int i, j;
    for(i = 0; i < duration; i++)
    {
        for(j = 0; j < 2; j++)
            Delay1TCY();
    }
}

void main() {
    OSCCONbits.IRCF = 0b111; // max clock speed: 8MHz
    
    TRISAbits.TRISA0 = 0;
    LED = 0;
    
    while(1)
    {
        LED ^= 1;
        //delay_us(2000);
        delay_us(1);
    }
}

