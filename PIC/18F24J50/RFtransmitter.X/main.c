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
#define FLUSHTX_CMD     0b11100001
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

    CSN = 0;
    WriteSPI1(W_PLD_NA_CMD);
    
    for(i = 0; i < len; i++)
    {
        while(!DataRdySPI1())
        WriteSPI1(data[i]);
    }
    CSN = 1;
    return 0;
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

void flushTXFIFO()
{
    CSN = 0;
    
    WriteSPI1(FLUSHTX_CMD);
    
    CSN = 1;
}

void configure()
{
    // register 0x00
    writeReg(0x00, 0x38);
    // register 0x01
    writeReg(0x01, 0x00); // Auto ACK register - clear all for no auto ACK
    // register 0x04
    writeReg(0x04, 0x00); // Retransmission register - default delay of 250 us
    // register 0x03
    writeReg(0x03, 0x03); // Configure address length field - default of 5 bits
    // register 0x06
    writeReg(0x06, 0x07); // RF register - RF power 0 dBm (highest) @ 1Mbps
    // register 0x05 - default
    // register 0x10 - default
}

void transmit(unsigned char * data, unsigned char len)
{
    unsigned char i;
    // register 0x07
    writeReg(0x07, 0x27); // RF register - RF power 0 dBm (highest) @ 1Mbps
    // register 0x00 - power up
    writeReg(0x00, 0x3A);
    
    // clear tx fifo
    flushTXFIFO();        // apparently we need this
    
    // write in payload
    writePayload(data, len);

    // send CE pulse to transmit TXFIFO
    sendPayload();    
    
#ifdef __DEBUG
    while(readStatus() != 0x20)
        LED = 0;
    LED = 1;
#endif
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

    configure();
    // queue up data to send
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;

    transmit(data, 3);

    CloseSPI1();

    while(1);
}

