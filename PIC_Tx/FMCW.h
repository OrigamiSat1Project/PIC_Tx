/* 
 * File:   FMCW.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 17:45
 */

#ifndef FMCW_H
#define	FMCW_H

#include "Type_define.h"

void Init_FMCW(void);

void _NOP(void);

void downlinkReceivedCommand(UBYTE, UBYTE, UBYTE, UBYTE);
void downlinkFMSignal(UBYTE,UBYTE, UBYTE, UBYTE, UBYTE);

void Morse_V(void);
int changeCharMorse (char);
void sendMorse(char*);
void sendMorseForTest(char data);

#endif	/* FMCW_H */

