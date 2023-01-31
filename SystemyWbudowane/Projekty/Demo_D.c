/*****************************************************************************/
/* Program demo dla obiektu D oprogamowania SPSym                            */
/* - naciśnięcie (i puszczenie) K1 otwiera bramę na czas przejazdu samochodu */
/* - naciśnięcie K1 przy otwieraniu wstrzymuje je, ale kolejne wznawia itd.  */
/* - naciśnięcie K2 przy otwieraniu wstrzymuje je, ale kolejne zamyka itd.   */
/* - naciśnięcie K2 przy zamykaniu wstrzymuje je, ale kolejne wznawia itd.   */
/* - naciśnięcie K1 przy zamykaniu wstrzymuje je, ale kolejne otwiera itd.   */
/* - przy częściowo otwartej/zamkniętej bramie K1 otwiera, a K2 zamyka ją    */
/* - L1 sygnalizuje otwarcie lub otwieranie bramy                            */
/* - L2 sygnalizuje zamknięcie lub zamykanie bramy                           */
/* - L5 sygnalizuje ruch bramy (lampa)                                       */
/* - L6 sygnalizuje alarm (uszkodzenie czujnika, timeout itp.)               */
/*****************************************************************************/
#include "spsym.h"     	// Pomocnicze funkcje i deklaracje
#include <stdio.h>      // Stadnardowe I/O

// --- Makrodefinicje dla obiektu D ---
#define cO  	X1		// Czujnik całkowitego otwarcia bramy
#define cZ  	X2		// Czujnik całkowitego zamknięcia bramy
#define cF  	X3		// Fotokomórka

#define SILNIK	Y1		// Silnik otwierający/zamykajacy bramę
#define KIER 	Y2		// Kierunek obrotów silnika (0=zamykanie, 1=otwieranie)
#define LAMPA 	Y3		// Lampa sygnalizująca ruch bramy
#define WJAZD 	Y4		// Zezwolenie na wjazd pojazdu (czerwone/zielone)

// --- Zmienne użytkownika ---
int tim1=0;		// Czas przestawiania bramy
int tim2=0;		// Czas impulsu sygnalizującego ruch bramy
int tim3=0;
char stan=0;	// Stan głównego automatu 
char stanALM=0;	// Stan automatu sygnalizacji alarmu
int timS1=0;
int alarm=0;	// Kod (numer) alarmu
int licz1;
int timA1;		// Czas impulsu przy sygnalizacji alarmu
char pcF;		// Poprzedni stan czujnika cF
int licz2;		// Licznik czasu otwartej bramy

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie L1..L4
    L5=L6=L7=L8=0;         	// Zgaszenie L5..L6
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;	// Poprzedni stan przycisków K1..K4
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;	// Poprzedni stan przycisków K5..K8
	pcF=cF;					// Poprzedni stan czujnika cF
}

