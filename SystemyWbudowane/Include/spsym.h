/****************************************************************************
 *        Pomocnicze deklaracje i definicje oprogramowania SPSym            *
 *                  Autor: Andrzej STEC, KIiA PRz                           *
 ****************************************************************************/

// ########## Wspólne dla wszystkich platform sprzêtowych ##########
char buf[64];              // Bufor wyœwietlacza LCD

#ifdef SP841

// ########## Tylko dla mikrokontrolerów rodziny 8051 (SP-841) ##########

bit X1, X2, X3, X4, X5, X6, X7, X8;
bit X9, X10, X11, X12, X13, X14, X15, X16;
bit Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8;
bit Y9, Y10, Y11, Y12, Y13, Y14, Y15, Y16;

#define FRAME_IN_MAX 40 			// Rozmiar bufora komunikatów
#define aWe_MAX 16					// Max iloœæ wejœæ analogowych
#define aWy_MAX 16					// Max iloœæ wyjœæ analogowych
bdata char bWe[2],bWy[2];			// Wejœcia i wyjœcia binarne obiektu
unsigned int aWe[aWe_MAX];		// Tablica wejœæ analogowych obiektu
unsigned int aWy[aWe_MAX];		// Tablica wyjœæ analogowych obiektu

#elif defined(SPAVR)

// ########## Tylko dla mikrokontrolerów AVR (SP-AVR, MegaAVR) ##########

#include <avr/io.h>					// Rejestry mikrokontrolera ATmega32
#include <avr/interrupt.h>			// Przerwania mikrokontrolera
#include <stdio.h>					// Standardowe I/O
#include "../Include/SPAVR/key.h"					// Obs³uga przycisków (SW0...SW7)
#include "../Include/SPAVR/led.h"					// Obs³uga diod LED (LD0...LD7)
#include "../Include/SPAVR/lcd.h"					// Obs³uga wyœwietlacza LCD 2x16 znaków
#include "../Include/SPAVR/rslib.h"					// Obs³uga komunikacji z Symulatorem Obiektów

//#include "SchematA.h"				// Oznaczenia obiektu A

#define	TCYKL	25					// Czas cyklu obliczeñ (25 x 4ms = 100ms)
extern void prolog(void);			// Inicjowanie zadania
extern void oblicz(void);			// Cykliczne obliczenia (zadanie)
extern void przerwanie(void);		// Przerwanie od uk³adu czasowo-liczn. T0

volatile unsigned char cykl;		// Odmierzanie czasu cyklu (pêtli g³ównej)
char buf[64];						// Bufor komunikatów LCD

int main(void)
{
	DDRA  = 0xff;					// Kierunek portu A (diody LED): port wyjœciowy
	DDRB  = 0x00;					// Kierunek portu B (klawisze) : port wejœciowy
	PORTA = 0xff;					// Port A (diody LED): ustaw "1" (wygaszone diody)
	PORTB = 0xff;					// Port B (klawisze) : aktywne rezystory podci¹gaj¹ce
	
	TCCR0 = _BV(WGM01)|_BV(CS02);	// Timer/Counter 0: tryb CTC, dzielnik=256
	OCR0  = 124;					// Limit zliczania T0: 0.125us x 256 x (124+1) = 4ms
	TIMSK = _BV(OCIE0);				// Odblokowanie przewañ T0 (Output Compare Match)
	sei();							// Globalne w³¹czenie obs³ugi przerwañ

	LCD_init();						// Inicjowanie wyœwietlacza LCD
	SYM_init(19200,0,0);			// Inicjowanie komunikacji z symulatorem
	prolog();						// Inicjowanie programu u¿ytkownika
	
	while(1)
	{
		cykl = TCYKL;				// Deklarowany czas cyklu (TCYKL x 4ms)
		//KEY_read();					// Odczyt stanu klawiszy aK1..aK4
		//-------------- Pocz¹tek kodu u¿ytkownika --------------------------------------
		oblicz();					// Obliczenia u¿ytkownika
		//-------------- Koniec kodu u¿ytkownika ----------------------------------------
		//LED_set();					// Ustawienie diod LED
		//KEY_mem();				// Zapamiêtanie stanu klawiszy w pK1..pK4
		//SYM_send();					// Przeœlij stan wyjœæ do obiektu
		//SYM_read();					// Odczytaj stan wejœæ z obiektu
		
		//if(!cykl) L4=!L4;			// Sygnalizacja przekroczenia czasu cyklu
		while(cykl);				// Oczekiwanie na koniec cyklu oblicz.
	}
	return 0; 
}

