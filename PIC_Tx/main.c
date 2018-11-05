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
#include "IMU.h"
#include "adxl345.h"
#include "ITG3701.h"
#include "ICM20601.h"
#include "ADC.h"

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


////test for interrupt
void interrupt interReceiverTest( void ){
    interruptI2C();
    
//   UBYTE RXDATA;
//   if (RCIF == 1) {
//       RXDATA = getChar();
//       //RXDATA++;
//       //putChar('G');
//       putChar(RXDATA);
//       
//       switch (RXDATA){
//           case 'b':
//               putChar(0xBB);
//               break;
//            case 'c':
//                putChar('C');
//                putChar('W');
//                
//                while(1){
//                CWKEY = 1;
//                __delay_ms(50);
//                CWKEY = 0;
//                __delay_ms(50);
//
//                CWKEY = 1;
//                __delay_ms(50);
//                CWKEY = 0;
//                __delay_ms(50);
//
//                CWKEY = 1;
//                __delay_ms(50);
//                CWKEY = 0;
//                __delay_ms(50);
//
//                CWKEY = 1;
//                __delay_ms(150);
//                CWKEY = 0;
//                __delay_ms(50);
//                }
//
//                putChar('C');
//                putChar('W');
//                putChar('2');
//                break;
//            case 'f':
//                putChar('F');
//                putChar('M');
//                CWKEY = 0;
//        //            __delay_ms(2000);
//                FMPTT = 1;
//                UBYTE EEPROMTestData[5];
//                EEPROMTestData[0] = 'H';
//                EEPROMTestData[1] = 'e';
//                EEPROMTestData[2] = 'l';
//                EEPROMTestData[3] = 'l';
//                EEPROMTestData[4] = 'o';
//                for (UINT i = 0; i< 10; i++){
//                    SendPacket(EEPROMTestData);
//                    __delay_ms(300);
//                }
//                
//                
//                
//                FMPTT = 0;
//                putChar('F');
//                putChar('M');  
//                putChar('2');      
//
//                break;
//       }
//       RCIF = 0;
//   }
}
    
void main(void) {
    __delay_ms(1000);
    /*?申?申?申?申?申?申*/
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
    Init_WDT();
    initADXL();
    initITG();
    initICM();
    
    sendPulseWDT();
    
    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    putChar('S');
    
//    UBYTE IMU[32];
//    UBYTE read[2];
//    readADXL(IMU,0);
//    __delay_us(20);
//    readITG(IMU,6);
//    __delay_us(20);
//    readICM(IMU,16);
//    __delay_us(20);
//    for(int i=0;i<32;i++) putChar(IMU[i]);
//    sendPulseWDT();
//    ADCreadtest();
//    delay_ms(200);
//    WriteOneByteToEEPROM(0x50,0x00,0x00,0xBE);
//    read[0] = ReadEEPROM(0x50,0x00,0x00);
//    putChar(read[0]);
//    __delay_ms(3);
//    WriteOneByteToEEPROM(0x52,0x00,0x00,0xBF);
//    read[1] = ReadEEPROM(0x52,0x00,0x00);
//    putChar(read[1]);
    
    
    
    
    
    while(1){
        putChar('m');
        sendPulseWDT();
       __delay_ms(500);
        //TODO check AD value
        //TODO send CW command
        //TODO send pulse to WDT
        
    }
    //return;
}
