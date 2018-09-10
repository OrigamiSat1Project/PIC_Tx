#include <xc.h>
#include "Type_define.h"
#include "ITG3701.h"
#include "I2C.h"
#include "EEPROM.h"
#include "time.h"

const UBYTE ITG_ADDR            = 0x69;
const UBYTE ITG_WHO_AM_I        = 0x75;
const UBYTE ITG_WHO_VALUE       = 0x68;
const UBYTE ITG_SMPLRT_DIV      = 0x19;
const UBYTE ITG_CONFIG          = 0x1A;
const UBYTE ITG_GYRO_CONFIG     = 0x1B;
const UBYTE ITG_FIFO_EN         = 0x23;
const UBYTE ITG_INT_PIN_CFG     = 0x37;
const UBYTE ITG_INT_ENABLE      = 0x38;
const UBYTE ITG_INT_STATUS      = 0x3A;
const UBYTE ITG_DATA_START      = 0x41;
const UBYTE ITG_DATA_STOP       = 0x48;
const UBYTE ITG_PWR_MGMT_1      = 0x6B;
const UBYTE ITG_PWR_MGMT_2      = 0x6C;

int readITGAddr(UBYTE address)
{
    UBYTE Address = ITG_ADDR << 1;
    UBYTE ReadAddress = Address | 0x01;
    UBYTE ans;
    I2CMasterStart();                   //Start condition
    I2CMasterWrite(ITG_ADDR);           //7 bit address + Write
    I2CMasterWrite(address);            //7 bit address + Write
    I2CMasterRepeatedStart();           //Restart condition
    I2CMasterWrite(ReadAddress);        //7 bit address + Read
    ans = I2CMasterRead(0);
    I2CMasterStop();                    //Stop condition
    return ans;
}

int writeITGAddr(UBYTE address, UBYTE data)
{
    UBYTE Address = ITG_ADDR << 1;
    I2CMasterStart();               //Start condition
    I2CMasterWrite(address);        //7 bit address + Write
    I2CMasterWrite(data);      //Data
    I2CMasterStop();                //Stop condition
}

int initITG()
{
   int ans;
   __delay_us(3000);
   ans = readITGAddr(ITG_WHO_AM_I);
   
   if(ans == ITG_WHO_VALUE){
       writeITGAddr(ITG_PWR_MGMT_1,0x01);     //oscirator : PLL
       writeITGAddr(ITG_PWR_MGMT_2,0x00);     //gyro : not standby mode
       writeITGAddr(ITG_FIFO_EN,0x00);        //FIFO disabled
       writeITGAddr(ITG_CONFIG,0x00);         //FIFO disabled , DLPF 250Hz
       writeITGAddr(ITG_GYRO_CONFIG,0x18);    //FS:4000deg/sec
       //writeITGAddr(ITG_INT_PIN_CFG,0x00);    //FSYNC pin : activ low
       writeITGAddr(ITG_INT_ENABLE,0x01);     //DARA Ready Interupt : Enable
       __delay_us(3000);
   }else ans = -1;
   return ans;
}

int readITG(UBYTE *data, int offset)
{
    UBYTE Address = ITG_ADDR << 1;
    UBYTE ReadAddress = Address | 0x01;
    int ans , i , ack ;
    UBYTE dataNumber;
    dataNumber = ITG_DATA_STOP - ITG_DATA_STOP +1;

    
    while(ans != 0x01){
        ans = readITGAddr(ITG_INT_STATUS);
        ans = ans & 0x01;
    }
    
    I2CMasterStart();                   //Start condition
    I2CMasterWrite(Address);            //7 bit address + Write
    I2CMasterWrite(ITG_DATA_START);     //7 bit address + Write
    I2CMasterRepeatedStart();           //Restart condition
    I2CMasterWrite(ReadAddress);        //7 bit address + Read
    for (UBYTE i = 0; i < dataNumber-1; i++){
        data[offset+i] = I2CMasterRead(1);
    }
    data[offset + dataNumber-1] = I2CMasterRead(0);
    I2CMasterStop();                //Stop condition
}