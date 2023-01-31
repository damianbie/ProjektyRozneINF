/* MATH.H: prototypes for mathematic functions     */
/* Copyright KEIL ELEKTRONIK GmbH 1988, 1990 V3.20 */

#pragma SAVE
#pragma REGPARMS
extern char  cabs  (char  val);
extern int    abs  (int   val);
extern long  labs  (long  val);
extern float fabs  (float val);
extern float sqrt  (float val);
extern float exp   (float val);
extern float log   (float val);
extern float log10 (float val);
extern float sin   (float val);
extern float cos   (float val);
extern float tan   (float val);
extern float asin  (float val);
extern float acos  (float val);
extern float atan  (float val);
extern float sinh  (float val);
extern float cosh  (float val);
extern float tanh  (float val);
extern float atan2 (float y, float x);

struct FPBUF  {
  unsigned char save[16];
};

extern void  fpsave (struct FPBUF *);
extern void  fprestore (struct FPBUF *);

extern float ceil  (float val);
extern float floor (float val);
extern float modf  (float val, float *n);
extern float pow   (float x, float y);

#pragma RESTORE
