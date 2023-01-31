/*************************************************************************/
/*  Program testowy dla obiektu X oprogamowania SPSym                    */
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

// --- Zmienne użytkownika ---
int licz=0;                	// Licznik przerwań
int stan=0;					// Stan automatu
int tim1=0;					// Odmierzanie czasu (liczba cykli obliczeniowych)
float T_on=30.0;			// Temperatura załączenia czujnika T (stC)
float temp;					// Temperatura wody (mierzona)
float poziom;				// Poziom wody (mierzony)

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie LED-ów
    L5=L6=L7=L8=0;         	// Zgaszenie LED-ów
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}

void oblicz(void)			// Kod użytkownika wykonywany cyklicznie
{ 
	// Obsługa przetwornika ADC (analogowo-cyfrowego) 16-bitowego
	temp = AI[0]/10.0;		// Temperatura wody ([0, 1000]=>[0.0, 100.0]
	poziom = AI[1]/10.0;	// Poziom wody ([0, 1000]=>[0.0, 100.0])
	
    // Automat stanów
    switch(stan)
    {
    	case 0:	// Stan początkowy/przejściowy 
    		Z1=Z2=Z3=G1=0;
    		tim1=30; stan=1;
    		break;
    	case 1: // Napełnianie przez Z1, maksymalnie 3s
    		Z1=1; Z2=0; Z3=0; G1=0;
    		if(X2) { tim1=100; stan=3; }
    		else if(!X2 && !tim1) { tim1=10; stan=2; }
    		break;
    	case 2:  // Napełnianie przez Z2, maksymalnie 1s
    		Z1=0; Z2=1; Z3=0; G1=0;
    		if(X2) { tim1=100; stan=3; }
    		else if(!X2 && !tim1) { tim1=30; stan=1; }
    		break;
    	case 3: // Grzanie do T, dokładnie 10s
    		Z1=0; Z2=0; Z3=0; G1=1;
    		if(!tim1 && T) { tim1=20; stan=5; }
    		else if(!tim1 && !T) { tim1=100; stan=4; }
    		break;
    	case 4: // Grzanie do T, maksymalnie 10s
    		Z1=0; Z2=0; Z3=0; G1=1;
    		if(T || !tim1) { tim1=20; stan=5; }
    		break;
    	case 5:	// Wylewanie przez Z3, maksymalnie 2s
    		Z1=0; Z2=0; Z3=1; G1=0;
    		if(!X1) { tim1=30; stan=1; }
    		else if(X1 && !tim1) { tim1=20; stan=6; }
    		break;
    	case 6:	// Przerwa w wylewaniu, maksymalnie 2s 
    		Z1=0; Z2=0; Z3=0; G1=0;
    		if(!X1) { tim1=30; stan=1; }
    		else if(X1 && !tim1) { tim1=20; stan=5; }
    		break;
    }
    if(tim1) --tim1;

    if(licz >= 12000) licz=0;

    // Temperatura załączenia czujnika T (tylko do symulacji)
    if(aK1) T_on += 0.1;
    if(aK2) T_on -= 0.1;
    if(T_on>99.0) T_on=99.0;
    if(T_on<1.0) T_on=1.0;
    
	// Obsługa przetwornika DAC (cyfrowo-analogowego) 16-bitowego
    AO[0] = (int)(10*80.0);			// Ograniczenie przepustowości zaworu Z3
    AO[1] = (int)(10*T_on);			// Temperatura aktywacji czujnika T
	
	// Obsługa diod LED i wyświetlacza LCD
    L1=Z1; L2=Z2; L3=Z3; L4=G1;		// Sygnalizacja stanu wyjść
    L5=aK5; L6=aK6; L7=aK7; L8=aK8;    
	sprintf(buf,"T_on=%3.1f (K1+/K2-)    ",T_on);
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
    aTab[0] = (int)(AI[0]/10.0);	// Temperatura (ADC: [0, 1000]=>[0.0, 100.0])
    aTab[1] = (int)(AI[1]/10.0);	// Poziom (ADC: [0, 1000]=>[0.0, 100.0])
    aTab[2] = tim1;					// Odmierzany czas (licznik cykli obl.)
    aTab[3] = 0.0;	//(int)(licz/100.0);
    aTab[4] = (int)(AO[0]/10.0);
    aTab[5] = (int)(AO[1]/10.0);	// T_on [stC]
    aTab[6] = -120;
    aTab[7] =  120;

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
    bTab[13] = tim1;
    bTab[14] = 0;
    bTab[15] = 0;
}
#endif
