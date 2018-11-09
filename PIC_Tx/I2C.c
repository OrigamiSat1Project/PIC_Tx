#include <xc.h>
#include "UART.h"
#include "I2C.h"
#include "Type_define.h"
#include "EEPROM.h"


#define _XTAL_FREQ 10000000
//UBYTE EEPROMData[16];
//UINT EEPROMDataLength;



int AckCheck;
int CollisionCheck;

/*******************************************************************************
*setting
******************************************************************************/
void InitI2CMaster(const UDWORD c){//Init Master Synchronous Serial Port(MSSP)
  SSPCON = 0b00101000;          //MSSP Control Register: Synchronous Serial Port Enabled;I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
  SSPCON2 = 0;                  //MSSP Control Register 2:
  SSPADD = (_XTAL_FREQ/(4*c))-1; //MSSP Address Register: the lower seven bits of SSPADD act as the baud rate generator reload value
  SSPSTAT = 0;                  //MSSP Status Register
  PIE1bits.SSPIE  = 1 ;               // enable MSSP interrupt
  PIE2bits.BCLIE  = 1 ;               // enable bus collision interrupt
  PIR1bits.SSPIF  = 0 ;               // clear MSSP interrupt flag
  PIR2bits.BCLIF  = 0 ;               // clear bus collision flag
}

void interruptI2C(void)
{
     if (PIR1bits.SSPIF == 1) {
          if (AckCheck == 1) {
              AckCheck = 0;
          }
          PIR1bits.SSPIF = 0;
     }
     if (PIR2bits.BCLIF == 1) {
          CollisionCheck = 1;
          PIR2bits.BCLIF = 0;
     }
}

void I2CMasterWait(char mask){
  while ((SSPSTAT & mask) || (SSPCON2 & 0x1F));
  //SSPSTAT : 0x05 -> transmit is not in progress & buffer empty
  //          0x04 -> transmit is not in progress
  //SSPCON2 : ack,receive,start,restart,stop is idle
}


int I2CMasterStart(UBYTE slave_address,UBYTE rw){
//  I2CMasterWait();
//  SEN = 1;                      //SEN Start Condition Enable; bit 0 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.SEN = 1 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     AckCheck = 1 ;
     SSPBUF = (char)((slave_address<<1)+rw);
     while (AckCheck);
     if (CollisionCheck == 1) return -1 ;
     return SSPCON2bits.ACKSTAT;
}

int I2CMasterRepeatedStart(UBYTE slave_address,UBYTE rw){
//  I2CMasterWait();
//  RSEN = 1;                     //Repeated Start Condition Enabled bit (Master mode only); bit 1 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.RSEN = 1 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     AckCheck = 1;
     SSPBUF = (char)((slave_address<<1)+rw);
     while (AckCheck);
     if (CollisionCheck == 1) return -1;
     return SSPCON2bits.ACKSTAT;
}

int I2CMasterStop(void){
//  I2CMasterWait();
//  PEN = 1;                      //Stop Condition Enable bit (Master mode only); bit 2 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.PEN = 1 ;
     if (CollisionCheck == 1) return -1 ;
     else                     return  0 ;
}

int I2CMasterWrite(UBYTE dataByte){                               
//  I2CMasterWait();
//  SSPBUF = dataByte;                   //Serial Receive/Transmit Buffer Register
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1;
     AckCheck = 1;
     SSPBUF = dataByte;
     while (AckCheck);
     if (CollisionCheck == 1) return -1;
     return SSPCON2bits.ACKSTAT;
}


int I2CMasterRead(UBYTE address){
     int data_from_slave ;

     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.RCEN = 1;      //  enable receive from slave
     I2CMasterWait(0x4) ;
     if (CollisionCheck == 1) return -1 ;
     data_from_slave = SSPBUF;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     SSPCON2bits.ACKDT = address;
     SSPCON2bits.ACKEN = 1;
     return data_from_slave;
}

/*******************************************************************************
*Method for EEPROM Write 
******************************************************************************/
/*
 *  Write To EEPROM
 *	arg      :   addressEEPROM, addressHigh, addressLow, *data
 *	return   :   EEPROM_address, high_address, low_address -> *data
 *	TODO     :   debug --->finish
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */

int ReadEEPROMonce(UBYTE address,UBYTE high_address,UBYTE low_address){
    int dat;
    int ans = -1;
    ans = I2CMasterStart(address,0);         //Start condition
    if(ans == 0){
        ans = I2CMasterWrite(high_address);    //Adress High Byte
        if(ans == -1) return -1;
        ans = I2CMasterWrite(low_address);    //Adress Low Byte
        if(ans == -1) return -1;
        ans = I2CMasterRepeatedStart(address,1);         //Restart condition
        if(ans == -1) return -1;
        dat = I2CMasterRead(1); //Read + Acknowledge
        if(dat == -1) return -1;
    }else return -1;
    ans = I2CMasterStop();
    if(ans == -1) return -1;
    __delay_ms(5);
    return dat;
}

UBYTE ReadEEPROM(UBYTE address,UBYTE high_address,UBYTE low_address){
    UBYTE dat;
    int ans = -1;
    while(ans == -1){
        ans = ReadEEPROMonce(address,high_address,low_address);
        __delay_ms(5);
    }
    dat = (UBYTE)ans;
    return dat;
}

int WriteOneByteToEEPROMonce(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    int ans = -1;
    ans = I2CMasterStart(addressEEPROM,0);               //Start condition
    if(ans == 0){
        ans = I2CMasterWrite(addressHigh);              //Adress High Byte
        if(ans == -1) return -1;
        ans = I2CMasterWrite(addressLow);           //Adress Low Byte
        if(ans == -1) return -1;
        ans = I2CMasterWrite(data);             //Data
        if(ans == -1) return -1;
    }else return -1;
    ans = I2CMasterStop();
    __delay_ms(5);
    return ans;
}

void WriteOneByteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    int ans = -1;
    while(ans == -1){
        ans = WriteOneByteToEEPROMonce(addressEEPROM,addressHigh,addressLow,data);
        __delay_ms(5);
    }
}