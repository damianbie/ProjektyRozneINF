/*************************************************************************/
/*  Program testowy bez obiektu w oprogamowaniu SPSym                    */
/*************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Stadnardowe I/O

// --- Makrodefinicje dla obiektu X ---
#define T  X8

#define Z1 Y1
#define Z2 Y2
#define Z3 Y3
#define Z4 Y4
#define Z5 Y5
#define M  Y6
#define G1 Y7

// --- Zmienne u¿ytkownika ---
int licz=0;                	// Licznik przerwañ

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie LED-ów
    L5=L6=L7=L8=0;         	// Zgaszenie LED-ów
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}

void oblicz(void)			// Kod u¿ytkownika wykonywany cyklicznie
{ 
    L1=aK1; L2=aK2; L3=aK3; L4=aK4;
    L5=aK5; L6=aK6; L7=aK7; L8=aK8;
    
    licz++;
    if(licz >= 100) licz=0;

	sprintf(buf,"K1=%dK2=%dK3=%dK4=%d    ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
	LCD_xy(1,1); LCD_puts(buf);
	sprintf(buf,"L1=%dL2=%dL3=%dL4=%d    ",(int)L1,(int)L2,(int)L3,(int)L4);
	LCD_xy(1,2); LCD_puts(buf);
}

void przerwanie(void)      	// Obs³ugs przerwania od uk³adu czasowo-licznikowego
{}

void komunikacja(void)		// Obs³uga komunikacji szeregowej
{}

#ifdef Symulator_PC
void wykres(void)			// Dane do tabeli i wykresu (dot. symulacji obiektu)
{
    aTab[0] = (int)(AI[0]/10.0);
    aTab[1] = (int)(AI[1]/5.0);
    aTab[2] = AI[7];
    aTab[3] = (int)(licz);
    aTab[4] = AO[0];
    aTab[5] = AO[7];
    aTab[6] = -120;
    aTab[7] =  120;

    bTab[0] = (int)aK1;
    bTab[1] = (int)aK2;
    bTab[2] = (int)aK3;
    bTab[3] = (int)aK4;
    bTab[4] = (int)aK5;
    bTab[5] = (int)aK6;
    bTab[6] = (int)L1;
    bTab[7] = (int)L2;
    bTab[8] = (int)L3;
    bTab[9] = (int)L4;
    bTab[10] = (int)L5;
    bTab[11] = (int)L6;
    bTab[12] = 12;
    bTab[13] = 13;
    bTab[14] = 14;
    bTab[15] = 15;
}
#endif
