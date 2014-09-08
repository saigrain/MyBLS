#include <stdlib.h>
#include "floatmath.h"
#include "util.h"

void dsolve (double a[25][25], double b[25], int m);

/* least-squares fit of order m polynomial to n data points */
void polynm (double *x, double *y, int n, double *a, int m)
{
  double alpha[25][25], beta[25];
  int i, j, k;

  /* clear arrays */
  for(i = 0; i < 25; i++) {
    beta[i] = 0.0;
    for(j = 0; j < 25; j++) alpha[i][j] = 0.0;
  }

  /* cumulate sums */
  for(i = 0; i < n; i++) {
    for(k = 0; k < m; k++) {
      beta[k] += y[i] * pow(x[i], (float) (k));
      for(j = 0; j <= k; j++) {
	alpha[j][k] += pow(x[i], (float) (k+j));
      }
    }
  }

  for(k = 1; k < m; k++) {
    for(j = 0; j < k; j++) alpha[k][j] = alpha[j][k];
  }

  /* solve linear equations */
  dsolve(alpha, beta, m);

  for(i = 0; i < m; i++) a[i] = beta[i];
}

/* least-squares fit of order m polynomial to n data points WITH WEIGHTS */
void polynm_w (float *x, float *y, float *s, int n, float *a, int m)
{
  double alpha[25][25], beta[25], weight;
  int i, j, k;

  /* clear arrays */
  for(i = 0; i < 25; i++) {
    beta[i] = 0.0;
    for(j = 0; j < 25; j++) alpha[i][j] = 0.0;
  }

  /* cumulate sums */
  for(i = 0; i < n; i++) {
    weight = 1.0 / s[i] / s[i];
    for(k = 0; k < m; k++) {
      beta[k] += y[i] * pow(x[i], (float) (k)) * weight;
      for(j = 0; j <= k; j++) {
	alpha[j][k] += pow(x[i], (float) (k+j)) * weight;
      }
    }
  }

  for(k = 1; k < m; k++) {
    for(j = 0; j < k; j++) alpha[k][j] = alpha[j][k];
  }

  /* solve linear equations */
  dsolve(alpha, beta, m);

  for(i = 0; i < m; i++) a[i] = beta[i];
}


/* gauss elimination to solve ax=b */

void dsolve (double a[25][25], double b[25], int m) {
  double temp, big, pivot, rmax;
  int i, iu, j, k, l = 0, jl, ib, ir;

  iu = m-1;
  for(i = 0; i < iu; i++) {
    big = 0.0;

    /* find largest remaining term in ith column for pivot */
    for(k = i; k < m; k++) {
      rmax = fabsf(a[i][k]);
      if(rmax > big) {
	big = rmax;
	l = k;
      }
    }

    /* check for non-zero term */
    if(big == 0.0) {
      for(ib = 0; ib < m; ib++) b[ib] = 0.0;
/*        fprintf(stderr, "solve: Zero determinant\n"); */
      return;
    }

    if(i != l) {
      /* switch rows */
      for(j = 0; j < m; j++) {
	temp    = a[j][i];
	a[j][i] = a[j][l];
	a[j][l] = temp;
      }
      temp = b[i];
      b[i] = b[l];
      b[l] = temp;
    }

    /* pivotal reduction */
    pivot = a[i][i];
    jl = i+1;

    for(j = jl; j < m; j++) {
      temp = a[i][j]/pivot;
      b[j] -= temp*b[i];
      for(k = i; k < m; k++) a[k][j] -= temp*a[k][i];
    }
  }

  /* back substitution for solution */
  for(i = 0; i < m; i++) {
    ir = m-1-i;
    if(a[ir][ir] != 0.0) {
      temp = b[ir];
      if(ir != m-1) {
	for(j = 1; j <= i; j++) {
	  k = m-j;
	  temp -= a[k][ir]*b[k];
	}
      }
      b[ir] = temp/a[ir][ir];
    }
    else
      b[ir] = 0.0;
  }
}
