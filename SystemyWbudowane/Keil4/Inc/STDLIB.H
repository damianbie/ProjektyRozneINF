/* STDLIB.H: standard functions                    */
/* Copyright KEIL ELEKTRONIK GmbH 1988, 1992 V3.40 */

#ifndef _SIZE_T
 #define _SIZE_T
 typedef unsigned int size_t;
#endif

#ifndef NULL
 #define NULL ((void *) 0L)
#endif

#pragma SAVE
#pragma REGPARMS

extern int    abs  (int   val);
extern long  labs  (long  val);

extern float atof (void *s1);
extern long  atol (void *s1);
extern int   atoi (void *s1);
extern int   rand ();
extern void  srand (int);

extern void *malloc      (unsigned int size);
extern void free         (void xdata *p);
extern void init_mempool (void xdata *p, unsigned int size);
extern void *realloc     (void xdata *p, unsigned int size);
extern void *calloc      (unsigned int size, unsigned int len);

#pragma RESTORE
