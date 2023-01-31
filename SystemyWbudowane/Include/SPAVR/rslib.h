#define FRAME_IN_MAX 40 						// Rozmiar bufora komunikatów
#define aWe_MAX 16		 						// Max liczba wejœæ analogowych
#define aWy_MAX 16		 						// Max liczba wyjœæ analogowych

extern volatile unsigned char cykl;				// Czas cyklu pêtli g³ównej

unsigned char X1,X2, X3, X4, X5, X6, X7, X8;	// Wejœcia sterownika / wyjœcia obiektu (czujniki)
unsigned char X9,X10,X11,X12,X13,X14,X15,X16;	// Wejœcia sterownika / wyjœcia obiektu (czujniki)
unsigned char Y1,Y2, Y3, Y4, Y5, Y6, Y7, Y8;	// Wyjœcia sterownika / wejœcia obiektu (zawory)
unsigned char Y9,Y10,Y11,Y12,Y13,Y14,Y15,Y16;	// Wyjœcia sterownika / wejœcia obiektu (zawory)

char volatile frame_in[FRAME_IN_MAX];			// Bufor ramki odbieranego komunikatu
char bWe[2]={0,0}, bWy[2]={0,0};				// Wejœcia i wyjœcia binarne ("bezpieczne" wart. pocz¹tkowe)
unsigned int aWe[aWe_MAX],aWy[aWy_MAX];			// Tablice wejœæ i wyjœæ analogowych

static char volatile rozpoczeto_transmisje=0;

unsigned char IN_ILE_CALKOWITYCH = 0;
static unsigned char volatile FRAME_IN_SIZE = 4;

unsigned char OUT_ILE_CALKOWITYCH = 0;
static unsigned char volatile FRAME_OUT_SIZE = 4;

static unsigned char volatile x_in=0;			// wskaŸnik zapisu do ramki wejsciowej
static unsigned char volatile y_out=0;			// wskaŸnik zapisu do ramki wyjsciowej

SIGNAL (SIG_USART_RECV)				// Przerwanie od odbiornika UART
{
	frame_in[x_in++] = UDR;						// Zapis znaku do bufora
		
	if(x_in >= FRAME_IN_SIZE)					// Jeœli koniec ramki komunikatu to:
	{
		x_in = 0;								// - wróæ na pocz¹tek bufora
		rozpoczeto_transmisje=1;				// - ramka gotowa do odczytu
	}
	UCSRA &= ~_BV(RXC);							// Potwierdzenie odebrania znaku
}

char UART_init(long baud)			// Inicjowanie portu szeregowego
{   
	char speed, err=0;
		
	cli();										// Zablokowanie wszystkich przerwañ
	
	speed = baud / 4800;
	switch(speed)
	{
		case  1: 
			UBRRH=0; UBRRL=103;					// 4800   (fosc=8MHz => UBRRL=103)
			break;
		case  2: 
			UBRRH=0; UBRRL=51;					// 9600   (fosc=8MHz => UBRRL=51)
			break;
		case  4: 
			UBRRH=0; UBRRL=25;					// 19200  (fosc=8MHz => UBRRL=25)
			break;
		case 12: 
			UBRRH=0; UBRRL=8;					// 57600  (fosc=8MHz => UBRRL=8)
			break;
		default: 
			UBRRH=0; UBRRL=25;					// 19200   (fosc=8MHz => UBRRL=25)
			err = 1;							// nietypowa prêdkoœæ (ustawiona domyœlna)
			break;
	}
	UCSRC =_BV(URSEL)|_BV(UCSZ1)|_BV(UCSZ0);	// transmisja 8N1
	UCSRB =_BV(RXEN)|_BV(TXEN)|_BV(RXCIE);		// w³¹czenie odbiornika RX i nadajnika TX 
	
	sei();										// Globalne odblokowanie przerwañ
	
	return err;
}

char checkCRC(void)					// Obliczenie sumy kontrolnej komunikatu
{	
	unsigned char i = 0;						// zmienna pomocnicza
	char sumaCRC = 0x55;						// pocz¹tkowa wartoœæ sumy kontrolnej

	for(i=0; i<(FRAME_IN_SIZE-1); i++)
		sumaCRC ^= frame_in[i];

	if(sumaCRC != frame_in[i]) return 0;

	return 1;
}

