/*** 時間制御 ***/

#include <htc.h>
#include "Type_define.h"
#include "time.h"
/**/
//msウェイト関数
void delay_ms(UWORD msec)
{
	while(msec)
	{
		__delay_ms(1);
		msec--;
	}	
}

//usウェイト関数
void delay_us(UWORD usec)
{
	while(usec)
	{
		__delay_us(1);
		usec--;
	}	
}

//sウェイト関数
void delay_s(UWORD sec)
{
	while(sec)
	{
		__delay_s(1);
		sec--;
	}	
}

UWORD time2ByteBinToDecimal(UBYTE timeHigh, UBYTE timeLow){
    int timeBin;
    UWORD timeDec = 0;
    
    timeBin = timeHigh<<8 | timeLow;
    
    int count = 1; // Initializing count value to 1, i.e 2^0 
    
    while (timeBin) 
    { 
        int last_digit = timeBin % 10; 
        timeBin = timeBin/10; 
          
        timeDec += last_digit*count; 
          
        count = count*2; 
    } 
    
    return timeDec;
}