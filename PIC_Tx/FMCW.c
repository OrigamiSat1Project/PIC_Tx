
#include <xc.h>
#include "FMCW.h"
#include "EEPROM.h"
#include "pinDefine.h"
#include "I2C.h"
#include "CRC16.h"
#include "encode_AX25.h"

#include "time.h"


#define EEPROM_COMMAND_DATA_SIZE 32

UBYTE commandData[EEPROM_COMMAND_DATA_SIZE];

/*
 * �y�����������s��Ȃ��i�ҋ@�j�z
 *  5�������ҋ@����
 */
void _NOP(void) {
    for(int i=0; i<5; i++){
        NOP();
    }
}


void downlinkReceivedCommand(UBYTE B0Select, UBYTE addressHigh, UBYTE addressLow, UBYTE downlinlTimes){
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
    for(int sendCounter = 0; sendCounter < downlinlTimes; sendCounter++){
        SeparateAndSendPacket(commandData,EEPROM_COMMAND_DATA_SIZE);
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
        SeparateAndSendPacket(readData,DataSize);
        __delay_ms(300);
    }
    FMPTT = 0;
}
void SeparateAndSendPacket(UBYTE *eDataField,UINT DataSize){
    UINT maxPacketDataSize =  32;
    UINT PacketSize;
    PacketSize = DataSize/maxPacketDataSize;
    UINT PacketCounter = 0;
    while (PacketCounter < PacketSize){
        //downlink signal which has 32 byte data
        UBYTE eDataFieldOfOnePacket[];
        for (int i = 0; i<maxPacketDataSize;i++){
            eDataFieldOfOnePacket[i] = eDataField[maxPacketDataSize*PacketCounter + i];
        }
        SendPacket(eDataFieldOfOnePacket,maxPacketDataSize);
        PacketCounter++;
    }
    if (DataSize%maxPacketDataSize != 0){//downlink last packet which does not have 32 byte
        UINT LastPacketSize;
        LastPacketSize = DataSize%maxPacketDataSize;
        UBYTE eDataFieldOfLastPacket[];
        for (int i = 0; i< LastPacketSize;i++){
            eDataFieldOfLastPacket[i] = eDataField[maxPacketDataSize*PacketSize + i];
        }
//        SendPacket(eDataFieldOfLastPacket,LastPacketSize);
    }
}
/*
 �ꕶ������
tu = 3ton
ton-ton�� = 1ton
tu-tu�� = 1ton
tu-ton�� = 1ton
������
3ton

�P���
7ton
 */