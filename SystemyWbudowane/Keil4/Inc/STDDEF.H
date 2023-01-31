/* STDDEF.H: standard definitions            */
/* Copyright KEIL ELEKTRONIK GmbH 1992 V1.00 */

#ifndef NULL
 #define NULL ((void *) 0)
#endif

#ifndef _SIZE_T
 #define _SIZE_T
 typedef unsigned int size_t;
#endif


typedef int          ptrdiff_t;
#define offsetof(s,m)	(size_t)&(((s *)0)->m)