char SYM_init(long predkosc, char ile_aWe, char ile_aWy)
{
	unsigned char i;

	if(ile_aWe*2>sizeof(aWe)) return 2;			// Zbyt ma³y rozmiar aWe[]
	if(ile_aWy*2>sizeof(aWy)) return 3;			// Zbyt ma³y rozmiar aWy[]
	if(ile_aWe*2+4>sizeof(frame_in)) return 4;	// Zbyt ma³y rozmiar frame_in[]

	cli();										// Zablokowanie obs³ugi przerwañ
	// Wyczyszczenie buforow
	for(i=0; i<FRAME_IN_MAX; i++) { frame_in[i]=0; }
	bWy[0]=0; bWy[1]=0;							// Ustawienie "bezpiecznych" wart. pocz¹tkowych
	for(i=0; i<aWy_MAX; i++) { aWy[i]=0; }

	// Ustawienie zmiennych
	FRAME_IN_SIZE = 4 + ile_aWe * 2;			// Rozmiar komunikatu (razem z CRC)
	OUT_ILE_CALKOWITYCH = ile_aWy;				// Liczba wyjœæ analogowych
	IN_ILE_CALKOWITYCH = ile_aWe;				// Liczba wejœæ analogowych
	
	if(UART_init(predkosc)) return 1;			// B³¹d inicjowania UART
	
	x_in = 0;									// Opró¿nienie bufora komunikatów
	rozpoczeto_transmisje=0;
	sei();										// Odblokowanie obs³ugi przerwañ

	X1 = bWe[0]&0x01 ? 1 : 0;					// Pocz¹tkowy stan czujnika X1
	X2 = bWe[0]&0x02 ? 1 : 0;					// Pocz¹tkowy stan czujnika X2
	X3 = bWe[0]&0x04 ? 1 : 0;					// Pocz¹tkowy stan czujnika X3
	X4 = bWe[0]&0x08 ? 1 : 0;					// Pocz¹tkowy stan czujnika X4
	X5 = bWe[0]&0x10 ? 1 : 0;					// Pocz¹tkowy stan czujnika X5
	X6 = bWe[0]&0x20 ? 1 : 0;					// Pocz¹tkowy stan czujnika X6
	X7 = bWe[0]&0x40 ? 1 : 0;					// Pocz¹tkowy stan czujnika X7
	X8 = bWe[0]&0x80 ? 1 : 0;					// Pocz¹tkowy stan czujnika X8
	X9 = bWe[1]&0x01 ? 1 : 0;					// Pocz¹tkowy stan czujnika X9
	X10= bWe[1]&0x02 ? 1 : 0;					// Pocz¹tkowy stan czujnika X10
	X11= bWe[1]&0x04 ? 1 : 0;					// Pocz¹tkowy stan czujnika X11
	X12= bWe[1]&0x08 ? 1 : 0;					// Pocz¹tkowy stan czujnika X12
	
	return 0;
}

char SYM_read(void)					// Odczyt wejœæ symulowanego obiektu
{	
	unsigned char i;
	int *ptr=(int*)(frame_in+3);				// WskaŸnik na pierwsze wejœcie analogowe
	
	while(x_in)									// Poczekaj na odczyt ca³ej ramki
	{
		if(!cykl) break;						// Przerwij jeœli up³yn¹³ czas
		{
			x_in=0;								// Rozpocznij od nowa
			rozpoczeto_transmisje=0;
		}
	}
	if(rozpoczeto_transmisje)
	{
		if(checkCRC())							// Jeœli suma kontrolna OK
		{
			bWe[0]=frame_in[1];					// 1-szy bajt wejœæ binarnych
			bWe[1]=frame_in[2];					// 2-gi bajt wejœæ binarnych
			for(i=0;i<IN_ILE_CALKOWITYCH;i++)
				aWe[i]=*ptr++;					// Kolejne wejœcia analogowe
				
			X1 = bWe[0]&0x01 ? 1 : 0;			// Stan czujnika X1
			X2 = bWe[0]&0x02 ? 1 : 0;			// Stan czujnika X2
			X3 = bWe[0]&0x04 ? 1 : 0;			// Stan czujnika X3
			X4 = bWe[0]&0x08 ? 1 : 0;			// Stan czujnika X4
			X5 = bWe[0]&0x10 ? 1 : 0;			// Stan czujnika X5
			X6 = bWe[0]&0x20 ? 1 : 0;			// Stan czujnika X6
			X7 = bWe[0]&0x40 ? 1 : 0;			// Stan czujnika X7
			X8 = bWe[0]&0x80 ? 1 : 0;			// Stan czujnika X8
			X9 = bWe[1]&0x01 ? 1 : 0;			// Stan czujnika X9
			X10= bWe[1]&0x02 ? 1 : 0;			// Stan czujnika X10
			X11= bWe[1]&0x04 ? 1 : 0;			// Stan czujnika X11
			X12= bWe[1]&0x08 ? 1 : 0;			// Stan czujnika X12
		}
		rozpoczeto_transmisje=0;
		return 1;
	}
	return 0;	
}


