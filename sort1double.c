#include <stdlib.h>

#include "mybls.h"

void sort1double (double *ia, long n) {
  long i, j, ii, jj, ifin;
  double it;

  jj = 4;
  while (jj < n) jj = 2 * jj;
  jj = MIN(n,(3 * jj)/4 - 1);
  while (jj > 1) {
    jj = jj/2;
    ifin = n - jj;
    for (ii = 0; ii < ifin; ii++) {
      i = ii;
      j = i + jj;
      if (ia[i] <= ia[j]) continue;
      it = ia[j];
      do {
	ia[j] = ia[i];
	j = i;
	i = i - jj;
	if (i < 0) break;
      } while (ia[i] > it);
      ia[j] = it;
    }
  }
}