ISR(TIMER0_COMP_vect)				// Przerwanie od Timer/Counter 0 (co 4ms)
{
	if(cykl) --cykl;				// Odmierzanie czasu cyklu pêtli g³ównej
	przerwanie();
}

int bTab[16]= {0,0,0,0,0,0,0,0};	// Tablica wymiany wartoœci binarnych (16)
int aTab[8] = {0,0,0,0,0,0,0,0};	// Tablica wymiany wartoœci analogowych (8)
unsigned short AI[8];				// Wejœcia analogowe: 8
unsigned short AO[8];				// Wyjœcia analogowe: 8

#else

// ########## Tylko do symulacji na komputerze PC ##########

#define Symulator_PC
#define _CRT_SECURE_NO_DEPRECATE 

#define COMPILE_DLL
#ifdef COMPILE_DLL
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif

typedef unsigned char bit;
typedef unsigned char sbit;

int bTab[16]= {0,0,0,0,0,0,0,0};	// Tablica wymiany wartoœci binarnych (16)
int aTab[8] = {0,0,0,0,0,0,0,0};	// Tablica wymiany wartoœci analogowych (8)

// ----- Klawisze panelu -----
char aK1,aK2,aK3,aK4,aK5,aK6,aK7,aK8;	// Aktualny stan klawiszy
char pK1,pK2,pK3,pK4,pK5,pK6,pK7,pK8;	// Poprzedni stan klawiszy

union KeyUnion {
	unsigned short d;				// bajtowo
	struct
	{
		unsigned char k1  : 1;
		unsigned char k2  : 1;
		unsigned char k3  : 1;
		unsigned char k4  : 1;
		unsigned char k5  : 1;
		unsigned char k6  : 1;
		unsigned char k7  : 1;
		unsigned char k8  : 1;
		unsigned char k9  : 1;
		unsigned char k10 : 1;
		unsigned char k11 : 1;
		unsigned char k12 : 1;
		unsigned char k13 : 1;
		unsigned char k14 : 1;
		unsigned char k15 : 1;
		unsigned char k16 : 1;
	} b;							// bitowo
} Keys;								// Klawisze: 16

#define	keys	Keys.d
#define	K1		Keys.b.k1
#define	K2		Keys.b.k2
#define	K3		Keys.b.k3
#define	K4		Keys.b.k4
#define	K5		Keys.b.k5
#define	K6		Keys.b.k6
#define	K7		Keys.b.k7
#define	K8		Keys.b.k8
#define	K9		Keys.b.k9
#define	K10		Keys.b.k10
#define	K11		Keys.b.k11
#define	K12		Keys.b.k12
#define	K13		Keys.b.k13
#define	K14		Keys.b.k14
#define	K15		Keys.b.k15
#define	K16		Keys.b.k16

// ----- Wejœcia binarne -----
union bInpUnion {
	unsigned short d;				// bajtowo
	struct
	{
		unsigned char x_1 : 1;
		unsigned char x_2 : 1;
		unsigned char x_3 : 1;
		unsigned char x_4 : 1;
		unsigned char x_5 : 1;
		unsigned char x_6 : 1;
		unsigned char x_7 : 1;
		unsigned char x_8 : 1;
		unsigned char x_9 : 1;
		unsigned char x10: 1;
		unsigned char x11: 1;
		unsigned char x12: 1;
		unsigned char x13: 1;
		unsigned char x14: 1;
		unsigned char x15: 1;
		unsigned char x16: 1;
	} b;							// bitowo
} BI[2];							// Wejœcia binarne: 2x16 = 32

