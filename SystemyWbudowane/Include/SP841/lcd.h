/*************************************************************************/
/* Plik : LCD.h                                                          */
/* Funkcje obs�ugi wy�wietlacza LCD w systemie prototypowym SP-841       */
/*************************************************************************/

void LCD_init(void);			// Inicjalizacja wy�wietlacza 
void LCD_clr(void);				// Zerowanie wy�wietlacza
void LCD_on(void);				// W��czenie LCD i kursora
void LCD_off(void);				// Wy��czenie LCD
void LCD_xy(char col, char row);// Pozycja kursora: kolumna, wiersz
void LCD_char(char c);	  		// Wy�wietlenie znaku ASCII
void LCD_puts(char *ptr);		// Wy�wietlenie �a�cucha znak�w
