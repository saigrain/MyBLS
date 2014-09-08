#ifndef __FLOATMATH_H__
#define __FLOATMATH_H__

#include <math.h>

#if defined(__SUNPRO_C)

/* The Sun Workshop compiler suite has the 'float' math functions
 * in sunmath.h.
 */

#include <sunmath.h>
#include <ieeefp.h>

#elif defined(__SVR4)  /* XXX - what does SunOS <5 define? */

/* Emulate 'float' versions of the math.h functions we use on systems which
 * don't have them */

/* sqrtf */
#define sqrtf(a) ((float) sqrt((double) (a)))

/* fabsf */
#define fabsf(a) ((float) fabs((double) (a)))

/* logf */
#define logf(a) ((float) log((double) (a)))

/* log10f */
#define log10f(a) ((float) log10((double) (a)))

/* expf */
#define expf(a) ((float) exp((double) (a)))

/* sinf */
#define sinf(a) ((float) sin((double) (a)))

/* cosf */
#define cosf(a) ((float) cos((double) (a)))

/* tanf */
#define tanf(a) ((float) tan((double) (a)))

/* asinf */
#define asinf(a) ((float) asin((double) (a)))

/* acosf */
#define acosf(a) ((float) acos((double) (a)))

/* atanf */
#define atanf(a) ((float) atan((double) (a)))

/* atan2f */
#define atan2f(x, y) ((float) atan2((double) (x), (double) (y)))

/* powf */
#define powf(x, y) ((float) pow((double) (x), (double) (y)))

/* isnanf */
#define isnanf(x) isnan((double) (x))

#endif

#endif  /* __FLOATMATH_H__ */
