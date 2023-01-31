/*************************************************************************/
/* Plik nag�owkowy biblioteki komunikacji z symulatorem obiekt�w         */
/*************************************************************************/

#define FRAME_IN_MAX 40 	// Rozmiar bufora komunikat�w
#define aWe_MAX 16			// Max ilo�� wej�� analogowych
#define aWy_MAX 16			// Max ilo�� wyj�� analogowych

// SYM_init() - Inicjowanie komunikacji z symulatorem obiekt�w
// predkosc   - Pr�dko�� transmisji RS-232
// ile_aWe    - Ile zmiennych ca�kowitych w ramce wej�ciowej (z symulatora)
// ile_aWy    - Ile zmiennych ca�kowitych w ramce wyj�ciowej (do symulatora)
char SYM_init(long predkosc, char ile_aWe, char ile_aWy);

char SYM_read();			// Odbi�r komunikatu z symulatora obiekt�w
void SYM_send();			// Wys�anie komunikatu do symulatora obiekt�w
