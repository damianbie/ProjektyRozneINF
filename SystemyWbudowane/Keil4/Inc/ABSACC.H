/* ABSACC.H: direct access to 8051 memory areas    */
/* Copyright KEIL ELEKTRONIK GmbH 1988, 1990 V3.03 */

#define CBYTE ((unsigned char volatile *) 0x50000L)
#define DBYTE ((unsigned char volatile *) 0x40000L)
#define PBYTE ((unsigned char volatile *) 0x30000L)
#define XBYTE ((unsigned char volatile *) 0x20000L)

#define CWORD ((unsigned int volatile *) 0x50000L)
#define DWORD ((unsigned int volatile *) 0x40000L)
#define PWORD ((unsigned int volatile *) 0x30000L)
#define XWORD ((unsigned int volatile *) 0x20000L)
