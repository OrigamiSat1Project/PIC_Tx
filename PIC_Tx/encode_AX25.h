/* 
 * File:   encode_AX25.h
 * Author: Curie
 *
 * Created on 2017/04/02, 20:49
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
//#include <PIC16F886.h>
#include "UART.h"
#include "MPU.h"
#include "Type_define.h"
#include "time.h"

#ifndef ENCODE_AX25_H
#define	ENCODE_AX25_H

//#define mycall "JQ1ZHX"
//#define ucall  "JQ1YCZ"
#define txd RC5
#define UHFstart RC2
#define FMPTT RC0
#define CWKEY RB2
#define HEAT RB3

#define FLAG_AX25 0x7e
//void test_Packetmaker(UBYTE *);
/*
//void SendByte(UBYTE);
//void flipout(void);
//void fcsbit(UBYTE);
//UINT Packetmaker(void);
//void test_Packetmaker(void);
*/

void SendPacket(UBYTE *,UINT);

#endif	/* ENCODE_AX25_H */