void oblicz(void)			// Kod użytkownika wykonywany cyklicznie
{ 
	// K1 - przycisk otwierania bramy
	// K2 - przycisk zamykania bramy
	// L1 - zamykanie się (skracające się impulsy) lub zamknięcie (L1=1) bramy
	// L2 - otwieranie się (skracające się impulsy) lub otwarcia (L2=1) bramy
	// L3 - otwieranie bramy
	// L4 - zamykanie bramy
	// L5 - ruch bramy (żółta LAMPA)
	// L6 - alarm (przeszkoda, brak sygnału z czujnika itp.)
	
    switch(stan)
    {
    	case 0: // Stan początkowy (przejściowy)
    		L1=1; L2=1; L3=1; L4=1;			// Mrugnij diodami (test sprawności diod)
    		if(!cO&&cZ) stan=1;				// Brama zamknięta
    		else if(cO&&!cZ) stan=3;		// Brama otwarta
    		else stan=4;					// Zamknij bramę
    		break;
	    case 1: // Brama zamknięta (oczekiwanie na otwarcie)
	    	KIER=1; SILNIK=0; WJAZD=0;		// Silnik wyłączony, kierunek do otwarcia, brak zezwolenia na wjazd
	    	L1=1; L2=0; L3=0; L4=0; L5=0;	// Stan normalny
	    	if(aK1&&!pK1&&!cF) { tim1=40; tim2=tim1/10+1; timS1=3; KIER=1; SILNIK=1; stan=2; }	// Otwórz bramę
    		else if(aK2&&!pK2&&cF) alarm=1;	// Alarm: Przeszkoda! (przy zamkniętej bramie mało prawdopodobne)
    		else if(!cZ) alarm=2;			// Ostrzeżenie: Niedomknięta brama (uszkodzony czujnik lub ręczne otwarcie)
    		break;
    	case 2: // Otwieranie bramy
    		KIER=1; SILNIK=1; WJAZD=0;		// Stan wyjść binarnych (pre-action)
    		L1=0; L4=0; 
	    	if(cO) { SILNIK=0; LAMPA=0; WJAZD=1; licz2=0; stan=3; }	// Stop (brama otwarta)
	    	else if(aK2&&!pK2&&!cF) { tim1=40; tim2=(40-tim1)/10+1; KIER=0; SILNIK=0; licz2=0; stan=5; }	// Zamknij bramę
	    	else if(aK1&&!pK1) { licz2=0; stan=5; }		// Wstrzymaj otwieranie, bez opóźnienia, bo można wznowić natychmiast
    		else if(aK2&&!pK2&&cF) alarm=1;	// Alarm!: Przeszkoda! (przy otwieraniu tylko ostrzeżenie, bo nie powinno przeszkadzać)
	    	else 
	    	{	// Wewnętrzne akcje (bez opuszczania stanu), internal (conditional) post-action
    			if(tim1) --tim1;			// Odmierzanie czasu otwierania bramy
	    		else alarm=3;				// Alarm!: Przekroczony czas otwierania (timeout)
    			if(tim2) --tim2;			// Odmierzanie czasu pojedynczego impulsu/przerwy
    			else { L2=!L2; tim2=tim1/10+1; }	// Ustawienie nowego czasu
    			if(timS1) --timS1;			// Odmierzanie czasu impulsu/przerwy mrugającej LAMPY
    			else { timS1=3; L5=!L5; LAMPA=!LAMPA; }	// Ustawienie nowego czasu
    			//if(++licz2>80) alarm=4;		// Przekroczony czas otwierania bramy
			}
    		break;
	    case 3: // Brama otwarta (oczekiwanie na zamknięcie)
	    	KIER=0; SILNIK=0; WJAZD=1;		// Stan wyjść binarnych
	    	L1=0; L2=1; L3=0; L4=0; L5=0;	// Stan wyjść panelu
	    	if(aK2&&!pK2&&!cF) { tim1=40; tim2=tim1/10+1; timS1=3; KIER=0; SILNIK=1; WJAZD=0; stan=4; }	// Zamknij
	    	else if(!cF&&pcF) { tim1=40; tim2=tim1/10+1; timS1=3; KIER=0; SILNIK=1; WJAZD=0; stan=4; }	// Zamknij po zniknięciu pojazdu/przeszkody
    		else 
    		{
    			if(aK2&&!pK2&&cF) alarm=1;	// Alarm!: Przeszkoda
    			else if(!cO) alarm=2;		// Ostrzeżenie: brak całkowitego otwarcia (uszkodzony czujnik lub ręczne otwarcie)
    			if(cF) WJAZD=0;				// czerwone światło po aktywacji fotokomórki (na czas przejazdu, przed zamknięciem bramy)
    			//if(++licz2>80) alarm=4;	// Przekroczony czas otwartej bramy (do automatycznego zamknięcia)
    		}
    		break;
    	case 4: // Zamykanie bramy
    		KIER=0; SILNIK=1; WJAZD=0;		// Stan wyjść binarnych
    		L2=0; L3=0;
	    	if(cZ) { SILNIK=0; LAMPA=0; stan=1; }	// Stop (brama zamknięta)
	    	else if(cF) { alarm=1; stan=6; }		// Alarm!: Przeszkoda
	    	else if(aK1&&!pK1) { tim1=40; tim2=tim1/10+1; timS1=5; KIER=1; SILNIK=0; stan=6; }	// Otwórz bramę
	    	else if(aK2&&!pK2) stan=6;		// Wstrzymaj zamykanie, bez timera, bo można wznowić natychmiast
	    	else if(!tim1) alarm=3;			// Alarm!: Timeout
    		else if(!tim2) { L1=!L1; tim2=tim1/10+1; }
    		else if(!timS1) { timS1=3; L5=!L5; LAMPA=!LAMPA; }
    		if(timS1) --timS1;
    		if(tim1) --tim1;
    		if(tim2) --tim2;
    		break;
    	case 5: // Wstrzymane otwieranie (z ew. zmianą kierunku)
	    	KIER=1; SILNIK=0; WJAZD=0; LAMPA=0;	// Stan wyjść binarnych
	    	if(aK1&&!pK1) { KIER=1; SILNIK=1; timS1=3; stan=2; }	// Otwórz bramę bez zwłoki
	    	else if(aK2&&!pK2&&!tim3) { tim1=40; tim2=tim1/10+1; timS1=3; KIER=0; SILNIK=1; WJAZD=0; stan=4; }	// Zamknij po upływie zwłoki
    		else if(tim3) --tim3;				// Czas na zatrzymanie silnika (zwłoka czasowa)
    		break;
    	case 6: // Wstrzymane zamykanie (z ew. zmianą kierunku)
	    	KIER=0; SILNIK=0; WJAZD=0; LAMPA=0;	// Stan wyjść binarnych
	    	if(aK1&&!pK1&&!tim3) { tim1=40; tim2=tim1/10+1; timS1=3; KIER=1; SILNIK=1; stan=2; }	// Otwórz po upływie zwłoki
	    	else if(aK2&&!pK2) { KIER=0; SILNIK=1; WJAZD=0; timS1=3; stan=4; }	// Zamknij bramę bez zwłoki
    		else if(tim3) --tim3;				// Czas na zatrzymanie silnika (zwłoka czasowa)
    		break;
    	default: // Domyślny (nierozpoznany) stan => zamknij bramę
    		KIER=0; SILNIK=0; WJAZD=0; LAMPA=0;	// Stan wyjść binarnych
	    	L1=!L1; L2=!L2; L3=!L3; L4=!L4;		// mruganie diodami
	    	if(aK1&&!pK1) { tim1=40; tim2=tim1/10+1; timS1=3; KIER=1; SILNIK=1; stan=2; }	// Otwórz bramę
	    	if(aK2&&!pK2) { tim1=40; tim2=tim1/10+1; timS1=3; KIER=0; SILNIK=1; stan=4; }	// Zamknij bramę
    		break;
    }
    
    if(cO && cZ) alarm=5;			// Alarm: uszkodzony czujnik (cO lub cZ)!
    
    // --- Sygnalizacja alarmu ---
    // 1 imp. => przeszkoda
    // 2 imp. => brak całkowitego otwarcia lub zamknięcia bramy (wadliwe działanie czujnika)
    // 3 imp. => przekroczony czas otwierania/zamykania (timeout)
    // 4 imp. => Przekroczony czas otwartej bramy
    // 5 imp. => uszkodzony czujnik cO lub cZ
    switch(stanALM)
    {
    	case 0: // Brak alarmu
    		L6=0;			// OFF
    		if(alarm) {timA1=2; licz1=alarm-1; stanALM=1;}
    		break;
	    case 1: // Impuls
	    	L6=1;			// ON
	    	--timA1;
	    	if(!timA1 && licz1) { timA1=4; stanALM=2; }				// => krótka przerwa
    		else if(!timA1 && !licz1) { timA1=20; stanALM=2; }		// => długa przerwa
    		break;
	    case 2: // Przerwa
	    	L6=0;			// OFF
	    	--timA1;
	    	if(!timA1 && licz1) { timA1=2; --licz1; stanALM=1; }	// => kolejny impuls
    		else if(!timA1 && !licz1) {alarm=0; stanALM=0;}			// => koniec
    		break;
    }
    
    AO[0] = tim2*5;

	sprintf(buf,"K1=%dK2=%dK3=%dK4=%d    ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
	LCD_xy(1,1); LCD_puts(buf);
	sprintf(buf,"L1=%dL2=%dL3=%dL4=%d    ",(int)L1,(int)L2,(int)L3,(int)L4);
	LCD_xy(1,2); LCD_puts(buf);
		
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;	// stan poprzedni przycisków K1..K4
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;	// stan poprzedni przycisków K5..K8
	pcF=cF;								// stan poprzedni fotokomórki
}

void przerwanie(void)      	// Obsługs przerwania od układu czasowo-licznikowego
{}

void komunikacja(void)		// Obsługa komunikacji szeregowej
{}

#ifdef Symulator_PC
void wykres(void)			// Dane do tabeli i wykresu (dot. symulacji obiektu)
{
	// --- Wykresy wartości analogowych (float) ---
    aTab[0] = (int)(AI[0]/10.0);
    aTab[1] = (int)(AI[1]/5.0);
    aTab[2] = AI[7];
    aTab[3] = (int)(licz1/100.0);
    aTab[4] = AO[0];
    aTab[5] = AO[7];
    aTab[6] = -120;
    aTab[7] =  120;

	// --- Wykresy wartości binarnych (0/1) ---
    bTab[0] = (int)aK1;
    bTab[1] = (int)aK2;
    bTab[2] = (int)cO;
    bTab[3] = (int)cZ;
    bTab[4] = (int)cF;
    bTab[5] = (int)L1;
    bTab[6] = (int)L2;
    bTab[7] = (int)L3;
    bTab[8] = (int)L4;
    bTab[9] = (int)L5;		// LAMPA
    bTab[10] = (int)L6;
    bTab[11] = (int)LAMPA;
    
	// --- wartości liczbowe bez wykresu (int) ---
    bTab[12] = stan;
    bTab[13] = alarm;
    bTab[14] = 14;
    bTab[15] = 15;
}
#endif
