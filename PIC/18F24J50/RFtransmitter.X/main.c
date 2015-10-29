/* 
 * File:   main.c
 * Author: Freddy
 *
 * Created on September 26, 2015, 1:06 AM
 * 
 * Transceiver project
 * 
 * Designed to send and receive wifi messages.
 */

#include <p18f24j50.h>
#include <usart.h>
#include <spi.h>

#pragma config WDTEN = OFF
#pragma config OSC = INTOSC
#pragma config DEBUG = ON
#pragma config XINST = OFF

#define LENGTH      3
#define READ_CMD    0b00000000
#define WRITE_CMD   0b00100000



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

unsigned char readStatusFromReg(unsigned char addr)
{
    unsigned char cmd, result;
    // Make sure address only takes least significant 5 bits
    if(addr & 11100000)
        return 0;
    
    // prepare to send
    cmd = addr | READ_CMD;
    LATAbits.LATA2 = 0;
    
    // address goes first
    WriteSPI1(addr);
    while(!DataRdySPI1());
    result = ReadSPI1();

    LATAbits.LATA2 = 1;
    
    return result;
}

unsigned char* talkToTransceiver(unsigned char cmdType, unsigned char addr,
        unsigned char* data, unsigned char len)
{
    unsigned char i, cmd;
    unsigned char* result;

    // Make sure address only takes least significant 5 bits
    if(addr & 11100000)
        return 0;
    
    // prepare to send
    cmd = addr | cmdType;
    LATAbits.LATA2 = 0;
    
    // address goes first
    WriteSPI1(addr);
    while(!DataRdySPI1());
    ReadSPI1();

    for(i = 0; i < len; i++)
    {
        //sendCh(data[i] + '0');
        WriteSPI1(data[i]);
        while(!DataRdySPI1());
        result[i] = ReadSPI1();
    }
    LATAbits.LATA2 = 1;
    
    return result;
}

void powerOnRF24L01()
{
    //unsigned char powerOnValue = 0b00001010;
    unsigned char val = 0b00001010;
    unsigned char* res;

    talkToTransceiver(WRITE_CMD, 0x00, &val, 1);
    res = talkToTransceiver(READ_CMD, 0x00, 0x0, 1);
    /*
    if(!res)
    {
        sendStr("NULL res");
    } else {
        sendCh(*res);
    }
    */
        
}

// program goes here
void main(void)
{
    OSCCONbits.IRCF = 0b111; // max clock speed: 8MHz
    
    // define GPIO for IRQ, CSN, CE for RF module
    TRISAbits.TRISA0 = 0; // output to LED

    TRISAbits.TRISA1 = 1; // input from IRQ on RF transceiver
    TRISAbits.TRISA2 = 0; // output to CSN on RF transceiver
    TRISAbits.TRISA3 = 0; // output to CE on RF transceiver
    TRISBbits.TRISB4 = 0; // output from SCLK to RF transceiver
    TRISBbits.TRISB5 = 1; // input from MISO on RF transceiver
    TRISCbits.TRISC7 = 0; // output from MOSI to RF transceiver

    /*
    Open1USART(USART_TX_INT_OFF
            & USART_RX_INT_OFF
            & USART_ADDEN_OFF
            & USART_BRGH_LOW
            & USART_SINGLE_RX
            & USART_SYNC_SLAVE
            & USART_EIGHT_BIT
            & USART_ASYNCH_MODE, 12);
     */
    OpenSPI1(SPI_FOSC_64, MODE_00, SMPMID);

    LATAbits.LATA0 = 0; // LED
    LATAbits.LATA2 = 0; // CSN
    LATAbits.LATA3 = 1; // CE

    //sendStr("Program goes here");
    
    if(readStatusFromReg(0x00) & 0x80)
        LATAbits.LATA0 = 1;
    
    /*
    while(1)
    {
        //sendStr("Start SPI command");
        powerOnRF24L01();
        //sendStr("SPI command end");
    }
    */
    //Close1USART();
    CloseSPI1();

    while(1);
}

