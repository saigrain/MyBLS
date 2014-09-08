#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "mybls.h"
#include "floatmath.h"

extern void sort1double (double *ia, long n);

int mono (double *time, double *wflux, double *weight, long ndata, 
	  double hw, double dep, long nep, 
	  double *wsum, double *ywsum);

int mybls (double *time, double *flux, double *weight, long ndata,
	   float pmin, float pmax, float kmin, float kmax, 
	   float sampling, double *period, double *statistic, 
	   double *dc, double *duration, double *epoch, double *depth, 
	   long *np) {
  
  double *wflux = (double *) NULL; /* To hold weighted flux */
  double med; /* Median flux */
  double phwc; /* Constant to convert period & duration to min. star radius */
  long nhw, nep, nper; /* Number of trial half-widths, epochs and periods */
  double hwmin, hwmin2, hwmax, hwmax2, dhw, hw; /* Trial half-widths */
  double trange, trangexp, dep; /* Trial epochs */
  double dfreq, freqmin, freqmax, lfreqmin, lfreqmax, freq, per; /* Trial periods */
  long idata, ihw, ip_dep, ipmin, ipmax, ip,
    nfp, ifp, iarg, ie; /* Indices */
  double *wsum = (double *) NULL, *ywsum = (double *) NULL, sumw, sumyw,
    *sumw_ = (double *) NULL, *sumyw_ = (double *) NULL; /* Mono sums */
  double stat, stat_p, depth_p, epoch_p; /* Epoch search */

  /* Compute light curve median, weighted data arrays and initial chi2 */
  wflux = (double *) malloc( sizeof(double) * ndata );
  for (idata = 0; idata < ndata; idata++) {
    wflux[idata] = flux[idata]; 
  }
  sort1double(wflux, ndata);
  med = wflux[(ndata+1)/2];
  for (idata = 0; idata < ndata; idata++) {
    wflux[idata] = (flux[idata] - med) * weight[idata];
  }

  /* Set the half-width sampling */
  phwc = pow(4 / PI_CONST / G_CONST / MSUN_CONST, 1.0 / 3.0) * 
    RSUN_CONST * pow(DAY_TO_SEC, -2.0 / 3.0);
  nhw = (long) (((double) DEFAULT_NHW) * sampling);
  hwmax = pow(pmax, 1.0 / 3.0) * kmax * phwc / 2.;
  hwmax2 = pow(pmin, 1.0 / 3.0) * kmax * phwc / 2.;  
  hwmin = pow(pmin, 1.0 / 3.0) * kmin * phwc / 2.;
  hwmin2 = pow(pmax, 1.0 / 3.0) * kmin * phwc / 2.;
  dhw = MIN((hwmax - hwmin2), (hwmax2-hwmin)) / ((double) (nhw-1));
  nhw = MAX((long) ((hwmax - hwmin) / dhw) + 1, 1);

  /* Set epoch sampling */
  dep = (double) (hwmin * pmin / sampling / 2.0); 
  trange = time[ndata-1] - time[0];
  trangexp = trange + 2 * (hwmax + dep);
  nep = (long) (trangexp / dep) + 1;

  /* Set period sampling and initialise arrays to hold periodogram */
  dfreq = (double) (hwmin / trange / sampling);
  freqmin = (double) (1.0 / pmax);
  freqmax = (double) (1.0 / pmin);
  nper = (long) ((freqmax - freqmin) / dfreq) + 1;
  if (nper > *np) {
    printf("mybls/mybls.c: Error: too many trial periods.\n");
  }
  *np = nper;
  for(ip = 0; ip < nper; ip++) {
    freq = freqmin +  ((double) ip) * dfreq;
    period[ip] = 1.0 / freq;
    statistic[ip] = 0.0;
    duration[ip] = 0.0;
    depth[ip] = 0.0;
    epoch[ip] = 0.0;
    dc[ip] = med;
  }

  /* Allocate arrays to hold single epoch search output */  
  wsum = (double *) malloc(sizeof(double) * nep);
  ywsum = (double *) malloc(sizeof(double) * nep);
  ip_dep = (long) (pmax / dep) + 1;
  sumw_ = (double *) malloc(sizeof(double) * ip_dep);
  sumyw_ = (double *) malloc(sizeof(double) * ip_dep);

  /* Loop over trial half-widths */
  for(ihw = 0; ihw < nhw; ihw++) {
    hw = hwmin + ((double) ihw) * dhw;

    /* Work out the range of periods that give a value of k in the
       desired range */
    lfreqmax = (double) (pow(2 * hw / kmax / phwc, -3.0));
    lfreqmin = (double) (pow(2 * hw / kmin / phwc, -3.0));
    ipmin = MAX(0, (long) ((lfreqmin - freqmin) / dfreq));
    ipmax = MIN(nper-1, (long) ((lfreqmax - freqmin) / dfreq));

    /* Reset dep to appropriate value */
    dep = (double) (hw * pmin / sampling / 2.0);
 
    /* Single event search */
    mono(time, wflux, weight, ndata, hw, dep, nep, wsum, ywsum);

    /* Loop over the trial trial periods */
    for(ip = ipmin; ip < ipmax; ip++) {
      per = period[ip];
      ip_dep = (long) (per / dep) + 1; /* period in dep units */ 
      nfp = (long) (trangexp / per); /* number of integer periods in LC */

      /* Find best epoch */
      stat_p = 0.;
      epoch_p = 0;
      depth_p = 0;
      for (ie = 0; ie < ip_dep; ie++) {
	sumw = 0.;
	sumyw = 0.;
	for(ifp = 0; ifp <= nfp; ifp++) {
	  iarg = (long) ((((double) ifp) * per + ((double) ie) * dep) / dep);
	  if (iarg >= nep) continue;
	  sumw += wsum[iarg];
	  sumyw += ywsum[iarg];
	}
	if(((sumw) > EPS) && ((sumyw) > EPS)) {
	  stat = sumyw / sqrt(sumw);
	} else {
	  stat = 0.0;
	}
	if(stat > stat_p) { /* Best epoch so far? */
	  stat_p = stat;
	  depth_p = sumyw / sumw;
	  epoch_p = ie * dep - hw;
	}
      }
      if (stat_p > statistic[ip]) { /* Best at this period so far? */
	statistic[ip] = stat_p;
	depth[ip] = depth_p;
	epoch[ip] = epoch_p + time[0] - hw;
	duration[ip] = 2 * hw;
      }
    } /* End period loop */
  } /* End half-width loop */

  /* Free memory */
  if (wflux) free((void *) wflux);
  if (wsum) free((void *) wsum);
  if (ywsum) free((void *) ywsum);
  if (sumw_) free((void *) sumw_);
  if (sumyw_) free((void *) sumyw_);

  return(0);
 
}

