
#include <xc.h>
#include "FMCW.h"
#include "EEPROM.h"
#include "pinDefine.h"
#include "I2C.h"
#include "CRC16.h"
#include "encode_AX25.h"

#include "time.h"


#define EEPROM_COMMAND_DATA_SIZE 32
#define MAX_DOWNLINK_DATA_SIZE 32

UBYTE commandData[EEPROM_COMMAND_DATA_SIZE];

/*
 * FMCW initialization
 * setting FM and CW 
 */
void Init_FMCW(void){
    /* ポートをLowにする（初期化） */
    FMPTT = 0;
    CWKEY = 0;
}


/*
 * 【何も処理を行わない（待機）】
 *  5処理分待機する
 */
void _NOP(void) {
    for(int i=0; i<5; i++){
        NOP();
    }
}


void downlinkReceivedCommand(UBYTE B0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlkTimes){
    UBYTE mainAddress;
    UBYTE subAddress;
    mainAddress = EEPROM_address | B0Select;
    subAddress = EEPROM_subaddress | B0Select;
    ReadDataFromEEPROM(mainAddress,addressHigh,addressLow, commandData,EEPROM_COMMAND_DATA_SIZE);
    if(crc16(0,commandData,29) == CRC_check(commandData,29)){
        commandData[31] = 0x0F;
    }else{
        ReadDataFromEEPROM(subAddress,addressHigh,addressLow, commandData,EEPROM_COMMAND_DATA_SIZE);
        if(crc16(0,commandData,29) == CRC_check(commandData,29)){
            commandData[31] = 0x6F;
        }else{
            commandData[31] = 0xFF;
        }
    }
    WriteCheckByteToEEPROMs(B0Select,addressHigh,addressLow, commandData[31]);
    __delay_ms(200);
    FMPTT = 1;
    for(int sendCounter = 0; sendCounter < downlinlkTimes; sendCounter++){
        SendPacket(commandData,EEPROM_COMMAND_DATA_SIZE);
        __delay_ms(300);
    }
    FMPTT = 0;
    
    /*-------------------------------------------------*/
    if(commandData[0]=='T'){                //command target = PIC_TX
        //Task target
        if(commandData[2] == 't'){          //task target =  PIC_TX
        // Command type
            switch(commandData[3]){         //Process command type
            case 'm': /*change sattelite mode*/
//                commandSwitchSatMode(commandData[4], commandData[5], commandData[6]);
                break;
            case 'p': /*power supply*/
//                commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
                break;
            case 'n': /*radio unit*/
//                commandSwitchFMCW(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                break;
            case 'i':/*I2C*/
//                commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7]);
                break;
            case 'u':/*UART*/
//                commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                break;
            case 'w':/*WDT (watch dog timer)*/
//                commandWDT(commandData[4], commandData[5], commandData[6]);
                break;
            case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
                //TODO: write function directly here or in MPU.c
                break;
            case 'r':/*internal processing*/
//                commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);
                break;
            case 'f':/*downlink FM Signal*/
                downlinkFMSignal(commandData[4],commandData[5],commandData[6],commandData[7],commandData[8]);
                break;
            default:
                //TODO: error message
                break;
            }
        }
        
    }
}

void downlinkFMSignal(UBYTE EEPROMAndB0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlTimes,UBYTE DataSize){
    UBYTE readAddress;
    readAddress = EEPROM_address | EEPROMAndB0Select;
    UBYTE readData[];
    ReadDataFromEEPROM(readAddress,addressHigh,addressLow, readData,DataSize);
    FMPTT = 1;
    __delay_ms(100);//TODO check time
    for(int sendCounter = 0; sendCounter < downlinlTimes; sendCounter++){
        SendPacket(readData,DataSize);
        __delay_ms(300);
    }
    FMPTT = 0;
}

/*
 一文字中の
tu = 3ton
ton-ton間 = 1ton
tu-tu間 = 1ton
tu-ton間 = 1ton
文字間
3ton

単語間
7ton
 */

/*
 * send morse test data 'V'
 *  1. CWKEY端子を0→1→0と変化させる
 *  2. ※1.を計３回行う
 */
void Morse_V(void){
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


/*
 *  translate char to morse
 *      arg     : char
 *      return  : bin (morse)
 */
int changeCharMorse (char _c){
    switch(_c){
        case '0': return 0b1110111011101110111;
        case '1': return 0b11101110111011101;
        case '2': return 0b111011101110101;
        case '3': return 0b1110111010101;
        case '4': return 0b11101010101;
        case '5': return 0b101010101;
        case '6': return 0b10101010111;
        case '7': return 0b1010101110111;
        case '8': return 0b101011101110111;
        case '9': return 0b10111011101110111;
        case 'a':
        case 'A': return 0b11101;
        case 'b':
        case 'B': return 0b101010111;
        case 'c':
        case 'C': return 0b10111010111;
        case 'd':
        case 'D': return 0b1010111;
        case 'e':
        case 'E': return 0b1;
        case 'f':
        case 'F': return 0b101110101;
        case 'g':
        case 'G': return 0b101110111;
        case 'h':
        case 'H': return 0b1010101;
        case 'i':
        case 'I': return 0b101;
        case 'j':
        case 'J': return 0b1110111011101;
        case 'k':
        case 'K': return 0b111010111;
        case 'l':
        case 'L': return 0b101011101;
        case 'm':
        case 'M': return 0b1110111;
        case 'n':
        case 'N': return 0b10111;
        case 'o':
        case 'O': return 0b11101110111;
        case 'p':
        case 'P': return 0b10111011101;
        case 'q':
        case 'Q': return 0b1110101110111;
        case 'r':
        case 'R': return 0b1011101;
        case 's':
        case 'S': return 0b10101;
        case 't':
        case 'T': return 0b111;
        case 'u':
        case 'U': return 0b1110101;
        case 'v':
        case 'V': return 0b111010101;
        case 'w':
        case 'W': return 0b111011101;
        case 'x':
        case 'X': return 0b11101010111;
        case 'y':
        case 'Y': return 0b1110111010111;
        case 'z':
        case 'Z': return 0b10101110111;
        default : return 0;
    }
}

/*
 *  send morse
 *      arg     : char *
 *      return  : N/A
 */
void sendMorse(char *HK_Data){
    int data_size = sizeof(HK_Data)/sizeof(HK_Data[0]);
    for (int i = 0;i<data_size;i++){
        int mo = changeCharMorse(HK_Data[i]);
        for (int n=0;n<19;n++){
            if(mo==0){
                break;
            } else if((mo&1)==1){
                CWKEY = 1;
                __delay_ms(50);
            }else{
                CWKEY = 0;
                __delay_ms(50);
            }
            mo=mo>>1;
        }
        CWKEY = 0;
        __delay_ms(150);
    }
}

void sendMorseForTest(char data){
    int mo = changeCharMorse(data);
    for (int n=0;n<19;n++){
        if(mo==0){
            break;
        } else if((mo&1)==1){  //1bit : 0->CWKEY = 0; / 1->CWKEY = 1;
            CWKEY = 1;
            __delay_ms(50);
        }else{
            CWKEY = 0;
            __delay_ms(50);
        }
        mo=mo>>1;
    }
    CWKEY = 0;
    __delay_ms(150);
}