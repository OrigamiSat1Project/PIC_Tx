/* 
 * File:   I2C.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 0:41
 */

#include <xc.h>
#include "Type_define.h"

#ifndef I2C_H
#define	I2C_H

#define I2Cbps 400000
#define I2Cnull 0xFF
 
void InitI2CMaster(const UDWORD c);
void I2CMasterWait(void);
void I2CMasterStart(void);
void I2CMasterRepeatedStart(void);
void I2CMasterStop(void);
void I2CMasterWrite(unsigned d);
UBYTE I2CMasterRead(UBYTE a);
void WriteToEEPROM(UBYTE,UBYTE,UBYTE,UBYTE *);
void WriteOneByteToEEPROM(UBYTE,UBYTE,UBYTE,UBYTE);
void WriteCheckByteToEEPROMs(UBYTE,UBYTE,UBYTE,UBYTE);
void ReadDataFromEEPROM(UBYTE ,UBYTE ,UBYTE,UBYTE *,UINT);
UBYTE ReadEEPROM(UBYTE, UBYTE, UBYTE);
void ReadDataAndDataSizeFromEEPROM(UBYTE ,UBYTE ,UBYTE,UBYTE *,UINT *);
void measureBatteryVoltage(UBYTE*);
void commandSwitchI2C(UBYTE , UBYTE , UBYTE *, UBYTE *);


#endif	/* I2C_H */
