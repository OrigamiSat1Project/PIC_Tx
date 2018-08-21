#include <xc.h>
//#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "encode_AX25.h"

#define bit_H 0x01
#define bit_L 0x00
#define UCALL "JQ1YCZ"             //call sign of Tokyo Tech
#define MYCALL  "JS1YAX"             //call sign of OrigamiSat-1
#define PACKET_SIZE 47

//void SendPacket(void);
void SendByte(UBYTE);
void flipout(void);
void fcsbit(UBYTE);
UINT Packetmaker(UBYTE *);
//void test_Packetmaker(UBYTE *, UBYTE *);

UINT eflag = 0;
UINT efcsflag = 0;
UINT estuff = 0;
UBYTE efcslo, efcshi;
const UBYTE SSID_UCALL =0x60;
const UBYTE SSID_MYCALL =0xE1;
const UBYTE CONTROL =0xE1;
const UBYTE PID =0xF0;
const UINT NO_OF_START_FLAG = 28;
const UINT NO_OF_END_FLAG = 7;
//UBYTE eDataField[] = "Hello! I'm OrigamiSat1!!";
//UBYTE eDataField[] = "unko";
UBYTE ePacket[PACKET_SIZE];
UINT ebitstatus = low;

//
//void test_Packetmaker(UBYTE *eDataField){
//    UINT num_ = Packetmaker(eDataField);
//    for(UINT i=0;i<num_;i++){
//        putch(ePacket[i]);
//    }
//    putcrlf();
//}

UINT Packetmaker(UBYTE *eDataField){
    UINT Datanum;
    Datanum = 32;//TODO: change value of Datanum
    
    for(UINT i=0;i<6;i++){
        ePacket[i] = UCALL[i] << 1;
    }
    ePacket[6] = SSID_UCALL;  //SSID
    for(UINT i=0;i<6;i++){
        ePacket[i+7] = MYCALL[i] << 1;
    }
    ePacket[13] = SSID_MYCALL; //SSID.e1?
    ePacket[14] = CONTROL; //Control.30?
    ePacket[15] = PID; //PID
//    UINT Datanum = 0;
//    for(Datanum=0;eDataField[Datanum] != '\0';Datanum++);
//    for(Datanum=0;eDataField[Datanum] != 0xD9;Datanum++);
    for(UINT i=0;i<Datanum;i++){
        ePacket[16+i] = eDataField[i];
    }
    return 16+Datanum;
}

void SendPacket(UBYTE *eDataField){
    UINT Packetnum;
    Packetnum = 0;
    Packetnum = Packetmaker(eDataField);
    ebitstatus = 1;
    efcslo = efcshi = 0xff;
    estuff = 0;
    //  FlagField
    eflag = 1;
    efcsflag = 0;
    for(UINT i=1;i<=NO_OF_START_FLAG;i++){
        SendByte(0x7e);
    }
    eflag = 0;
    //  eDataField
    for(UINT i=0;i<Packetnum;i++){
        SendByte(ePacket[i]);
    }
    
    
    //  FCSField
    efcsflag = 1;
    efcslo = efcslo ^ 0xff;
    efcshi = efcshi ^ 0xff;
    SendByte(efcslo);
    SendByte(efcshi);
    efcsflag = 0;
    
    //  FlagField
    eflag = 1;
    for(UINT i=1;i<=NO_OF_END_FLAG;i++){
        SendByte(0x7e);
    }
}


void SendByte(UBYTE byte){
    UBYTE bt;
    for(UINT i=0;i<8;i++){
        bt = byte & bit_H;
        //  eDataField �� FCSCalculate
        if(efcsflag == 0 && eflag == 0){
            fcsbit(bt);
        }
        //  eDataField, FCSField �� bitestuffing
        if(bt == bit_L){
            flipout();
        }else{
            estuff ++;
            
            if(eflag == 0 && estuff == 5){
                __delay_us(espan);
                flipout();
            }
        }
        __delay_us(espan);
        byte = byte >> 1;
    }
}

//NRZI
void flipout(void){
    estuff = 0;
    if(ebitstatus == low){
        txd = high;
        ebitstatus = high;
    }else{
        txd = low;
        ebitstatus = low;
    }
}


//  FCSCalculation
void fcsbit(UBYTE tbyte){
    #asm
        BCF 03,0
        RRF _efcshi,F
        RRF _efcslo,F
    #endasm
    if(((STATUS & bit_H)^(tbyte)) == bit_H){
        efcshi = efcshi ^ 0x84;
        efcslo = efcslo ^ 0x08;
    }
}