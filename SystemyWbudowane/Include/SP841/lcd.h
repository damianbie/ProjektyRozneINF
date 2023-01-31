/*************************************************************************/
/* Plik : LCD.h                                                          */
/* Funkcje obs³ugi wyœwietlacza LCD w systemie prototypowym SP-841       */
/*************************************************************************/

void LCD_init(void);			// Inicjalizacja wyœwietlacza 
void LCD_clr(void);				// Zerowanie wyœwietlacza
void LCD_on(void);				// W³¹czenie LCD i kursora
void LCD_off(void);				// Wy³¹czenie LCD
void LCD_xy(char col, char row);// Pozycja kursora: kolumna, wiersz
void LCD_char(char c);	  		// Wyœwietlenie znaku ASCII
void LCD_puts(char *ptr);		// Wyœwietlenie ³añcucha znaków
