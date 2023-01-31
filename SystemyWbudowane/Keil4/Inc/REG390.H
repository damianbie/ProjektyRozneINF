#ifndef REG390
#define REG390
/*--------------------------------------------------------------------------
REG390.H

Header file for Dallas DS80C390.
Copyright (c) 1988-2000 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
//#include "def.h"


sfr  P4    =0x80;
sbit P4_7  =P4^7;  sbit P4_6  =P4^6;  sbit P4_5  =P4^5;  sbit P4_4  =P4^4;
sbit P4_3  =P4^3;  sbit P4_2  =P4^2;  sbit P4_1  =P4^1;  sbit P4_0  =P4^0;

sfr  SP    =0x81;
sfr  DPL   =0x82;  sfr  DPH   =0x83;
sfr  DPL1  =0x84;  sfr  DPH1  =0x85; /* DS87C390 Extensions  */
sfr  DPS   =0x86;  /* DS87C390 Extensions  */
/* b7 =ID1,  b6 =ID0,  b5 =TSL,  b0 =SEL  */

sfr  PCON  =0x87;
enum {
  SMOD_0   =0x80, /* b7  */
  SMOD0    =0x40, /* b6  */
  OFDF     =0x20, /* b5  */
  OFDE     =0x10, /* b4  */
  GF1      =0x08, /* b3  */
  GF2      =0x04, /* b2  */
  STOP     =0x02, /* b1  */
  IDLE     =0x01  /* b0  */
};

sfr  TCON  =0x88;
sbit TF1   =TCON^7;  sbit TR1   =TCON^6;
sbit TF0   =TCON^5;  sbit TR0   =TCON^4;
sbit IE1   =TCON^3;  sbit IT1   =TCON^2;
sbit IE0   =TCON^1;  sbit IT0   =TCON^0;

sfr  TMOD  =0x89;
/* b7 =GATE, b6 =C/-T, b5 =M1, b4 =M0  -Timer 1  */
/* b3 =GATE, b2 =C/-T, b1 =M1, b0 =M0  -Timer 2  */

sfr  TL0   =0x8A;  sfr  TH0   =0x8C;
sfr  TL1   =0x8B;  sfr  TH1   =0x8D;

sfr  CKCON =0x8E; /* DS87C390 Extensions  */
/* b7 =WD1, b6 =WD0, b5 =T2M, b4 =T1M  */
/* b3 =T0M, b2 =MD2, b1 =MD1, b0 =MD0  */

sfr  P1    =0x90;

/* External Interrupt Flag  */
sfr  EXIF  =0x91; /* DS87C390 Extensions  */
enum {
  IE5      =0x80, /* b7  */
  IE4      =0x40, /* b6  */
  IE3      =0x20, /* b5  */
  IE2      =0x10, /* b4  */
  CKRY     =0x08, /* b3  */
  RGMD     =0x04, /* b2  */
  RGSL     =0x02, /* b1  */
  BGS      =0x01  /* b0  */
};

sfr  P4CNT  =0x92; /* DS87C390 Extensions -Timed Access Protected  */
/* b7 -,  */
/* b6 =SBCAN  */
#define SBCAN      0x40
/* b5 =P4CNT.5, P4CNT.4, P4CNT.3  */
/* b2 =P4CNT.2, P4CNT.1, P4CNT.0  */

sfr  DPX    =0x93; /* DS87C390 Extensions  */
sfr  DPX1   =0x95; /* DS87C390 Extensions  */

sfr  C0RMS0 =0x96; /* DS87C390 Extensions  */
sfr  C0RMS1 =0x97; /* DS87C390 Extensions  */

sfr  SCON0 =0x98;
sbit SM0   =SCON0^7; /* Alternate SM0_FE_0 */
sbit SM1   =SCON0^6; /* Alternate SM1_0 */
sbit SM2   =SCON0^5; /* Alternate SM2_0 */
sbit REN   =SCON0^4; /* Alternate REN_0 */
sbit TB8   =SCON0^3; /* Alternate TB8_0 */
sbit RB8   =SCON0^2; /* Alternate RB8_0 */
sbit TI    =SCON0^1; /* Alternate TI_0  */
sbit RI    =SCON0^0; /* Alternate RI_0  */

sfr  SBUF0 =0x99;

sfr  ESP   =0x9B; /* DS87C390 Extensions  */
/* b1 =ESP.1,  b0 =ESP.0  */

sfr  AP    =0x9C; /* DS87C390 Extensions  */
sfr  ACON  =0x9D; /* DS87C390 Extensions  */
/* b2 =AP,  b1 =AM1, b0 =AM0  */

