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
