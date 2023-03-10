/*************************************************************************/
/* Plik : LCD.h                                                          */
/* Funkcje obsługi wyświetlacza LCD w systemie prototypowym SP-841       */
/*************************************************************************/

void LCD_init(void);			// Inicjalizacja wyświetlacza 
void LCD_clr(void);				// Zerowanie wyświetlacza
void LCD_on(void);				// Włączenie LCD i kursora
void LCD_off(void);				// Wyłączenie LCD
void LCD_xy(char col, char row);// Pozycja kursora: kolumna, wiersz
void LCD_char(char c);	  		// Wyświetlenie znaku ASCII
void LCD_puts(char *ptr);		// Wyświetlenie łańcucha znaków
