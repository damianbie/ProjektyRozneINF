/* CTYPE.H: prototypes for character functions     */
/* Copyright KEIL ELEKTRONIK GmbH 1988, 1990 V3.0  */

#pragma SAVE
#pragma REGPARMS
extern bit isalpha (char);
extern bit isalnum (char);
extern bit iscntrl (char);
extern bit isdigit (char);
extern bit isgraph (char);
extern bit isprint (char);
extern bit ispunct (char);
extern bit islower (char);
extern bit isupper (char);
extern bit isspace (char);
extern bit isxdigit (char);
extern char tolower (char);
extern char toupper (char);
extern char toint   (char);

#define _tolower(c) ( (c)-'A'+'a' )
#define _toupper(c) ( (c)-'a'+'A' )
#define toascii(c)  ( (c) & 0x7F )
#pragma RESTORE