sfr  C0TMA0 =0x9E; /* DS87C390 Extensions  */
sfr  C0TMA1 =0x9F; /* DS87C390 Extensions  */

sfr  P2     =0xA0;

sfr  P5     =0xA1; /* DS87C390 Extensions  */
sfr  P5CNT  =0xA2; /* DS87C390 Extensions Timed Access Protected  */
enum {
  CAN1BA    =0x80, /* b7 -CAN 1 Bus Active  */
  CAN0BA    =0x40, /* b6 -CAN 0 Bus Active  */
  SP1EC     =0x20, /* b5 -Serial Port 1 External Connection  */
  C1_IO     =0x10, /* b4 -CAN 1 IO Enable  */
  C0_IO     =0x08, /* b3 -CAN 0 IO Enable  */
  P5CNT_2   =0x04, /* b2 -P5.7-4 Configuration Control Bits  */
  P5CNT_1   =0x02,
  P5CNT_0   =0x01
};

/* CAN 0 Control Register
sfr  C0C    =0xA3; /* DS87C390 Extensions  */
enum {
  ERIE      =0x80, /* b7 =ERIE   -Error Interrupt Enable  */
  STIE      =0x40, /* b6 =STIE   -Statis Interrupt Enable  */
  PDE       =0x20, /* b5 =PDE    -Power-Donw Enable  */
  SIESTA    =0x10, /* b4 =SIESTA -Sieta Mode Enable  */
  CRST      =0x08, /* b3 =CRST   -Reset (=1)  */
  AUTOB     =0x04, /* b2 =AUTOB  -Auto Baude  */
  ERCS      =0x02, /* b1 =ERCS   -Error Count Select  */
  SWINT     =0x01  /* b0 =SWINT -Software Initialization Enable 1->0 loaded TSEG1,2  */
};

/* CAN 0 Status Register  */
sfr  C0S    =0xA4; /* DS87C390 Extensions  */
enum {
  BUSOFF    =0x80, /* b7 =BUSOFF   -Bus off  */
  CECE      =0x40, /* b6 =CECE     -Error Count Exceeded  */
  WKS       =0x20, /* b5 =WKS      -Wake-Up Status  */
  RXS       =0x10, /* b4 =RXS      -Receive Status  */
  TXS       =0x08, /* b3 =TXS      -Transmit Status  */
  ER2       =0x04, /* b2 =ER2-0    -Bus Error Status  */
  ER1       =0x02,
  ER0       =0x01
};

/* CAN 0  */
sfr  C0IR   =0xA5; /* DS87C390 Extensions  */
/* INTIN7 INTIN6 INTIN5 INTIN4 INTIN3 INTIN2 INTIN1 INTIN0  */

sfr  C0TE   =0xA6; /* DS87C390 Extensions  */
sfr  C0RE   =0xA7; /* DS87C390 Extensions  */

sfr  IE     =0xA8;
sbit EA     =IE^7;
sbit ES1    =IE^6;
sbit ET2    =IE^5;
sbit ES0    =IE^4;
sbit ET1    =IE^3;  sbit EX1   =IE^2;
sbit ET0    =IE^1;  sbit EX0   =IE^0;

sfr  SADDR0 =0xA9; /* DS87C390 Extensions  */
sfr  SADDR1 =0xAA; /* DS87C390 Extensions  */

/* definicja bitów w rej CxMyC  */
/* x -CAN No  */
/* y -Message Center No  */
enum {
  MSRDY     =0x80, /* b7 -Ready  */
  ETI       =0x40, /* b6 -Enable Transmit Interrupt  */
  ERI       =0x20, /* b5 -Enable Receive Interrupt  */
  INTRQ     =0x10, /* b4 -Interrupt Request  */
  EXTRQ     =0x08, /* b3 -External Transmit Request  */
  MTRQ      =0x04, /* b2 -Microcontroller Transmit Request  */
  ROW       =0x02, /* b1 -Receive Overwrite  */
  TIH       =0x02, /* b1 -Transmit Inhibit  */
  DTUP      =0x01  /* b0 -Data Update  */
};

sfr  C0M1C  =0xAB; /* DS87C390 Extensions  */
sfr  C0M2C  =0xAC; /* DS87C390 Extensions  */
sfr  C0M3C  =0xAD; /* DS87C390 Extensions  */
sfr  C0M4C  =0xAE; /* DS87C390 Extensions  */
sfr  C0M5C  =0xAF; /* DS87C390 Extensions  */

sfr  P3     =0xB0;

/* b7 =P3.7  */
/* b6 =P3.6  */
sbit T1     =P3^5;
sbit T0     =P3^4;
sbit INT1   =P3^3;
sbit INT0   =P3^2;
sbit TXD0   =P3^3;
sbit RXD0   =P3^2;

