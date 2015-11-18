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

#pragma config WDTEN = OFF
#pragma config OSC = INTOSC
#pragma config DEBUG = ON
#pragma config XINST = OFF

#define LENGTH      3
#define READ_CMD    0b00000000
#define WRITE_CMD   0b00100000
#define NOP_CMD     0b11111111

unsigned char readStatus()
{
    LATAbits.LATA2 = 0;
    
    WriteSPI1(NOP_CMD);

    LATAbits.LATA2 = 1;
    
    return SSP1BUF;
}

char writeReg(unsigned char addr, unsigned char* data,
        unsigned char len)
{
    unsigned char cmd, i;
    // Make sure address only takes least significant 5 bits
    if(addr & 11100000)
        return -1;
    
    if(len < 1)
        return -1;
    
    // prepare to send
    cmd = addr | WRITE_CMD;
    LATAbits.LATA2 = 0;
    
    // address goes first
    WriteSPI1(cmd);

    for(i = 0; i < len; i++)
    {
        while(!DataRdySPI1());
        WriteSPI1(data[i]);
    }
    
    LATAbits.LATA2 = 1;
    
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
    LATAbits.LATA2 = 0;
    
    // address goes first
    WriteSPI1(cmd);

    for(i = 0; i < len; i++)
    {
        while(!DataRdySPI1());
        data[i] = ReadSPI1();
    }

    LATAbits.LATA2 = 1;
    
    return 0;
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

    LATAbits.LATA0 = 0; // LED
    LATAbits.LATA2 = 1; // CSN
    LATAbits.LATA3 = 1; // CE

    // power up to stand by
    data[0] = 0x0A;
    writeReg(0x00, data, 1);

#ifdef __DEBUG
    // check to confirm what we just wrote in
    readReg(0x00, data, 1);
    
    if(*data == 0x0A)
        LATAbits.LATA0 = 1;
    
    if(readStatus() != 0x0E)
        LATAbits.LATA0 = 0;
#endif
    
    // any configuration - let's use all defaults for now.
    
    // enter TX mode
    
    // send stuff as a test

    CloseSPI1();

    while(1);
}

