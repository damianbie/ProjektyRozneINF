#include <util/delay.h> 

#define DDR_DB4  DDRD 
#define PORT_DB4 PORTD 
#define DB4      PD2 

#define DDR_DB5  DDRD 
#define PORT_DB5 PORTD 
#define DB5      PD3 

#define DDR_DB6  DDRD 
#define PORT_DB6 PORTD 
#define DB6      PD4 

#define DDR_DB7  DDRD 
#define PORT_DB7 PORTD 
#define DB7      PD5 


#define DDR_RS   DDRD 
#define PORT_RS  PORTD 
#define RS       PD6 

#define DDR_E    DDRD 
#define PORT_E   PORTD 
#define E        PD7 

#define SET_DB4 PORT_DB4 |= _BV(DB4) 
#define CLR_DB4 PORT_DB4 &= ~_BV(DB4) 

#define SET_DB5 PORT_DB5 |= _BV(DB5) 
#define CLR_DB5 PORT_DB5 &= ~_BV(DB5) 

#define SET_DB6 PORT_DB6 |= _BV(DB6) 
#define CLR_DB6 PORT_DB6 &= ~_BV(DB6) 

#define SET_DB7 PORT_DB7 |= _BV(DB7) 
#define CLR_DB7 PORT_DB7 &= ~_BV(DB7) 

#define SET_E PORT_E |= _BV(E) 
#define CLR_E PORT_E &= ~_BV(E) 

#define SET_RS PORT_RS |= _BV(RS) 
#define CLR_RS PORT_RS &= ~_BV(RS) 

// funkcja wystawiaj�ca na szyn� danych po��wk� bajtu 
// pod uwag� brana jest m�odsza po��wka parametru 
void out_nibble(char x) 
{ 
	if(x & _BV(0)) SET_DB4; else  CLR_DB4; 
	if(x & _BV(1)) SET_DB5; else  CLR_DB5; 
	if(x & _BV(2)) SET_DB6; else  CLR_DB6; 
	if(x & _BV(3)) SET_DB7; else  CLR_DB7; 
} 

// funkcja zapisuj�ca do LCD bajt danych 
void write_to_lcd(char x) 
{ 
	SET_E; 
	out_nibble(x >> 4); 				// wystawienie na szyn� danych starszej po��wki bajtu 
	CLR_E; 							    // opadaj�ce zbocze na E powoduje zapis do wy�wietlacza 
	SET_E; 
	out_nibble(x); 						// wystawienie na szyn� danych m�odszej po��wki bajtu 
	CLR_E; 								// opadaj�ce zbocze na E powoduje zapis do wy�wietlacza 
	_delay_ms(1); 						// op�nienie ok 1 ms 
} 

// funkcja zapisuj�ca do LCD rozkaz 
void write_command(char x) 
{ 
	CLR_RS; 							// niski stan na RS -> zapis instrukcji 
	write_to_lcd(x); 					// zapis do LCD 
} 

// funkcja zapisujaca do LCD znak 
void LCD_char(char x) 
{ 
	SET_RS; 							// wysoki stan na RS -> zapis danej 
	write_to_lcd(x); 					// zapis do LCD 
} 

// funkcja zapisu tekstu do wy�wietlacza LCD 
void LCD_puts(char * s) 
{ 
	while(*s) 							// do napotkania 0 
	{ 
		LCD_char(*s); 					// zapisz znak wskazywany przez s na LCD 
		s++; 							// zwi�ksz s (przygotuj nastepny znak) 
	} 
} 

// funkcja inicjuj�ca LCD 
void LCD_init(void) 
{ 
	// ustawienie wszystkich linii steruj�cych LCD jako wyj�cia 
	DDR_DB4 |= _BV(DB4); 
	DDR_DB5 |= _BV(DB5); 
	DDR_DB6 |= _BV(DB6); 
	DDR_DB7 |= _BV(DB7); 
	DDR_E |= _BV(E); 
	DDR_RS |= _BV(RS); 
 
	_delay_ms(15); 						// czekaj 15ms na ustabilizowanie si� napi�cia zasilaj�cego 
	CLR_E; 								// E = 0 
	CLR_RS; 							// RS = 0 
	char i; 							// zmianna licznikowa 
	for(i = 0; i < 3; i++) 			// trzykrotne powt�rzenie bloku instrukcji 
	{ 
		SET_E; 							// E = 1 
		out_nibble(0x03); 
		CLR_E; 							// E = 0 
		_delay_ms(5); 					// czekaj 5ms 
	} 
	SET_E; 								// E = 1 
	out_nibble(0x02); 
	CLR_E; 								// E = 0 
	_delay_ms(1); 						// czekaj 1ms 
	write_command(0x28); 				// interfejs 4-bity, 2-linie, znak 5x7 
	write_command(0x08);				// wy��cz LCD, kursor i miganie 
	write_command(0x01); 				// czy�� LCD 
	write_command(0x06); 				// bez przesuwania w prawo 
	write_command(0x0C); 				// w��cz LCD, bez kursora i mrugania 
} 

// funkcja ustawiajaca kursor 
void LCD_xy(char x,char y) 
{ 
	unsigned char com=0x80; 
	com|=(x|(y<<6)); 
	write_command(com); 
}

// funkcja czyszczaca wyswietlacz LCD 
void LCD_clr(void)
{
write_command(0x01);                     //Czy�ci wy�wietlacz i ustawia kursor na pocz�tku (adres=0)
write_command(0x02);                     //Aby po wyczyszczeniu LCD dziala za pierwszym razem funkcja LCD_xy()
}
