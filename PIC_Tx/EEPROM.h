/* 
 * File:   EEPROM.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 2:17
 */

#include <xc.h>
#include "Type_define.h"

#ifndef EEPROM_H
#define	EEPROM_H

#define EEPROM_address 0x50
//#define EEPROM_Maddress 0x54
#define EEPROM_subaddress 0x52
//#define EEPROM_subMaddress 0x56
#define whigh_address 0x00
#define wlow_address 0xE0
#define rhigh_address 0x00
#define rlow_address 0xE0

//CW DOWNLINK
//GoogleDrive : BUS_EEPROM_memory map
//TODO : command size 8->16に変更になったらsub_addressが8E88から変更あり.OBCに確認
#define CW_DOWNLINK_EEPROM_DataLength 8
#define CW_DOWNLINK_MAIN_HIGH_ADDRESS 0x8E
#define CW_DOWNLINK_MAIN_LOW_ADDRESS  0x80
#define CW_DOWNLINK_SUB_HIGH_ADDRESS  0x8E
#define CW_DOWNLINK_SUB_LOW_ADDRESS   0x88

#endif	/* EEPROM_H */

