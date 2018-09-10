#include <xc.h>
#include "Type_define.h"
#include "adxl345.h"
#include "I2C.h"
#include "EEPROM.h"
#include "Time.h"

const UBYTE ADXL345_ADDR            = 0x1D;
const UBYTE ADXL345_DEVID           = 0x00;
const UBYTE ADXL345_DEVID_VALUE     = 0xE5;
const UBYTE ADXL345_BW_RATE         = 0x2C;
const UBYTE ADXL345_DATA_FORMAT     = 0x31;
const UBYTE ADXL345_FIFO_CTL        = 0x38;
const UBYTE ADXL345_POWER_CTL       = 0x2D;
const UBYTE ADXL345_DATA_START      = 0x32;
const UBYTE ADXL345_DATA_STOP       = 0x37;
const UBYTE ADXL345_INT_SOURCE      = 0x30;
const UBYTE ADXL345_INT_ENABLE      = 0x2E;

//
int readADXLAddr(UBYTE address)
{
    UBYTE Address = ADXL345_ADDR << 1;
    UBYTE ReadAddress = Address | 0x01;
    UBYTE ans;
    I2CMasterStart();                   //Start condition
    I2CMasterWrite(ADXL345_ADDR);       //7 bit address + Write
    I2CMasterWrite(address);            //7 bit address + Write
    I2CMasterRepeatedStart();           //Restart condition
    I2CMasterWrite(ReadAddress);        //7 bit address + Read
    ans = I2CMasterRead(0);
    I2CMasterStop();                    //Stop condition
    return ans;
}

int writeADXLAddr(UBYTE address, UBYTE data)
{
    UBYTE Address = ADXL345_ADDR << 1;
    I2CMasterStart();               //Start condition
    I2CMasterWrite(address);        //7 bit address + Write
    I2CMasterWrite(data);           //Data
    I2CMasterStop();                //Stop condition
}
//
int initADXL()
{
   int ans;
   __delay_us(3000);
   ans = readADXLAddr(ADXL345_DEVID);
   if(ans == ADXL345_DEVID_VALUE){
       writeADXLAddr(ADXL345_POWER_CTL,0x08);      // autosleep=off,mode=measure
       writeADXLAddr(ADXL345_INT_ENABLE,0x80);     // interupe enable
       writeADXLAddr(ADXL345_BW_RATE,0x0B);        // rate = 200Hz
       writeADXLAddr(ADXL345_DATA_FORMAT,0x0B);    // proto=I2C,full resolution mode,range=16g
       writeADXLAddr(ADXL345_FIFO_CTL,0x00);       // FIFO=bypass
       __delay_us(3000);
   }else ans = -1;
   return ans;
}
//
int readADXL(UBYTE *data, int offset)
{
    UBYTE Address = ADXL345_ADDR << 1;
    UBYTE ReadAddress = Address | 0x01;
    int ans , i , ack ;
    UBYTE dataNumber;
    dataNumber = ADXL345_DATA_STOP - ADXL345_DATA_STOP +1;

    
    while(ans != 0x01){
        ans = readADXLAddr(ADXL345_INT_SOURCE);
        ans = ans & 0x01;
    }
    
    I2CMasterStart();                   //Start condition
    I2CMasterWrite(Address);            //7 bit address + Write
    I2CMasterWrite(ADXL345_DATA_START); //7 bit address + Write
    I2CMasterRepeatedStart();           //Restart condition
    I2CMasterWrite(ReadAddress);        //7 bit address + Read
    for (UBYTE i = 0; i < dataNumber-1; i++){
        data[offset+i] = I2CMasterRead(1);
    }
    data[offset + dataNumber-1] = I2CMasterRead(0);
    I2CMasterStop();                //Stop condition
}