#include <xc.h>
#include "WDT.h"
#include "Type_define.h"
#include "time.h"

void Init_WDT(void){
    RA1 = 0;
    sendPulseWDT();
}

void sendPulseWDT(void){
    if(RA1 = 0){
        RA0 = 0;
        __delay_ms(300);
        RA0 = 1;
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