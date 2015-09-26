/* 
 * File:   main.c
 * Author: Freddy
 *
 * Created on July 23, 2015, 1:17 AM
 */

#include <p18f24j50.h>
#include <usart.h>

#pragma config WDTEN = OFF
#pragma config OSC = INTOSC
#pragma config DEBUG = ON
#pragma config XINST = OFF

// send a single character
void sendCh(char char_to_send)
{
    putc1USART(char_to_send);
    while(Busy1USART());
}

// send a whole string
void sendStr(rom char* str_to_send)
{
    while(*str_to_send)
    {
        sendCh(*str_to_send);
        str_to_send++;
    }
    
    sendCh('\n');
    sendCh('\r');
}

// program goes here
void main(void)
{
    OSCCONbits.IRCF = 0b111; // max clock speed: 8MHz
    
    Open1USART(USART_TX_INT_OFF
            & USART_RX_INT_OFF
            & USART_ADDEN_OFF
            & USART_BRGH_LOW
            & USART_SINGLE_RX
            & USART_SYNC_SLAVE
            & USART_EIGHT_BIT
            & USART_ASYNCH_MODE, 12);

    sendStr("Program goes here");

    Close1USART();

    while(1);
}