int mono (double *time, double *wflux, double *weight, long ndata, 
	  double hw, double dep, long nep, 
	  double *wsum, double *ywsum) {

  double epoch, t0, t1, t2, wsum_, ywsum_;
  long i1, i2, oldi1, oldi2, idata, ie, nsum;

  /* Initialise arrays */
  for(ie = 0; ie < nep; ie++) {
    wsum[ie] = 0.0;
    ywsum[ie] = 0.0;
  }

  /* Initialise counters and in-eclipse sums */
  ie = 0;
  i1 = 0;
  i2 = -1;
  oldi1 = i1;
  oldi2 = i2;
  nsum = 0;
  wsum_ = 0.0;
  ywsum_ = 0.0;
  t0 = time[0] - hw;
  epoch = t0;
  
  /* Epoch loop */
  while(epoch < time[ndata-1] + hw ) {
    t1 = epoch - hw;
    t2 = epoch + hw;
    while((i1 < ndata) & (time[i1] <= t1)) i1++;
    while((i2 < ndata-2) & (time[i2+1] < t2)) i2++;

    /* printf("%f %f %f %ld %ld\n", epoch, t1, t2, i1, i2); */

    if ((i2 < i1) && (nsum == 0)) { /* No in-eclipse points */
      /* Move to just before the next data point */
      epoch = MAX(epoch + dep, dep * ((long) ((time[i1] - hw) / dep)));
      /* printf("Skipping to epoch %f\n", epoch); */
      continue;
    }

    /* Any changes to in-eclipse sums? */
    if (i1 > oldi1) {
      for (idata = oldi1; idata < i1; idata++) {
	ywsum_ -= wflux[idata];
	wsum_ -= weight[idata];
	nsum--;
      }
    }
    if (i2 > oldi2) {
      for (idata = oldi2+1; idata <= i2; idata++) {
	ywsum_ += wflux[idata];
	wsum_ += weight[idata];
	nsum++;
      }
    }

    /* printf("%f %f %ld\n", ywsum_, wsum_, nsum); */

    /* Store */
    if (nsum > 0) {
      ie = (long) ((epoch + hw - t0) / dep);
      /* printf("%f %f %ld\n", epoch + hw - t0, dep, ie); */
      if (ie >= nep) printf("mybls.c/mono - Error: ie > nep\n");
      ywsum[ie] = ywsum_;
      wsum[ie] = wsum_;
    }
    
    /* Store indices */
    oldi1 = i1;
    oldi2 = i2;

    /* Move eclipse forwards */
    epoch += dep;

  } /* End epoch loop */

  return(0);

}
  
