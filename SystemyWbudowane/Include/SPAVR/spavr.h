// Przyciski K1..K8
#define DDRKEY	DDRB			// Rejestr kierunku danych (wejœcie/wyjœcie)
#define PKEY	PORTB			// Rejestr portu z przyciskami (SW0..SW7)
#define KEY		PINB			// Rejestr aktualnego stanu przycisków

#define K1 (KEY & 0x01)
#define K2 (KEY & 0x02)
#define K3 (KEY & 0x04)
#define K4 (KEY & 0x08)
#define K5 (KEY & 0x10)
#define K6 (KEY & 0x20)
#define K7 (KEY & 0x40)
#define K8 (KEY & 0x80)

// Diody LED
#define DDRLED	DDRA			// Rejestr kierunku danych (wejœcie/wyjœcie)
#define LED		PORTA			// Rejestr portu z diodami LED
#define LEDBITS	0x3F			// Maska dla portu z diodami LED

#define D1 0
#define D2 1
#define D3 2
#define D4 3
#define D5 4
#define D6 5

char L1,L2,L3,L4,L5,L6,L7,L8;	// Diody LD0..LD7

void LED_init(void)				// Inicjowanie rejestrów z diodami LED
{
	DDRLED |= LEDBITS;			// Kierunek linii: port wyjœciowy
	LED    |= LEDBITS;			// Diody LED: "1" = wygaszone diody
}

void LED_set(void)				// Ustaw diody LED
{ unsigned char leds=0;
  
  if(L1) leds |= _BV(D1);		// 0x01;
  if(L2) leds |= _BV(D2);		// 0x02;
  if(L3) leds |= _BV(D3);		// 0x04;
  if(L4) leds |= _BV(D4);		// 0x08;
  if(L5) leds |= _BV(D5);		// 0x10;
  if(L6) leds |= _BV(D6);		// 0x20;
  LED = (LED&~LEDBITS) | (~leds&LEDBITS);	// Wpis do rejestru portu
}





//char aK1,aK2,aK3,aK4,aK5,aK6,aK7,aK8;	// Aktualny stan klawiszy
//char pK1,pK2,pK3,pK4,pK5,pK6,pK7,pK8;	// Poprzedni stan klawiszy

void KEY_init(void)				// Inicjowanie rejestrów dla klawiszy
{
	DDRKEY &= 0x00;				// Kierunek linii: "0" = port wejœciowy
	PKEY   |= 0xff;				// "1" = aktywne rezystory podci¹gaj¹ce (pull-up)
}

int KEY_read(void)				// Odczyt aktualnego stanu klawiszy
{
	aK1 = !K1;		// 0x01
	aK2 = !K2;		// 0x02
	aK3 = !K3;		// 0x04
	aK4 = !K4;		// 0x08
	aK5 = !K5;		// 0x10
	aK6 = !K6;		// 0x20
	aK7 = !K7;		// 0x40
	aK8 = !K8;		// 0x80
	return (~KEY & 0xFF);
}

void KEY_mem(void)				// Zapamiêtanie stanu klawiszy
{
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}