#define	binp0	BI[0].d
#define	binp1	BI[1].d
#define	X1		BI[0].b.x_1
#define	X2		BI[0].b.x_2
#define	X3		BI[0].b.x_3
#define	X4		BI[0].b.x_4
#define	X5		BI[0].b.x_5
#define	X6		BI[0].b.x_6
#define	X7		BI[0].b.x_7
#define	X8		BI[0].b.x_8
#define	X9		BI[0].b.x_9
#define	X10		BI[0].b.x10
#define	X11		BI[0].b.x11
#define	X12		BI[0].b.x12
#define	X13		BI[0].b.x13
#define	X14		BI[0].b.x14
#define	X15		BI[0].b.x15
#define	X16		BI[0].b.x16

// ----- Diody LED -----
union LedUnion {
	unsigned short d;				// bajtowo
	struct
	{
		unsigned char l1 : 1;
		unsigned char l2 : 1;
		unsigned char l3 : 1;
		unsigned char l4 : 1;
		unsigned char l5 : 1;
		unsigned char l6 : 1;
		unsigned char l7 : 1;
		unsigned char l8 : 1;
	} b;							// bitowo
} Leds;								// Diody LED: 8 (max 16)

#define	leds	Leds.d
#define	L1		Leds.b.l1
#define	L2		Leds.b.l2
#define	L3		Leds.b.l3
#define	L4		Leds.b.l4
#define	L5		Leds.b.l5
#define	L6		Leds.b.l6
#define	L7		Leds.b.l7
#define	L8		Leds.b.l8

// --- Wyjœcia binarne ---
union bOutUnion {
	unsigned short d;				// bajtowo
	struct
	{
		unsigned char y1 : 1;
		unsigned char y2 : 1;
		unsigned char y3 : 1;
		unsigned char y4 : 1;
		unsigned char y5 : 1;
		unsigned char y6 : 1;
		unsigned char y7 : 1;
		unsigned char y8 : 1;
		unsigned char y9 : 1;
		unsigned char y10: 1;
		unsigned char y11: 1;
		unsigned char y12: 1;
		unsigned char y13: 1;
		unsigned char y14: 1;
		unsigned char y15: 1;
		unsigned char y16: 1;
	} b;							// bitowo
} BO[2];							// Wyjœcia binarne: 2x16 = 32

#define	bout0	BO[0].d
#define	bout1	BO[1].d
#define	Y1		BO[0].b.y1
#define	Y2		BO[0].b.y2
#define	Y3		BO[0].b.y3
#define	Y4		BO[0].b.y4
#define	Y5		BO[0].b.y5
#define	Y6		BO[0].b.y6
#define	Y7		BO[0].b.y7
#define	Y8		BO[0].b.y8
#define	Y9		BO[0].b.y9
#define	Y10		BO[0].b.y10
#define	Y11		BO[0].b.y11
#define	Y12		BO[0].b.y12
#define	Y13		BO[0].b.y13
#define	Y14		BO[0].b.y14
#define	Y15		BO[0].b.y15
#define	Y16		BO[0].b.y16

unsigned short AI[8];				// Wejœcia analogowe: 8
unsigned short AO[8];				// Wyjœcia analogowe: 8

// ########### Struktury do obs³ugi peryferii ############

struct PLANT_IOstru
{	// Interfejs wejœæ i wyjœæ obiektowych
	unsigned short ai[8], ao[8];	// analogowe: 8 wejsæ, 8 wyjœæ (16-bit)
	unsigned short bi[2], bo[2];	// binarne: 2x16 wejœæ, 2x16 wyjœæ
	unsigned short fi[8], fo[8];	// licznikowe (czêstotliwoœæ)
} PLANT;

