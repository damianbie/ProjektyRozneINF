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
char stan1=1, stan2=1, tim2, tim3, stan3=1;
char mode = 2, manVal = 1;
char modeType = 1, modeState;

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie LED-ów
    L5=L6=L7=L8=0;         	// Zgaszenie LED-ów
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}

void oblicz(void)			// Kod użytkownika wykonywany cyklicznie
{ 
	switch(mode)
	{
		case 1:
			L1 = 1; L2 = 1;
			// zmiana trybu naS man
			if(!pK1 && aK1) 
			{
				mode = 2;
				Z1 = Z2 = Z3 = Z4 = Z5 = 0;
				G1 = G2 = 0; G3 = 0; G4 = 0;
				M = 0;
				break;
			}
			
			sprintf(buf,"Tryb auto.              ");
			LCD_xy(1,1); LCD_puts(buf);
			sprintf(buf,"                         ");
			
			// zbiornik1 
			switch(stan1)
			{
				case 1: 
					Z1 = 1; G1 = 0;
					if(x2) stan1 = 2;
				break;
				case 2:
					Z1 = 0; G1 = 1;
					if(T1) stan1 = 3;
				break;
				case 3: 
					Z1 = 0; G1 = 0;
					if(!T1) stan1 = 2;
					else if(!x1 || (stan3 == 5 && !x2)) stan1 = 1;
			}
			// zbiornik 2
			switch(stan2)
			{
				case 1:
					Z3 = 1; G2 = 0;
					if(x3) stan2 = 2;
				break;
				case 2: 
					Z3 = 1; G2 = 1;
					if(x4 && !T2) stan2 = 3;
					else if (x4 && T2) stan2 = 4;
				break;
				case 3: 
					Z3 = 0; G2 = 1;
					if(T2) stan2 = 4;
				break;
				case 4:
					Z3 = 0; G2 = 0;
					if(!T2) stan2 = 5;
					else if(!x3) stan2 = 1;
				break;
				case 5:
					Z3 = 0; G2 = 1;
					if(T2) {tim2 = 20; stan2 = 6;}
				break;
				case 6:
					Z3 = 0; G2 = 1;
					if(!tim2 && 1) stan2 = 4;
				break;
			}
			if(tim2) tim2--;
			
			// zbiornik 3
			switch(stan3)
			{
				case 1:
					sprintf(buf,"Czekanie na zbiorn.   ");
					Z2 = 0; Z4 = 0; Z5 = 0; G3 = 0; G4 = 0; M = 0;
					if(stan1 == 3 && stan2 == 4) stan3 = 2;
				break;
				case 2:
					sprintf(buf,"Nalewanie koncen.      ");
					Z2 = 1; Z4 = 0; Z5 = 0; G3 = 0; G4 = 0; M = 0;
					if(x5)
					{
						stan3 = 3;
						tim3 = 40;
					}
				break;
				case 3:
					sprintf(buf,"Mieszanie skl.         ");
					Z2 = 0; Z4 = 1; Z5 = 0; G3 = 1; G4 = 1; M = 1;
					if(!tim3)
					{
						stan3 = 4;
						tim3 = 20;
					}
					else if(x6) stan3 = 5;
				break;
				case 4:
					sprintf(buf,"Mieszanie skl.        ");
					Z2 = 1; Z4 = 0; Z5 = 0; G3 = 1; G4 = 1; M = 1;
					if(!tim3)
					{
						stan3 = 3;
						tim3 = 40;
					}
					if(x6) stan3 = 5;
				break;
				case 5:
					sprintf(buf,"Dolewanie wody        ");
					Z2 = 0; Z4 = 1; Z5 = 0; G3 = 1; G4 = 1; M = 1;
					if(x7 && !T3) stan3 = 6;
					else if(x7 && T3)
					{
						stan3 = 7;
						tim3 = 40;
					}
				break;
				case 6:
					sprintf(buf,"Dogrzewanie           ");
					Z2 = 0; Z4 = 0; Z5 = 0; G3 = 1; G4 = 0; M = 1;
					if(T3)
					{
						stan3 = 7;
						tim3 = 40;
					}
				break;
				case 7:
					sprintf(buf,"Nalewanie do but.       ");
					Z2 = 0; Z4 = 0; Z5 = 1; G3 = 0; G4 = 0; M = 1;
					if(!tim3) 
					{
						stan3 = 8;
						tim3 = 50;
					}
					else if(!x5) stan3 = 1;
				break;
				case 8:
					sprintf(buf,"Czekanie na but.     ");
					Z2 = 0; Z4 = 0; Z5 = 0; G3 = 0; G4 = 0; M = 1;
					if(!tim3) 
					{
						stan3 = 7;
						tim3 = 40;
					}
					else if(!x5) stan3 = 1;
					break;
			}
			if(tim3) tim3--;
			
			LCD_xy(1,2); LCD_puts(buf);
			break;
		case 2:  // tryb reczny 
			L1 = 1; L2 = 0;
			sprintf(buf,"Tryb manualny        ");
			LCD_xy(1,1); LCD_puts(buf);
			if(!pK1 && aK1) // zmiana na tryb automatyczny
			{ 
				mode 		= 1;  
				modeType 	= 1;
				modeState 	= 0;
				stan1 		= 1;
				stan2 		= 1;
				stan3 		= 1;
				break;
			}
			
			if(!pK2 && aK2) modeType--;
			if(modeType <= 0) modeType = 1;
			
			if(!pK3 && aK3) modeType++;
			if(modeType > 10) modeType = 10;
			
			modeState = aK4;
			L4 = modeState;
			switch(modeType)
			{
				case 1: 	
					Z1 = modeState;
					sprintf(buf,"Z1 - %d              ", modeState);
				break;
				case 2:
					if (X1)
					{
						G1 = modeState;
						sprintf(buf,"G1 - %d                ", G1);	
					}
					else 
						sprintf(buf,"G1 - %d Brak plynu     ", G1);
				break;
				case 3:
					Z3 = modeState;
					sprintf(buf,"Z3 - %d                ", modeState);
				break;
				case 4: 
					if(X3)
					{
						G2 = modeState;
						sprintf(buf,"G2 - %d              ", G2);
					}
					else 
						sprintf(buf,"G2 - %d Brak plynu  ", G2);
				break;
				case 5: 
					Z2 = modeState;
					sprintf(buf,"Z2 - %d                ", modeState);
				break;
				case 6: 
					Z4 = modeState;
					sprintf(buf,"Z4 - %d                ", modeState);
				break;
				case 7: 
					M = modeState;
					sprintf(buf,"M - %d                ", modeState);
				break;
				case 8: 
					if(X5) 
					{
						G3 = modeState;
						sprintf(buf,"G3 - %d                 ", G3);
					}
					else 
					 	sprintf(buf,"G3 - %d Brak plynu      ", G3);
				break;
				case 9: 
					if(X5) 
					{
						G4 = modeState;
						sprintf(buf,"G4 - %d               ", G4);
					}
					else 
						sprintf(buf,"G4 - %d Brak plynu     ", G4);
				break;
				case 10: 
					Z5 = modeState;
					sprintf(buf,"Z5 - %d                ", modeState);
				break;
				default:
					sprintf(buf,"ERROR                  ", modeState);
			}
			LCD_xy(1,2); LCD_puts(buf);
			break;
	}

    AO[0] += 1; AO[1] += 2;
    AO[2] += 3; AO[3] += 4;
    AO[4] += 5; AO[5] += 6;
    AO[6] += 7; AO[7] += 7;
	
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
