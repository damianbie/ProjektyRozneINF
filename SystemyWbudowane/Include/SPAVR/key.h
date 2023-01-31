#define DDRKEY	DDRB			// Rejestr kierunku danych (wejœcie/wyjœcie)
#define PKEY	PORTB			// Rejestr portu z przyciskami (SW0..SW7)
#define KEY		PINB			// Rejestr aktualnego stanu przycisków

#define KEY1 0
#define KEY2 1
#define KEY3 2
#define KEY4 3
#define KEY5 4
#define KEY6 5
#define KEY7 6
#define KEY8 7

#define K1 (KEY & 0x01)
#define K2 (KEY & 0x02)
#define K3 (KEY & 0x04)
#define K4 (KEY & 0x08)
#define K5 (KEY & 0x10)
#define K6 (KEY & 0x20)
#define K7 (KEY & 0x40)
#define K8 (KEY & 0x80)

char aK1,aK2,aK3,aK4,aK5,aK6,aK7,aK8;	// Aktualny stan klawiszy
char pK1,pK2,pK3,pK4,pK5,pK6,pK7,pK8;	// Poprzedni stan klawiszy

void KEY_init(void)				// Inicjowanie rejestrów dla klawiszy
{
	DDRKEY &= 0x00;				// Kierunek linii: "0" = port wejœciowy
	PKEY   |= 0xff;				// "1" = aktywne rezystory podci¹gaj¹ce (pull-up)
}

int KEY_read(void)				// Odczyt aktualnego stanu klawiszy
{
	/*aK1 = !(KEY & _BV(KEY1));		// 0x01
	aK2 = !(KEY & _BV(KEY2));		// 0x02
	aK3 = !(KEY & _BV(KEY3));		// 0x04
	aK4 = !(KEY & _BV(KEY4));		// 0x08
	aK5 = !(KEY & _BV(KEY5));		// 0x10
	aK6 = !(KEY & _BV(KEY6));		// 0x20
	aK7 = !(KEY & _BV(KEY7));		// 0x40
	aK8 = !(KEY & _BV(KEY8));		// 0x80*/
	aK1 = !K1;		// 0x01
	aK2 = !K2;		// 0x02
	aK3 = !K3;		// 0x04
	aK4 = !K4;		// 0x08
	aK5 = !K5;		// 0x10
	aK6 = !K6;		// 0x20
	aK7 = !K7;		// 0x40
	aK8 = !K8;		// 0x80
	return KEY;
}

void KEY_mem(void)				// Zapamiêtanie stanu klawiszy
{
	pK1=aK1; pK2=aK2; pK3=aK3; pK4=aK4;
	pK5=aK5; pK6=aK6; pK7=aK7; pK8=aK8;
}