void SYM_send(void)					// Wyœlij komunikat do symulatora obiektów
{	unsigned char i;
	unsigned char crc = 0x55;
	unsigned char byte_to_send = 0;
	
	bWy[0]=0;									// Stan wyjœæ sterownika
	if(Y1) bWy[0] |= 0x01;						// Dopisz stan wyjœcia Y1
	if(Y2) bWy[0] |= 0x02;						// Dopisz stan wyjœcia Y2
	if(Y3) bWy[0] |= 0x04;						// Dopisz stan wyjœcia Y3
	if(Y4) bWy[0] |= 0x08;						// Dopisz stan wyjœcia Y4
	if(Y5) bWy[0] |= 0x10;						// Dopisz stan wyjœcia Y5
	if(Y6) bWy[0] |= 0x20;						// Dopisz stan wyjœcia Y6
	if(Y7) bWy[0] |= 0x40;						// Dopisz stan wyjœcia Y7
	if(Y8) bWy[0] |= 0x80;						// Dopisz stan wyjœcia Y8
	
	//Wyœlij ile liczb ca³kowitych
	while(!(UCSRA & _BV(UDRE)));				// Czekaj, jeœli nie wys³any poprzedni znak
	UDR = OUT_ILE_CALKOWITYCH;					// Wyœlij przez UART liczbê wyjœæ analogowych
	crc ^= OUT_ILE_CALKOWITYCH;					// Obliczaj sumê kontroln¹
	
	//Wyœlij pola bitowe
	while(!(UCSRA & _BV(UDRE)));				// Czekaj, jeœli nie wys³any poprzedni znak
	UDR = bWy[0];								// Wyœlij przez UART 1-szy bajt wyjœæ binarnych
	crc ^= bWy[0];								// Obliczaj sumê kontroln¹
	while(!(UCSRA & _BV(UDRE)));				// Czekaj, jeœli nie wys³any poprzedni znak
	UDR = bWy[1];								// Wyœlij przez UART 2-gi bajt wyjœæ binarnych
	crc ^= bWy[1];								// Obliczaj sumê kontroln¹
	
	//wyœlij liczby ca³kowite
	for(i=0; i<OUT_ILE_CALKOWITYCH; i++)
	{
		byte_to_send = aWy[y_out]>>8;			// Starszy bajt liczby
		while(!(UCSRA & _BV(UDRE)));			// Czekaj, jeœli nie wys³any poprzedni znak
		UDR = byte_to_send;						// Wyœlij bajt przez UART
		crc ^= byte_to_send;					// Obliczaj sumê kontroln¹

		byte_to_send = (char)aWy[y_out];		// M³odszy bajt liczby
		while(!(UCSRA & _BV(UDRE)));			// Czekaj, jeœli nie wys³any poprzedni znak
		UDR = byte_to_send;						// Wyœlij bajt przez UART
		crc ^= byte_to_send;					// Obliczaj sumê kontroln¹
	}
	//wyœlij crc
	while(!(UCSRA & _BV(UDRE)));				// Czekaj, jeœli niewys³any poprzedni znak
	UDR = crc;									// Wyœlij przez UART sumê kontroln¹
}
