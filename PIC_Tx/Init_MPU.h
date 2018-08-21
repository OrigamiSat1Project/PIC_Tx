/* 
 * File:   Initmpu.h
 * Author: Curie
 *
 * Created on 2017/01/21, 20:52
 */

#ifndef INITMPU_H
#define	INITMPU_H

#include "Type_define.h"

//#define led_yellow RB1
//#define FX614_RXD RB0
#define high 1
#define low  0
#define ledy_on() led_yellow = high;
#define ledy_off() led_yellow = low;



void Init_MPU(void);
void led_onoff(void);
void debug_ledy(void);


#endif	/* INITMPU_H */

