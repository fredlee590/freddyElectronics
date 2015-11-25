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
#include <spi.h>
#include <delays.h>

#pragma config WDTEN = OFF
#pragma config OSC = INTOSC
#pragma config DEBUG = ON
#pragma config XINST = OFF

// commonly used commands
#define READ_CMD        0b00000000
#define WRITE_CMD       0b00100000
#define W_PLD_NA_CMD    0b10110000
#define NOP_CMD         0b11111111

// PINS
#define LED LATAbits.LATA0
#define CSN LATAbits.LATA2
#define CE  LATAbits.LATA3

unsigned char readStatus()
{
    CSN = 0;
    WriteSPI1(NOP_CMD);
    CSN = 1;
    
    return SSP1BUF;
}

char writeReg(unsigned char addr, unsigned char data)
{
    unsigned char cmd, i;
    // Make sure address only takes least significant 5 bits
    if(addr & 11100000)
        return -1;
    
    // prepare to send
    cmd = addr | WRITE_CMD;
    CSN = 0;
    
    // address goes first
    WriteSPI1(cmd);
    while(!DataRdySPI1());
    WriteSPI1(data);
    while(!DataRdySPI1());
    
    CSN = 1;
    
    return 0;
}

char readReg(unsigned char addr, unsigned char* data,
        unsigned char len)
{
    unsigned char cmd, i;
    // Make sure address only takes least significant 5 bits
    if(addr & 11100000)
        return -1;
    
    if(len < 1)
        return -1;
    
    // prepare to send
    cmd = addr | READ_CMD;
    CSN = 0;
    
    // address goes first
    WriteSPI1(cmd);

    for(i = 0; i < len; i++)
    {
        while(!DataRdySPI1());
        data[i] = ReadSPI1();
    }

    CSN = 1;
    
    return 0;
}

char writePayload(unsigned char* data, unsigned char len)
{
    unsigned char i;
    if(len > 32)
        return -1;

    WriteSPI1(W_PLD_NA_CMD);
    
    for(i = 0; i < len; i++)
    {
        WriteSPI1(data[i]);
    }
}

void sendPayload()
{
    unsigned int i, j;
    CE = 1;
    // wait at least 10 us
    for(i = 0; i < 20; i++)
    {
        for(j = 0; j < 2; j++)
        {
            Delay1TCY();
        }
    }
    CE = 0;
}
// program goes here
void main(void)
{
    unsigned char* data;
    OSCCONbits.IRCF = 0b111; // max clock speed: 8MHz
    
    // define GPIO for IRQ, CSN, CE for RF module
    TRISAbits.TRISA0 = 0; // output to LED

    TRISAbits.TRISA1 = 1; // input from IRQ on RF transceiver
    TRISAbits.TRISA2 = 0; // output to CSN on RF transceiver
    TRISAbits.TRISA3 = 0; // output to CE on RF transceiver
    TRISBbits.TRISB4 = 0; // output from SCLK to RF transceiver
    TRISBbits.TRISB5 = 1; // input from MISO on RF transceiver
    TRISCbits.TRISC7 = 0; // output from MOSI to RF transceiver

    OpenSPI1(SPI_FOSC_64, MODE_00, SMPMID);

    LED = 0; // LED
    CSN = 1; // CSN
    CE = 0; // CE

#ifdef __DEBUG
    if(readStatus() == 0x0E)
        LED = 1;
#endif

    // power up to stand by
    writeReg(0x00, 0x7A);

#ifdef __DEBUG
    // check to confirm what we just wrote in
    readReg(0x00, data, 1);
    
    if(*data == 0x7A)
        LED = 0;
#endif
    
    // any configuration
    writeReg(0x01, 0x00);
    writeReg(0x03, 0x3);
    writeReg(0x04, 0x00);
    writeReg(0x06, 0x07);
    
    // queue up data to send
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    writePayload(data, 3);
    
    // send CE pulse to transmit TXFIFO
    sendPayload();
    
#ifdef __DEBUG
    //while(!(readStatus() & 0b00100000))
    //    LED = 0;
    //LED = 1;
#endif
    CloseSPI1();

    while(1);
}