struct LCDbufStru
{	// Bufor wyœwietlacza LCD 2-linijkowego (ze sterownikiem HD44780U)
	unsigned char x, y;			// pozycja kursora: x=nr_kolumny, y=nr_wiersza
	unsigned char buf1[41];		// linia 1: 0x00..0x27
	unsigned char buf2[41];		// linia 2: 0x40..0x67
} LCD;
struct DISP_IOstru
{	// Interfejs panelu operatora
	unsigned short key, led;		// klawisze (16), diody LED (16)
	struct LCDbufStru lcd;			// wyœwietlacz LCD (2x24 znaki)
	unsigned char seg7[4];		// wyswietlacz 7-segmentowy (4 cyfry)
} DISP;

#define BUF1SIZE	128
struct COMbuf1Stru
{	// Bufor nr 1 do obs³ugi komunikacji
	unsigned short head;			// pocz¹tek (do zapisu znaków)
	unsigned short tail;			// koniec (do odczytu znaków)
	unsigned short stat;			// status: przepe³nienie, b³¹d CRC itp.
	unsigned char buf[BUF1SIZE];	// bufor
} BUF1;
#define BUF2SIZE	128
struct COMbuf2Stru
{	// Bufor nr 2 do obs³ugi komunikacji
	unsigned short head;			// pocz¹tek (do zapisu znaków)
	unsigned short tail;			// koniec (do odczytu znaków)
	unsigned short stat;			// status: przepe³nienie, b³¹d CRC itp.
	unsigned char buf[BUF2SIZE];	// bufor
} BUF2;
struct COM_IOstru
{	// Interfejs komunikacji szeregowej
	struct COMbuf1Stru inp;		// bufor wejœciowy (odbiornik)
	struct COMbuf2Stru out;		// bufor wyjœciowy (nadajnik)
} COM;

struct CHART_IOstru
{	// Interfejs danych wykresu
	short aTab[8];					// analogowe: 8 zmiennych <-32768, 32767>
	unsigned short bTab[16];		// binarne: 16 zmiennych <0, 65535>
} CHART;

// #################### Funkcje ####################

void dll_SetPlantOut(struct PLANT_IOstru *ptr)
{	unsigned char i;
	for(i=0; i<8; i++)
		AI[i] = ptr->ai[i];		// ObiektSym => wejœcia analogowe sterownika
	for(i=0; i<2; i++)
		BI[i].d = ptr->bi[i];		// ObiektSym => Wejœcia binarne sterownika
}
void dll_GetPlantInp(struct PLANT_IOstru *ptr)
{	unsigned char i;
	for(i=0; i<8; i++)
		ptr->ao[i] = AO[i];		// Wyjœcia analogowe sterownika => wejœcia analogowe obiektu (ObiektSym)
	for(i=0; i<2; i++)
		ptr->bo[i] = BO[i].d;		// Wyjœcia binarne sterownika => wejœcia binarne obiektu (ObiektSym)
}

void dll_SetDispOut(struct DISP_IOstru *ptr)
{	
	Keys.d = ptr->key;				// ObiektSym => klawisze sterownika
}
void dll_GetDispInp(struct DISP_IOstru *ptr)
{	unsigned char i;
	ptr->led = Leds.d;				// Pozycja x kursora => ObiektSym
	ptr->lcd.x = LCD.x;			// Pozycja x kursora => ObiektSym
	ptr->lcd.y = LCD.y;			// Pozycja y kursora => ObiektSym
	for(i=0; i<41; i++)
		ptr->lcd.buf1[i] = LCD.buf1[i];	// Linia 1 wyœwietlacza LCD => ObiektSym
	for(i=0; i<41; i++)
		ptr->lcd.buf2[i] = LCD.buf2[i];	// Linia 1 wyœwietlacza LCD => ObiektSym
}

