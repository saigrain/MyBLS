#ifndef __MYBLS_H__
#define __MYBLS_H__

/* -- Constants -- */

/* Precision */
#define EPS 1e-15

/* Default parameters, tunable at command line */
#define DEFAULT_NHW 3 /* Controls trial duration sampling */

/* Parameters controling transit search */
#define POLYNM_ORDER 2 /* Order of polynomial to fit to periodogram */

/* Astro constants */
#define G_CONST 6.67e-11
#define PI_CONST 3.1415927
#define DAY_TO_SEC 86400.0
#define HOUR_TO_SEC 3600.0
#define DAY_TO_HOUR 24.0
#define MSUN_CONST 1.989e30 
#define RSUN_CONST 6.95e8 

/* -- Macros -- */

#undef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#undef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* -- Subroutines -- */
int mybls (double *time, double *flux, double *weight, long ndata,
	   float pmin, float pmax, float kmin, float kmax, 
	   float sampling, double *period, double *statistic, 
	   double *dc, double *duration, double *epoch, double *depth, 
	   long *np);

#endif  /* __MYBLS_H__ */
