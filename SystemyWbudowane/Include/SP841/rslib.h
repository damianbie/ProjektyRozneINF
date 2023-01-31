/*************************************************************************/
/* Plik nag³owkowy biblioteki komunikacji z symulatorem obiektów         */
/*************************************************************************/

#define FRAME_IN_MAX 40 	// Rozmiar bufora komunikatów
#define aWe_MAX 16			// Max iloœæ wejœæ analogowych
#define aWy_MAX 16			// Max iloœæ wyjœæ analogowych

// SYM_init() - Inicjowanie komunikacji z symulatorem obiektów
// predkosc   - Prêdkoœæ transmisji RS-232
// ile_aWe    - Ile zmiennych ca³kowitych w ramce wejœciowej (z symulatora)
// ile_aWy    - Ile zmiennych ca³kowitych w ramce wyjœciowej (do symulatora)
char SYM_init(long predkosc, char ile_aWe, char ile_aWy);

char SYM_read();			// Odbiór komunikatu z symulatora obiektów
void SYM_send();			// Wys³anie komunikatu do symulatora obiektów