void dll_GetChart(struct CHART_IOstru *ptr)
{	unsigned char i;
	for(i=0; i<8; i++)
		ptr->aTab[i] = aTab[i];	// zmienne analogowe sterownika => ObiektSym
	for(i=0; i<16; i++)
		ptr->bTab[i] = bTab[i];	// zmienne binarne sterownika => ObiektSym
}

void dll_SetComBuf(struct COM_IOstru *ptr)
{	unsigned short i;
	/*ptr->inp.head = BUF1.head;		// Pozycja zapisu bufora
	ptr->inp.tail = BUF1.tail;		// Pozycja odczytu bufora
	ptr->inp.stat = BUF1.stat;		// Status odbiornika
	for(i=0; i<BUF1SIZE; i++)
		ptr->inp.buf[i] = BUF1.buf[i];	// Bufor 1 (odbiornika)*/
	BUF1.head = ptr->inp.head;		// Pozycja zapisu bufora
	BUF1.tail = ptr->inp.tail;		// Pozycja odczytu bufora
	BUF1.stat = ptr->inp.stat;		// Status odbiornika
	for(i=0; i<BUF1SIZE; i++)
		BUF1.buf[i] = ptr->inp.buf[i];	// Bufor 1 (odbiornika)
	
	// Pozycja odczytu bufora nadajnika (po obs³udze poprz. cyklu)
	// zale¿na od prêdkoœci transmisji
	BUF2.tail = ptr->out.tail;
}
void dll_GetComBuf(struct COM_IOstru *ptr)	
{	unsigned short i;
	ptr->out.head = BUF2.head;		// Pozycja zapisu bufora 2
	ptr->out.tail = BUF2.tail;		// Pozycja odczytu bufora 2
	ptr->out.stat = BUF2.stat;		// Status nadajnika
	for(i=0; i<BUF2SIZE; i++)
		ptr->out.buf[i] = BUF2.buf[i];	// Bufor 2 (nadajnika)
	
	// Pozycja odczytu bufora odbiornika (po obs³udze cyklu)
	// zale¿na od prêdkoœci transmisji
	ptr->inp.tail = BUF1.tail;		// Pozycja odczytu bufora
}

// ---- LCD 2x24 znaki -----

void LCD_xy(unsigned char x, unsigned char y)	// Ustawienie kursora
{
	if(x<1) x=1;		// Pozycja pierwszego znaku w wierszu: 1
	if(y<1) y=1;		// 1 => wiersz 1, 2 => wiersz 2
	if(x>40) x=40;		// Ograniczenie do 40 znaków w wierszu
	if(y>2) y=2;		// Ograniczenie do 2 wierszy
	LCD.x = x;					
	LCD.y = y;					
}

void LCD_putchar(char ch)
{
    if(LCD.y<2)
        LCD.buf1[LCD.x] = ch;
    else
        LCD.buf2[LCD.x] = ch;
}

void LCD_puts(char *buf)
{   int i;

    if(LCD.y<2)
    {
        for(i=LCD.x-1; *buf && i<40; i++) LCD.buf1[i] = *buf++;
        LCD.buf1[i] = 0;
    }
    else
    {
        for(i=LCD.x-1; *buf && i<40; i++) LCD.buf2[i] = *buf++;
        LCD.buf2[i] = 0;
    }
}

// ----------------------------------------------------------

void SYM_read(void)		// IO_getInp()	// stare - dla kompatybilnoœci
{
/*    X1  = binp & 0x0001 ? 1 : 0;
    X2  = binp & 0x0002 ? 1 : 0;
    X3  = binp & 0x0004 ? 1 : 0;
    X4  = binp & 0x0008 ? 1 : 0;
    X5  = binp & 0x0010 ? 1 : 0;
    X6  = binp & 0x0020 ? 1 : 0;
    X7  = binp & 0x0040 ? 1 : 0;
    X8  = binp & 0x0080 ? 1 : 0;
    X9  = binp & 0x0100 ? 1 : 0;
    X10 = binp & 0x0200 ? 1 : 0;
    X11 = binp & 0x0400 ? 1 : 0;
    X12 = binp & 0x0800 ? 1 : 0;
    X13 = binp & 0x1000 ? 1 : 0;
    X14 = binp & 0x2000 ? 1 : 0;
    X15 = binp & 0x4000 ? 1 : 0;
    X16 = binp & 0x8000 ? 1 : 0;*/
}

