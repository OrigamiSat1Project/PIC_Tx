#include <xc.h>
#include "WDT.h"
#include "Type_define.h"
#include "pinDefine.h"

void resetWDT(void);

void Init_WDT(void){
    INTCONbits.GIE  = 1;    // Grobal interrupt??????
    INTCONbits.PEIE = 1;    // ???????????
    
    OPTION_REGbits.PSA = 1;
    OPTION_REGbits.PS = 7;
    
//    WDTCONbits.WDTPS = 11;
 
    PIR1bits.TMR1IF = 0;    // Timer1??????????????
    PIE1bits.TMR1IE = 1;    // Timer1???????
 
 
    T1CONbits.T1CKPS = 3;   // ???????1/1???
    TMR1 = TIMER_INTERVAL;  // ????????????
 
    T1CONbits.TMR1ON = 1;   // Timer1??????
}

//process command data if the command type is 'WDT' 
void commandWDT(UBYTE command, UBYTE timeHigh, UBYTE timeLow){   //times are given in ms
    if(command = 's'){
        //Command leads to a reset of PIC (instead of "stop send pulse" use "resetWDT", 
        //this should have same result, but safes the time since it is not necessary
        //to wait until WDT realises that phase is missing (would take about 11 sec))
        resetWDT();
    }else{
        //TODO: return error message
    }
}

//Resets the WDT and as consequence also resets the TX_PIC; LOW==ON HIGH==OFF
void resetWDT(void){
    if(WDT_POWER== LOW){
       WDT_POWER= HIGH;//TODO: check if a delay is needed
       WDT_POWER= LOW;
    }else{
       WDT_POWER= LOW;//TODO: check if a delay is needed
       WDT_POWER= HIGH;
    }
}

//
//void interrupt intr(void){
//    volatile static int intr_counter;
//    if ( PIR1bits.TMR1IF == 1 ) {
//        TMR1 = TIMER_INTERVAL;  // ?????????
// 
//        intr_counter++;
//        if ( intr_counter >= 100 ) {
//            intr_counter = 0;
//        }
// 
//        // 0.5sec???RB0???????    
//        if ( intr_counter <= 50 || intr_counter > 51) {
//            PORTAbits.RA0 = 1;
//        } else {
//            PORTAbits.RA0 = 0;
//        }
// 
//        PIR1bits.TMR1IF = 0;    // ???????????
//    }   
// 
//    return;
//}