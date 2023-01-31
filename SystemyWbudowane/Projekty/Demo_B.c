/******************************************************************************/
/* Obiekt B                    												  */
/* Zbiornik 1 (lewy górny) 													  */
/* h < X1 - nalewamy Z1 do X1 cały czas 									  */
/* h >=X1 - dalej nalewamy do X2 w cyklu 3 + 2 s 							  */
/* h >=X2 - zamykamy Z1 i czekamy na opróżnienie do X1, ale nie krócej niż 5s */
/* Zbiornik 2 (prawy górny) 												  */
/* h < X3 - nalewamy Z3 do X3 cały czas 									  */
/* h >=X3 - dalej nalewamy z Z3 do X4 w cyklu 3 + 2 s, ale nie dłużej niż 8s  */
/* h >=X4 - zamykamy Z3 i czekamy na opróżnienie do X43						  */
/* Zbiornik 3 (dolny) 														  */
/* h < X5 - nalewamy z obu Z2+Z4 do X5 (cały czas) 							  */
/* h >=X5 - dalej nalewamy do X7 na przemian Z2 i Z4 						  */
/*          w cyklu 3 + 3 s (3 z lewego, 3 z prawego, itd.) 				  */
/* h >=X7 - zamykamy Z2/Z4, włączamy grzałkę G3, 							  */
/*          aż temperatura osiągnie T3, ale nie dłużej niż 5 s 				  */
/*        - wyłączamy grzałkę, wylewamy Z5 do X5, ale nie krócej niż 5 s	  */
/******************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Stadnardowe I/O

// --- Makrodefinicje dla obiektu B ---
#define x1 	X1 				// BI[0].b.x_1
#define x2 	X2 				// BI[0].b.x_2
#define x3 	X3 				// BI[0].b.x_3
#define x4 	X4 				// BI[0].b.x_4
#define x5 	X5 				// BI[0].b.x_5
#define x6 	X6 				// BI[0].b.x_6
#define x7 	X7 				// BI[0].b.x_7
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
char stan1=1, tm1, stan2=1, tm2a, tm2b, stan3=1, tm3;

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie LED-ów
    L5=L6=L7=L8=0;         	// Zgaszenie LED-ów
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}

void oblicz(void)			// Kod użytkownika wykonywany cyklicznie
{ 
	switch(stan1) { // ------------------------------------- zbiornik 1
		case 1: Z1=1; if(X1){tm1=30; stan1=2;}
			break;
		case 2: Z1=1; if(X2){tm1=50; stan1=4;}
			else if(!tm1){tm1=20; stan1=3;}
			else if(!X1) stan1=1;
			break;
		case 3: Z1=0; if(!tm1){tm1=30; stan1=2;}
			else if(!X1) stan1=1;
			break;
		case 4: Z1=0; if(!tm1&&X1) stan1=5;
			else if(!tm1&&!X1)stan1=1;
			break;
		case 5: Z1=0; if(!X1) stan1=1;
			break;
	}
	if (tm1) --tm1;

	switch(stan2) { // ------------------------------------- zbiornik 2
		case 1: Z3=1; if(X3){tm2a=30; tm2b=80; stan2=2;}
			break;
		case 2: Z3=1; if(!tm2b||X4)stan2=4;
			else if(!tm2a){tm2a=20; stan2=3;}
			else if(!X3) stan2=1;
			break;
		case 3: Z3=0; if(!tm2a){tm2a=30; stan2=2;}
			else if(!tm2b) stan2=4;
			else if(!X3) stan2=1;
			break;
		case 4: Z3=0; if(!X3) stan2=1;
			break;
	}
	if (tm2a) --tm2a;
	if (tm2b) --tm2b;

	switch(stan3) { // ------------------------------------- zbiornik 3
		case 1: Z2=1; Z4=1; Z5=0; G3=0;
			if(X5){tm3=30; stan3=2;}
			break;
		case 2: Z2=1; Z4=0; Z5=0; G3=0;
			if(X7){tm3=50; stan3=4;}
			else if(!tm3){tm3=30; stan3=3;}
			break;
		case 3: Z2=0; Z4=1; Z5=0; G3=0;
			if(X7){tm3=50; stan3=4;}
			else if(!tm3){tm3=20; stan3=2;}
			break;
		case 4: Z2=0; Z4=0; Z5=0; G3=1;
			if(T3||!tm3){tm3=50; stan3=5;}
			break;
		case 5: Z2=0; Z4=0; Z5=1; G3=0;
			if(!tm3&&X5)stan3=6;
			else if(!tm3&&!X5)stan3=1;
			break;
		case 6: Z2=0; Z4=0; Z5=1; G3=0;
			if(!X5)stan3=1;
			break;
	}
	if (tm3) --tm3;
	L1=Z1; L2=Z2; L3=Z3; L4=Z4; L5=Z5; L6=G3; // zaświeć diody

    AO[0] += 1; AO[1] += 2;
    AO[2] += 3; AO[3] += 4;
    AO[4] += 5; AO[5] += 6;
    AO[6] += 7; AO[7] += 7;

	sprintf(buf,"K1=%dK2=%dK3=%dK4=%d    ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
	LCD_xy(1,1); LCD_puts(buf);
	sprintf(buf,"L1=%dL2=%dL3=%dL4=%d    ",(int)L1,(int)L2,(int)L3,(int)L4);
	LCD_xy(1,2); LCD_puts(buf);
	
	pK1 = aK1; pK2 = aK2; pK3 = aK3; pK4 = aK4;
	pK5 = aK5; pK6 = aK6; pK7 = aK7; pK8 = aK8;
}

void przerwanie(void)      	// Obsługa przerwania od układu czasowo-licznikowego
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
    aTab[4] = (int)(AI[4]/10.0);
    aTab[5] = (int)(AI[5]/10.0);
    aTab[6] = (int)(AI[6]/10.0);
    aTab[7] = AO[7];

    bTab[0] = X2;
    bTab[1] = X1;
    bTab[2] = Z1;
    bTab[3] = X4;
    bTab[4] = X3;
    bTab[5] = Z3;
    bTab[6] = X7;
    bTab[7] = X5;
    bTab[8] = T3;
    bTab[9] = Z2;
    bTab[10] = Z4;
    bTab[11] = Z5;
    bTab[12] = stan1;
    bTab[13] = stan2;
    bTab[14] = stan3;
    bTab[15] = 15;
}
#endif