void SYM_send(void)        // IO_setOut()  // stare - dla kompatybilnoœci
{
    /*bout = 0;
    if(Y1)  bout |= 0x0001;
    if(Y2)  bout |= 0x0002;
    if(Y3)  bout |= 0x0004;
    if(Y4)  bout |= 0x0008;
    if(Y5)  bout |= 0x0010;
    if(Y6)  bout |= 0x0020;
    if(Y7)  bout |= 0x0040;
    if(Y8)  bout |= 0x0080;
    if(Y9)  bout |= 0x0100;
    if(Y10) bout |= 0x0200;
    if(Y11) bout |= 0x0400;
    if(Y12) bout |= 0x0800;
    if(Y13) bout |= 0x1000;
    if(Y14) bout |= 0x2000;
    if(Y15) bout |= 0x4000;
    if(Y16) bout |= 0x8000;*/
}

int KEY_read(void)					// Odczyt aktualnego stanu klawiszy
{
	aK1 = !K1; aK2 = !K2; aK3 = !K3; aK4 = !K4;
	aK5 = !K5; aK6 = !K6; aK7 = !K7; aK8 = !K8;
	return keys;
}

void KEY_mem(void)					// Zapamiêtanie stanu klawiszy
{
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}
void AI_read(void)					// Odczyt wejœæ analogowych AI[0]..AI[7]
{}
void BI_read()              		// Odczyt wejœæ binarnych BI[0]..BI[7]
{}
void LED_set()              		// Ustawienie diod LED L1..L8
{}
void AO_set()               		// Ustawienie wyjœæ analogowych AO[0]..AO[7]
{}
void BO_set()               		// Ustawienie wyjœæ binarnych BO[0]..BO[7]
{}

extern void prolog(void);			// Obliczenia pocz¹tkowe
extern void oblicz(void);			// Obliczenia cykliczne
extern void przerwanie(void);		// Obs³uga przerwania T0
extern void komunikacja(void);		// Obs³uga komunikacji
extern void wykres(void);			// Obliczenia do przebiegów

DECLSPEC void setup(void)        	// Obliczenia pocz¹tkowe (inicjalizacja)
{
	prolog();
}

DECLSPEC void task(void)      	// G³ówna funkcja wywo³ywana cyklicznie co zadany czas (np.100ms)
{
    KEY_read();             		// Odczyt stanu klawiszy aK1..aK8
    AI_read();              		// Odczyt wejœæ analogowych AI[0]..AI[7]
    BI_read();              		// Odczyt wejœæ binarnych BI[0]..BI[7]

    oblicz();               		// Obliczenia u¿ytkownika

    LED_set();              		// Ustawienie diod LED L1..L8
    AO_set();               		// Ustawienie wyjœæ analogowych AO[0]..AO[7]
    BO_set();               		// Ustawienie wyjœæ binarnych BO[0]..BO[7]

    SYM_send();             		// Przes³anie stanu wyjœæ do symulatora obiektu
    SYM_read();             		// Odczyt stanu wejœæ z symulatora obiektu

    komunikacja();          		// Obs³uga komunikacji szeregowej
}

DECLSPEC void int_T0(void)         	// Funkcja obs³ugi przerwania od uk³adu czasowo-licznikowego
{
	przerwanie();
}

DECLSPEC void trend(void)         	// Funkcja rysowania przebiegów czasowych
{
	wykres();
}

#endif
