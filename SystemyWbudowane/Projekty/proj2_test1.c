/*************************************************************************/
/*  Program testowy dla obiektu X oprogamowania SPSym                    */
/*************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Stadnardowe I/O

// --- Makrodefinicje dla obiektu X ---
#define T  X8

#define TF Y1
#define BU Y2
#define Z3 Y3
#define Z4 Y4
#define YY5 Y5
#define YY6 Y6
#define PWM Y7

#define TE1 X5
#define TE2 X6

// --- Zmienne użytkownika ---
const float STEP = 50;

// obsluga zmiany trybow
int mode = 0, modeCh = 0; 
int modeTim=0;


// obsluga auto====
#define T1_MIN 400
#define T2_MIN 300
#define T3_MIN 800

int T1 = T1_MIN;
int T2 = T2_MIN;
int T3 = T3_MIN;

static int stanT1 = 1, timT1 = 0, liczT1 = 0;
static int stanT2 = 1, timT2 = 0, liczT2 = 0;
static int stanT3 = 1, timT3 = 0, liczT3 = 0;
// obsluga wyjscia ==

// obsluga man
int licz=0;                	// Licznik przerwań

float temp;					// Temperatura wody (mierzona)
float poziom;				// Poziom wody (mierzony)

void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1=L2=L3=L4=0;         	// Zgaszenie LED-ów
    L5=L6=L7=L8=0;         	// Zgaszenie LED-ów
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}
void zgrzewanie(void)
{
	const int Tkl1 = 1000;
	const int TB = 200;
	
	
	static int stanZgrz = 1;
	static int timZgrz = 0;
	
	
	char KL = X1;
	char WY = 0, BUZZ = 0;
	switch(stanZgrz)
	{
		case 1:
			WY = 0; BUZZ = 0;
			if(KL) {timZgrz = Tkl1 / STEP; stanZgrz = 2;}
		break;
		case 2:
			WY = 0; BUZZ = 0;
			if(!KL) stanZgrz = 1;
			else if(!timZgrz) {timZgrz = TB / STEP; stanZgrz = 3;}
		break;
		case 3:
			WY = 1; BUZZ = 1;
			if(!KL) stanZgrz = 1;
			else if(!timZgrz) {timZgrz = (T1 - TB) / STEP; stanZgrz = 4;}
		break;
		case 4:
			WY = 1; BUZZ = 0;
			if(!KL) stanZgrz = 1;
			else if(!timZgrz) {timZgrz = T2 / STEP; stanZgrz = 5;}
		break;
		case 5:
			WY = 0; BUZZ = 0;
			if(!KL) stanZgrz = 1;
			else if(!timZgrz) {timZgrz = (T3-TB) / STEP; stanZgrz = 6;}
		break;
		case 6:
			WY = 1; BUZZ = 0;
			if(!KL) stanZgrz = 1;
			else if(!timZgrz) {timZgrz = TB / STEP; stanZgrz = 7;}
		break;
		case 7:
			WY = 1; BUZZ = 1;
			if(!KL) stanZgrz = 1;
			else if(!timZgrz) { stanZgrz = 8;}
		break;
		case 8:
			WY = 0; BUZZ = 0;
			if(!KL) stanZgrz = 1;
		break;
	}
	if(timZgrz) --timZgrz;
	
	TF = WY; BU = BUZZ;
}
void mruganieDiodami()
{
	static int stanDiody = 1, timDiody = 0;
	int KL = X1;
	switch(stanDiody)
	{
		case 1: L2 = 0;
			if(X1) {stanDiody = 2; timDiody = 600 / STEP;}
		break;
		case 2:L2 = 1;
			if(!X1) {stanDiody = 1;}
			else if(!timDiody) {stanDiody = 3; timDiody = 600 / STEP;}
		break;
		case 3:L2 = 0;
			if(!X1) {stanDiody = 1;}
			else if(!timDiody) {stanDiody = 2; timDiody = 600 / STEP;}
		break;
	}
	if(timDiody) --timDiody;
}
void kontrolaWent(void)
{
	const short PWM_T1 = 200, PWM_T2 = 800,
				PWM_T3 = 800, PWM_T4 = 200;

	char oPWM = 0;
	static int timWent = 0, stanWent = 1;
	
	switch(stanWent)
	{
		case 1: oPWM = 0; // temp za niska 
			if(TE1) {stanWent = 2; timWent = PWM_T1 / STEP;}
		break;
		
		 // pwm niskie 
		case 2: oPWM = 1;
			if(!TE1) stanWent = 1;
			else if(!timWent) {stanWent = 3; timWent = PWM_T2 / STEP;}
		break;
		case 3: oPWM = 0;
			if(!TE1) stanWent = 1;
			else if(TE2) {stanWent = 4; timWent = PWM_T3 / STEP;}
			else if(!timWent) {stanWent = 2; timWent = PWM_T1 / STEP;}
		break;
		
		// pwm szybkie 
		case 4: oPWM = 1;
			if(!TE2) {stanWent = 3; timWent = PWM_T2 / STEP;}
			else if(!timWent) {stanWent = 5; timWent = PWM_T4 / STEP;}
		break;
		case 5: oPWM = 0;
			if(!TE2) {stanWent = 2; timWent = PWM_T1 / STEP;} 
			else if(!timWent) {stanWent = 4; timWent = PWM_T3 / STEP;}
		break;
	}
	
	if(timWent) --timWent;
	PWM = oPWM;
}
void zmianaTrybu(void)
{
	switch(modeCh)
    {
    	case 0:
    		if(aK1) 
    		{
    			modeTim = 1000 / STEP;
    			modeCh = 1;
    		}
    	break;
    	case 1:
    		if(!modeTim)
    		{
    			mode = !mode;
    			modeCh = 2;
    		}
    		else if (!aK1)
    		{
    			modeCh = 0;
    		}
    	break;
    	case 2:
    		if (!aK1)
    		{
    			modeCh = 0;
    		}
    	break;
    }
}
void klawisze(char kl, int* stan, int* tim, int* licz)
{
	switch(*stan)
	{
		case 1:
			if(kl) {*stan = 2; (*licz) = (*licz) +1;}
		break;
		case 2:
			if(!kl) {*stan = 3; *tim = 1000 / STEP;}
		break;
		case 3:
			if(kl) {*stan = 2; (*licz) = *licz + 1;}
			else if(!(*tim)) {*stan = 1; *licz = 0;}
		break;
	}
	if(*tim) (*tim) = *tim - 1;
}
void oblicz(void)			// Kod użytkownika wykonywany cyklicznie
{ 
	// Obsługa przetwornika ADC (analogowo-cyfrowego) 16-bitowego
	temp = AI[0]/10.0;		// Temperatura wody ([0, 1000]=>[0.0, 100.0]
	poziom = AI[1]/10.0;	// Poziom wody ([0, 1000]=>[0.0, 100.0])
	
    //tryb pracy 
    switch(mode)
    {
    	// auto
    	case 0:
    		L1 = 0;
    		//TF = 0;
    		//PWM = 0;
    		zmianaTrybu();
    		zgrzewanie();
    		kontrolaWent();
    		
    		klawisze(aK3, &stanT1, &timT1, &liczT1);
    		klawisze(aK4, &stanT2, &timT2, &liczT2);
    		klawisze(aK5, &stanT3, &timT3, &liczT3);
    		
    		if(liczT1 && liczT1 < 11) T1 = T1_MIN + liczT1 *50;
    		if(liczT2 && liczT2 < 11) T2 = T2_MIN + liczT2 * 50;
    		if(liczT3 && liczT3 < 11) T3 = T3_MIN + liczT3 * 50;
    		
    		//          "                    "
    		sprintf(buf,"[T1] [T2] [T3][AUTO]", X1);
			LCD_xy(1,1); LCD_puts(buf);
			//          "                     "
			sprintf(buf,"%i=%i=%i    [ms]  ", T1, T2, T3);
			LCD_xy(1,2); LCD_puts(buf);
    	break;
    	case 1:
    		L1 = 1;
    		TF = X1;
    		PWM = 1;
    		mruganieDiodami();
    	
    		//          "                    "
    		sprintf(buf,"             ![MAN]!", X1);
			LCD_xy(1,1); LCD_puts(buf);
			
			if(TE1 && TE2) sprintf(buf,"TEMP: WARN   F: 100%");
			else if(TE2) sprintf(buf,"TEMP: OK     F: 100%");
			else sprintf(buf,"TEMP: OK     F: 100%");
			
			LCD_xy(1,2); LCD_puts(buf);
    		zmianaTrybu();	
    	break;
    }

	if(modeTim) --modeTim;
	
	
	
	// Obsługa diod LED i wyświetlacza LCD
    //L1=Z1; L2=Z2; L3=Z3; L4=G1;		// Sygnalizacja stanu wyjść
    //L5=aK5; L6=aK6; L7=aK7; L8=aK8;    
    
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
    aTab[2] = 0;					// Odmierzany czas (licznik cykli obl.)
    aTab[3] = 0.0;	//(int)(licz/100.0);
    aTab[4] = (int)(AO[0]/10.0);
    aTab[5] = (int)(AO[1]/10.0);	// T_on [stC]
    aTab[6] = -120;
    aTab[7] =  120;

    bTab[0] = (int)X1;
    bTab[1] = (int)TF;
    bTab[2] = (int)BU;
    bTab[3] = (int)PWM;
    bTab[4] = aK1;			// BI[0].d&0x04;
    bTab[5] = aK2;			// BI[0].d&0x02;
    bTab[6] = aK3;			// BI[0].d&0x01;
    bTab[7] = aK4;			// BI[0].d&0x80;
    bTab[8] = aK5;
    bTab[9] = L1;
    bTab[10] = L2;
    bTab[11] = L3;
    bTab[12] = 0;
    bTab[13] = 0;
    bTab[14] = 0;
    bTab[15] = 0;
}
#endif
