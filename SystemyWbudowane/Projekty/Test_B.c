/*************************************************************************/
/*  Program testowy dla obiektu A oprogamowania SPSym                    */
/*************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Stadnardowe I/O

// --- Makrodefinicje dla obiektu B ---
#define x1 	BI[0].b.x_1	// X1
#define x2 	BI[0].b.x_2	// X2
#define x3 	BI[0].b.x_3	// X3
#define x4 	BI[0].b.x_4	// X4
#define x5 	BI[0].b.x_5	// X5
#define x6 	BI[0].b.x_6	// X6
#define x7 	BI[0].b.x_7	// X7
#define T1  X8
#define T2  X9
#define T3  X10

#define Z1 Y1
#define Z2 Y2
#define Z3 Y3
#define Z4 Y4
#define Z5 Y5
#define M  Y6
#define G1 Y7
#define G2 Y8
#define G3 Y9
#define G4 Y10

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
    
    if(!aK8)
    {
    	if(aK1 != pK1) Z1 = !Z1;
    	if(aK2 != pK2) Z2 = !Z2;
    	if(aK3 != pK3) Z3 = !Z3;
    	if(aK4 != pK4) Z4 = !Z4;
    	if(aK5 != pK5) Z5 = !Z5;
    	if(aK6 != pK6) M  = !M;
    }
    else
    {
    	if(aK1 != pK1) G1 = !G1;
    	if(aK2 != pK2) G2 = !G2;
    	if(aK3 != pK3) G3 = !G3;
    	if(aK4 != pK4) G4 = !G4;
    }

    if(licz >= 12000) licz=0;

    AO[0] += 100; AO[1] += 200;
    AO[2] += 300; AO[3] += 400;
    AO[4] += 500; AO[5] += 600;
    AO[6] += 700; AO[7] = licz/100;

	sprintf(buf,"K1=%dK2=%dK3=%dK4=%d    ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
	LCD_xy(1,1); LCD_puts(buf);
	sprintf(buf,"L1=%dL2=%dL3=%dL4=%d    ",(int)L1,(int)L2,(int)L3,(int)L4);
	LCD_xy(1,2); LCD_puts(buf);
	
	pK1 = aK1; pK2 = aK2; pK3 = aK3; pK4 = aK4;
	pK5 = aK5; pK6 = aK6; pK7 = aK7; pK8 = aK8;
}

void przerwanie(void)      	// Obsługa przerwania od układu czasowo-licznikowego
{
    licz++;
}

void komunikacja(void)		// Obsługa komunikacji szeregowej
{}

#ifdef Symulator_PC
void wykres(void)			// Dane do tabeli i wykresu (dot. symulacji obiektu)
{
    aTab[0] = (int)(AI[0]/10.0);
    aTab[1] = (int)(AI[1]/10.0);
    aTab[2] = (int)(AI[2]/10.0);
    aTab[3] = (int)(AI[3]/10.0);
    aTab[4] = (int)(AI[4]/10.0);
    aTab[5] = (int)(AI[5]/10.0);
    aTab[6] = (int)(licz/100.0);
    aTab[7] = AO[7];

    bTab[0] = (int)aK1;
    bTab[1] = (int)aK2;
    bTab[2] = (int)L1;
    bTab[3] = (int)L2;
    bTab[4] = BI[0].d&0x04;
    bTab[5] = BI[0].d&0x02;
    bTab[6] = BI[0].d&0x01;
    bTab[7] = BI[0].d&0x80;
    bTab[8] = Z3;
    bTab[9] = G1;
    bTab[10] = M;
    bTab[11] = 11;
    bTab[12] = 12;
    bTab[13] = 13;
    bTab[14] = 14;
    bTab[15] = 15;
}
#endif
