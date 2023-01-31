/*************************************************************************/
/*  Program testowy dla obiektu A oprogamowania SPSym                    */
/*************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Stadnardowe I/O

// --- Makrodefinicje dla obiektu A ---
#define x1 BI[0].b.x_1		// X1 (BI[0].d&0x01)
#define x2 BI[0].b.x_2		// X2 (BI[0].d&0x02)
#define x3 BI[0].b.x_3		// X3 (BI[0].d&0x04)
#define T  X8

#define Z1 Y1
#define Z2 Y2
#define Z3 Y3
#define Z4 Y4
#define Z5 Y5
#define M  Y6
#define G1 Y7

// --- Zmienne użytkownika ---
int licz=0;                	// Licznik przerwań

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie LED-ów
    L5=L6=L7=L8=0;         	// Zgaszenie LED-ów
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}

void oblicz(void)			// Kod użytkownika wykonywany cyklicznie
{ 
    L1=aK1; L2=aK2; L3=aK3; L4=aK4;
    L5=aK5; L6=aK6; L7=aK7; L8=aK8;
    
    if(aK1) Z1 = 1; else Z1 = 0;
    if(aK2) Z2 = 1; else Z2 = 0;
    if(aK3) Z3 = 1; else Z3 = 0;
    if(aK4) G1 = 1; else G1 = 0;
    if(aK5) M  = 1; else M  = 0;

    if(licz >= 12000) licz=0;

    AO[0] = 800; 	// Ograniczenie przepustowości zaworu Z3 (800/10 = 80%)
    AO[1] = 350;	// Temperatura aktywacji czujnika T (350/10 = 35 stC)
    AO[2] += 300; AO[3] += 400;
    AO[4] += 500; AO[5] += 600;
    AO[6] += 700; AO[7] = licz/100;

	sprintf(buf,"K1=%dK2=%dK3=%dK4=%d    ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
	LCD_xy(1,1); LCD_puts(buf);
	sprintf(buf,"L1=%dL2=%dL3=%dL4=%d    ",(int)L1,(int)L2,(int)L3,(int)L4);
	LCD_xy(1,2); LCD_puts(buf);
}

void przerwanie(void)      	// Obsługs przerwania od układu czasowo-licznikowego
{
    licz++;
}

void komunikacja(void)		// Obsługa komunikacji szeregowej
{}

#ifdef Symulator_PC
void wykres(void)			// Dane do tabeli i wykresu (dot. symulacji obiektu)
{
    aTab[0] = (int)(AI[0]/10.0);
    aTab[1] = (int)(AI[1]/5.0);
    aTab[2] = AI[7];
    aTab[3] = (int)(licz/100.0);
    aTab[4] = AO[0];
    aTab[5] = AO[7];
    aTab[6] = -9;
    aTab[7] = 109;

    bTab[0] = (int)aK1;
    bTab[1] = (int)aK2;
    bTab[2] = (int)L1;
    bTab[3] = (int)L2;
    bTab[4] = X3;			// BI[0].d&0x04;
    bTab[5] = X2;			// BI[0].d&0x02;
    bTab[6] = X1;			// BI[0].d&0x01;
    bTab[7] = T;			// BI[0].d&0x80;
    bTab[8] = Z1;
    bTab[9] = Z2;
    bTab[10] = Z3;
    bTab[11] = G1;
    bTab[12] = 12;
    bTab[13] = 13;
    bTab[14] = 14;
    bTab[15] = 15;
}
#endif
