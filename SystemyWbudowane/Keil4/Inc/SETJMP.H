/* SETJMP.H: prototypes for longjmp facility  */
/* Copyright KEIL ELEKTRONIK GmbH 1990 V3.00  */

/* define the buffer type for holding the state information */

#pragma SAVE
#pragma REGPARMS
#ifndef _JMP_BUF_DEFINED
#define _JBLEN	8  /* RET-ADDR, ?C_XBP, ?C_IBP, ?C_BP, SP, ASTACK */
typedef  char jmp_buf[_JBLEN];
#define _JMP_BUF_DEFINED
#endif

/* function prototypes */
int  setjmp  (jmp_buf);
void longjmp (jmp_buf, int);
#pragma RESTORE