sfr  C0M6C  =0xB3; /* DS87C390 Extensions  */
sfr  C0M7C  =0xB4; /* DS87C390 Extensions  */
sfr  C0M8C  =0xB5; /* DS87C390 Extensions  */
sfr  C0M9C  =0xB6; /* DS87C390 Extensions  */
sfr  C0M10C =0xB7; /* DS87C390 Extensions  */

sfr  IP     =0xB8;
sbit PS1    =IP^6;
sbit PT2    =IP^5;
sbit PS0    =IP^4;
sbit PT1    =IP^3;  sbit PX1   =IP^2;
sbit PT0    =IP^1;  sbit PX0   =IP^0;

sfr  SADEN0 =0xB9; /* DS87C390 Extensions  */
sfr  SADEN1 =0xBA; /* DS87C390 Extensions  */

sfr  C0M11C =0xBB; /* DS87C390 Extensions  */
sfr  C0M12C =0xBC; /* DS87C390 Extensions  */
sfr  C0M13C =0xBD; /* DS87C390 Extensions  */
sfr  C0M14C =0xBE; /* DS87C390 Extensions  */
sfr  C0M15C =0xBF; /* DS87C390 Extensions  */

sfr  SCON1  =0xC0; /* DS87C390 Extensions  */
sbit SM01   =SCON1^7; /* Alternate SM0_FE_1 */
sbit SM11   =SCON1^6; /* Alternate SM1_1 */
sbit SM21   =SCON1^5; /* Alternate SM2_1 */
sbit REN1   =SCON1^4; /* Alternate REN_1 */
sbit TB81   =SCON1^3; /* Alternate TB8_1 */
sbit RB81   =SCON1^2; /* Alternate RB8_1 */
sbit TI1    =SCON1^1; /* Alternate TI_1  */
sbit RI1    =SCON1^0; /* Alternate RI_1  */

sfr  SBUF1  =0xC1; /* DS87C390 Extensions  */
sfr  PMR    =0xC4; /* DS87C390 Extensions  */
sfr  STATUS =0xC5; /* DS87C390 Extensions  */
sfr  MCON   =0xC6; /* DS87C390 Extensions  */
/* b7 -IDM1  */
/* b6 -IDM0  */
/* b5 -CMA  =1 CAN_MC =0x401000, =0 -0x00EE00   */
#define CMA_0  MCON &=~0x20
#define CMA_1  MCON |= 0x20
/* b4 -?     */
/* b3 PDCE3  */
/* b2 PDCE2  */
/* b1 PDCE1  */
/* b0 PDCE0  */

sfr  TA     =0xC7; /* DS87C390 Extensions  */

   void  TimedAccessProtected(void);
#define  TimedAccessProtected() TA =0xAA; TA =0x55

sfr  T2CON  =0xC8; /* DS87C390 Extensions  */
sbit TF2    =T2CON^7;
sbit EXF2   =T2CON^6;
sbit RCLK   =T2CON^5;
sbit TCLK   =T2CON^4;
sbit EXEN2  =T2CON^3;
sbit TR2    =T2CON^2;
sbit C_T2   =T2CON^1;
sbit CP_RL2 =T2CON^0;

sfr  T2MOD  =0xC9; /* DS87C390 Extensions  */
sfr  RCAP2L =0xCA; /* DS87C390 Extensions  */
sfr  RCAP2H =0xCB; /* DS87C390 Extensions  */
sfr  TL2    =0xCC; /* DS87C390 Extensions  */
sfr  TH2    =0xCD; /* DS87C390 Extensions  */
sfr  COR    =0xCE; /* DS87C390 Extensions  */

sfr  PSW    =0xD0;
sbit CY     =PSW^7;
sbit AC     =PSW^6;
sbit F0     =PSW^5;
sbit RS1    =PSW^4;
sbit RS0    =PSW^3;
sbit OV     =PSW^2;
sbit F1     =PSW^1;
sbit P      =PSW^0;

sfr  MCNT0  =0xD1; /* DS87C390 Extensions  */
sfr  MCNT1  =0xD2; /* DS87C390 Extensions  */
sfr  MA     =0xD3; /* DS87C390 Extensions  */
sfr  MB     =0xD4; /* DS87C390 Extensions  */
sfr  MC     =0xD5; /* DS87C390 Extensions  */
sfr  CIRMS0 =0xD6; /* DS87C390 Extensions  */
sfr  CIRMS1 =0xD7; /* DS87C390 Extensions  */

