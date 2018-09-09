//#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <PIC16F886.h>
#include "UART.h"
#include "MPU.h"
#include "Type_define.h"
#include "time.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "WDT.h"
#include "CW.h"
#include "pinDefine.h"
#include "CRC16.h"

void interrupt InterReceiver(void);

#define COMMAND_SIZE 10;


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

/* PIC16F887 Configuration Bit Settings */
#pragma config FOSC     = HS            // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE     = OFF           // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE    = ON            // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE    = ON            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP       = OFF           // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD      = OFF           // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN    = OFF            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO     = OFF            // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN    = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP      = OFF           // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V    = BOR40V        // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      = OFF           // Flash Program Memory Self Write Enable bits (Write protection off)


//test_interrupt
//pc-->pic-->pc 
//void interrupt interReceiverTest( void ){
//    UBYTE RXDATA;
//    if (RCIF == 1) {
//        RXDATA = getChar();
//        RXDATA++;
//        putChar('S');
//        RCIF = 0;
//    }
//}

/**/
//test_get EEPROM address
//pc-->pic-->pc 
//void interrupt InterReceiver( void ){
//    UBYTE RXDATA[3];
//    UINT COMMAND_SIZE;
//    COMMAND_SIZE =3;
//    if (RCIF == 1) {
//        for(UINT i=0; i<COMMAND_SIZE; i++){
//            RXDATA[i] =getChar();
//            putChar(RXDATA[i]);
//            NOP();
//        }
//        putChar('s');
//        RCIF = 0;
//    }
//}

//
//void interrupt InterReceiver(void){
//    putChar('I');
//    
//    int commandSize;
//    //commandSize = 10;
//    commandSize = 1;
//    
//    //UBYTE RXDATA[10];//array size = commandSize
//    UBYTE RXDATA[1];//array size = commandSize
//
//    //    UBYTE RXDATA[COMMAND_SIZE];
////    volatile static int intr_counter;
//    if (RCIF == 1) {
//        for (int i = 0; i < 1; i++){
//            RXDATA[i] = getChar();
////            putChar(RXDATA[i]);
//        }
//        for (int i = 0; i < 1; i++){
//            putChar(RXDATA[i]);
//            NOP();
//        }
//       //TODO add case RXDATA[0]!=t or g
////        UWORD crcResult, crcValue;
////        UBYTE crcResultHigh,crcResultLow,crcValueHigh,crcValueLow;
////        crcResult = crc16(0,RXDATA,8);
////        crcValue =  CRC_check(RXDATA,8);
////        crcResultHigh = crcResult>>8;
////        crcResultLow = crcResult & 0x00FF;
////        crcValueHigh = crcValue>>8;
////        crcValueLow = crcValue & 0x00FF;
////        
////        putChar(crcResultHigh);
////        putChar(crcResultLow);
////        putChar(crcValueHigh);
////        putChar(crcValueLow);
//        
////        if(crcResult == crcValue){
//            putChar('C');
//            switch(RXDATA[1]){
//                case 0x75:
//                    putChar('R');
//                    downlinkReceivedCommand(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5]);
//                    break;
//                case 0x63:
////                    CwDownLink(RXDATA);
//                    putChar('C');
//                    putChar('W');
//                    CWKEY = 1;
//                    __delay_ms(50);
//                    CWKEY = 0;
//                    __delay_ms(50);
//
//                    CWKEY = 1;
//                    __delay_ms(50);
//                    CWKEY = 0;
//                    __delay_ms(50);
//
//                    CWKEY = 1;
//                    __delay_ms(50);
//                    CWKEY = 0;
//                    __delay_ms(50);
//
//                    CWKEY = 1;
//                    __delay_ms(150);
//                    CWKEY = 0;
//                    __delay_ms(50);
//                    putChar('C');
//                    putChar('W');
//                    
//                    break;
//                case 0x66:
//                    putChar('F');
//                    putChar('M');
//                    __delay_ms(2000);
//                    FMPTT = 1;
//                    __delay_ms(2000);
//                    FMPTT = 0;
//                    __delay_ms(2000);
//                    FMPTT = 1;
//                    __delay_ms(2000);
//                    FMPTT = 0;
//                    putChar('F');
//                    putChar('M');
//                    downlinkFMSignal(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6]);
//                    break;
//                case 0x61:
//                    cutWire(RXDATA[2],RXDATA[3]);
//                    break;
//            }
////        }else{
////            
////            putChar('D');
////            ///コマンドCRCダメだった時の処理
////        }
//        RCIF = 0;
////    }
//}

void interrupt interReceiverTest( void ){
    UBYTE RXDATA;
    if (RCIF == 1) {
        RXDATA = getChar();
        //RXDATA++;
        //putChar('G');
        putChar(RXDATA);
        
        switch (RXDATA){
        case 'c':
            putChar('C');
            putChar('W');
            
            while(1){
            CWKEY = 1;
            __delay_ms(50);
            CWKEY = 0;
            __delay_ms(50);

            CWKEY = 1;
            __delay_ms(50);
            CWKEY = 0;
            __delay_ms(50);

            CWKEY = 1;
            __delay_ms(50);
            CWKEY = 0;
            __delay_ms(50);

            CWKEY = 1;
            __delay_ms(150);
            CWKEY = 0;
            __delay_ms(50);
            }

            putChar('C');
            putChar('W');
            putChar('2');
            break;
        case 'f':
            putChar('F');
            putChar('M');
            CWKEY = 0;
//            __delay_ms(2000);
            FMPTT = 1;
            UBYTE EEPROMTestData[5];
            EEPROMTestData[0] = 'H';
            EEPROMTestData[1] = 'e';
            EEPROMTestData[2] = 'l';
            EEPROMTestData[3] = 'l';
            EEPROMTestData[4] = 'o';
            for (UINT i = 0; i< 10; i++){
                SendPacket(EEPROMTestData);
                __delay_ms(300);
            }
            
            
            
            FMPTT = 0;
            putChar('F');
            putChar('M');  
            putChar('2');      

            break;
        }
        RCIF = 0;
    }
}
    
    

void main(void) {
    __delay_ms(1000);
    /*初期化*/
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
//    Init_WDT();
    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    putChar('S');
    
    
    while(1){
        /*
        while(1){
            //getbit();
            //debug_ledy();   //6us
            putch('O');
            __delay_ms(500);
        }*/
        putChar('m');
       __delay_ms(1000);
//        FMPTT = 1;
//        __delay_ms(2000);
//        FMPTT = 0;
        //TODO check AD value
        //TODO send CW command
        //TODO send pulse to WDT
        
    }
    //return;
}
