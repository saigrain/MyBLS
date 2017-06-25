#ifndef __UTIL_H__
#define __UTIL_H__

/* --Constants-- */

/* Length of error message buffers */
#define ERRSTR_LEN 1024

/* Precision */
#define EPS 1e-15

/* --Macros-- */

#undef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#undef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#undef ABS
#define ABS(x) ((x) >= 0 ? (x) : -(x))

#undef SWAP
#define SWAP(x, y, t) (t) = (x); (x) = (y); (y) = (t)

#define ARRAYLEN(a) (sizeof(a)/sizeof((a)[0]))

#define NINT(a) ((int) ((a)+((a) < 0 ? -0.5 : 0.5)))
#define NLNG(a) ((long) ((a)+((a) < 0 ? -0.5 : 0.5)))

/* --Functions-- */

/* Utility functions */

void setprogname (const char *name);

void fatal (int code, const char *fmt, ...);
void warning (const char *fmt, ...);
void error (int code, const char *fmt, ...);

void fitsio_err (char *errstr, int status, const char *fmt, ...);
void report_err (char *errstr, const char *fmt, ...);
void report_syserr (char *errstr, const char *fmt, ...);

char *strtrim(char *str, const char *trim);
char *strrtrim(char *str, const char *trim);
char *strltrim(char *str, const char *trim);

/* In libc, but not the headers */

char *basename (const char *);

#endif /* __UTIL_H__ */