/* Watch Dog  */
sfr  WDCON  =0xD8; /* DS87C390 Extensions  */
sbit SMOD_1 =WDCON^7;
sbit POR    =WDCON^6;
sbit EPF1   =WDCON^5;
sbit PF1    =WDCON^4;
sbit WDIF   =WDCON^3;
sbit WTRF   =WDCON^2;
sbit EWT    =WDCON^1;
sbit RWT    =WDCON^0;

sfr  CITMA0 =0xDE; /* DS87C390 Extensions  */
sfr  CITMA1 =0xDF; /* DS87C390 Extensions  */

sfr  ACC    =0xE0;

sfr  C1C    =0xE3; /* DS87C390 Extensions  */
sfr  C1S    =0xE4; /* DS87C390 Extensions  */
sfr  C1IR   =0xE5; /* DS87C390 Extensions  */
sfr  C1TE   =0xE6; /* DS87C390 Extensions  */
sfr  C1RE   =0xE7; /* DS87C390 Extensions  */

sfr  EIE    =0xE8; /* DS87C390 Extensions
sbit CANBIE =EIE^7;
sbit C0IE   =EIE^6;
sbit C1IE   =EIE^5;
sbit EWDI   =EIE^4;
sbit EX5    =EIE^3; /* Alternate EC3 */
sbit EX4    =EIE^2; /* Alternate EC2 */
sbit EX3    =EIE^1; /* Alternate EC1 */
sbit EX2    =EIE^0; /* Alternate EC0 */

sfr  MXAX   =0xEA; /* DS87C390 Extensions  */
sfr  C1M1C  =0xEB; /* DS87C390 Extensions  */
sfr  C1M2C  =0xEC; /* DS87C390 Extensions  */
sfr  C1M3C  =0xED; /* DS87C390 Extensions  */
sfr  C1M4C  =0xEE; /* DS87C390 Extensions  */
sfr  C1M5C  =0xEF; /* DS87C390 Extensions  */

sfr  B      =0xF0;

sfr  C1M6C  =0xF3; /* DS87C390 Extensions  */
sfr  C1M7C  =0xF4; /* DS87C390 Extensions  */
sfr  C1M8C  =0xF5; /* DS87C390 Extensions  */
sfr  C1M9C  =0xF6; /* DS87C390 Extensions  */
sfr  C1M10C =0xF7; /* DS87C390 Extensions  */

sfr  EIP    =0xF8; /* DS87C390 Extensions  */
sbit CANBIP =EIP^7;
sbit C0IP   =EIP^6;
sbit C1IP   =EIP^5;
sbit PWD1   =EIP^4;
sbit PX5    =EIP^3; /* Alternate PC3 */
sbit PX4    =EIP^2; /* Alternate PC2 */
sbit PX3    =EIP^1; /* Alternate PC1 */
sbit PX2    =EIP^0; /* Alternate PC0 */

sfr  C1M11C =0xFB; /* DS87C390 Extensions  */
sfr  C1M12C =0xFC; /* DS87C390 Extensions  */
sfr  C1M13C =0xFD; /* DS87C390 Extensions  */
sfr  C1M14C =0xFE; /* DS87C390 Extensions  */
sfr  C1M15C =0xFF; /* DS87C390 Extensions  */

                       /* Priority  */
#define  iInt0      0  /*  1 0003 Ext. Int. 0    IE0   EX0    */
#define  iTimer0    1  /*  2 000B Timer 0        TF0   ET0    */
#define  iInt1      2  /*  3 0013 Ext. Int. 1    TX1   EX1    */
#define  iTimer1    3  /*  4 001B Timer 1        TF1   ET1    */
#define  iSerial0   4  /*  5 0023 Serial Port 0  R-TI0 ES0    */
#define  iTimer2    5  /*  6 002B Timer 2        ET2   ET2    */
#define  iPower     6  /*  0 0033 Power Fail     PFI   EPFI   */
#define  iSerial1   7  /*  7 003B Serial Port 1  R-TI1 ES1    */
#define  iInt2      8  /*  8 0043 Ext. Int. 2    IE2   EX2    */
#define  iInt3      9  /*  9 004B Ext. Int. 3    IE3   EX3    */
#define  iInt4     10  /* 10 0053 Ext. Int. 4    IE4   EX4    */
#define  iInt5     11  /* 11 005B Ext. Int. 5    IE5   EX5    */
#define  iWatchDog 12  /* 14 0063 WDT Out Int.   EWIF  EWDI   */
#define  iCAN0     13  /* 12 006B CAN Int. 0     var   C0IE   */
#define  iCAN1     14  /* 13 0073 CAN Int. 1     var   C1IE   */
#define  iCANBUS   15  /* 15 007B CAN BUS int.   var   CANBIE */

#endif