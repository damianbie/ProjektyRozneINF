/*************************************************************************/
/*  Obiekt A (zad.5). Zaprojektuj układ sterowania według algorytmu:	 */
/*  h < x1 - nalewamy z obu Z1+Z2 do x1									 */
/*  h >=x1 - dalej nalewamy tylko z Z1 do x2, ale nie dłużej niż 5s	 	 */
/*  	   - zamykamy Z1, wylewamy z Z3 do x1 w cyklu 3+3s.				 */
/*  	   - dodatkowo, w każdej chwili może być ręcznie otwarty zawór W */
/*  	     (zakłócenie - woda może się szybciej wylewać niż nalewac)	 */
/*************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Stadnardowe I/O

// --- Makrodefinicje dla obiektu A ---
#define x1 X1 				// BI[0].d&0x01
#define x2 X2 				// BI[0].d&0x02
#define x3 X3 				// BI[0].d&0x04
#define T  X8

#define Z1 Y1
#define Z2 Y2
#define Z3 Y3
#define Z4 Y4
#define Z5 Y5
#define M  Y6
#define G1 Y7

// --- Zmienne użytkownika ---
char stan=1, tim; 			// definicje, cykl 0.1 s

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie LED-ów
    L5=L6=L7=L8=0;         	// Zgaszenie LED-ów
}

void oblicz(void)			// Kod użytkownika wykonywany cyklicznie
{ 
	switch(stan) {
		case 1: Z1=1; Z2=1; Z3=0;
			if (x1) { tim=50; stan=2; }
			break;
		case 2: Z1=1; Z2=0; Z3=0;
			if (!tim||x2) { tim=20; stan=3; } else if (!x1) stan=1;
			break;
		case 3: Z1=0; Z2=0; Z3=1;
			if (!tim) { tim=30; stan=4; } else if (!x1) stan=1;
			break;
		case 4: Z1=0; Z2=0; Z3=0;
			if (!tim) { tim=30; stan=3; } else if (!x1) stan=1;
			break;
	}
	if (tim) --tim; // dekrementuj timer co cykl
	L1=Z1; L2=Z2; L3=Z3; // zaswiec diody

    AO[0] = 800; 	// Ograniczenie przepustowości zaworu Z3 (800/10 = 80%)
    AO[1] = 350;	// Temperatura aktywacji czujnika T (350/10 = 35 stC)
    AO[2] += 3; AO[3] += 4;
    AO[4] += 5; AO[5] += 6;
    AO[6] += 7; AO[7] = 0;

	sprintf(buf,"K1=%dK2=%dK3=%dK4=%d    ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
	LCD_xy(1,1); LCD_puts(buf);
	sprintf(buf,"L1=%dL2=%dL3=%dL4=%d    ",(int)L1,(int)L2,(int)L3,(int)L4);
	LCD_xy(1,2); LCD_puts(buf);
}

void przerwanie(void)      	// Obsługs przerwania od układu czasowo-licznikowego
{}

void komunikacja(void)		// Obsługa komunikacji szeregowej
{}

#ifdef Symulator_PC
void wykres(void)			// Dane do tabeli i wykresu (dot. symulacji obiektu)
{
    aTab[0] = (int)(AI[0]/10.0);
    aTab[1] = (int)(AI[1]/10.0);
    aTab[2] = (int)(AI[2]/10.0);
    aTab[3] = (int)(AI[3]/10.0);
    aTab[4] = AO[0];
    aTab[5] = AO[1];
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
    bTab[12] = stan;
    bTab[13] = BI[0].d;
    bTab[14] = 14;
    bTab[15] = 15;
}
#endif
