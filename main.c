/*
 * mybls: maximum likelihood transit search for corot LCs
 *        (designed to be called from python, this file is a dummy 
 *         wrapper for testing purposes)
 */

/*
 * h-files
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "mybls.h"

#define NDMAX 10000
#define NPMAX 10000

/* -- Main program -- */
int main(int argc, char *argv[])
{

  char *pt1, *pt2, line[80], *filename = "example.dat";
  FILE *inf = (FILE *) NULL;
  long nmeas, np = NPMAX;
  double time[NDMAX], flux[NDMAX], weight[NDMAX];
  double period[NPMAX], statistic[NPMAX], dc[NPMAX], duration[NPMAX], 
    epoch[NPMAX], depth[NPMAX];
  double stat = 0., per = 0., dep = 0., dcv = 0., ep = 0., dur = 0.;
  long ip = 0;

  /* read in example CoRoT LC */
  inf = fopen(filename, "r");
  nmeas = 0;
  while (fgets(line, 80, inf)) {
    if (nmeas >= NDMAX) {
      printf("mybls/main.c: Error: too many data points.\n");
    }
    time[nmeas] = strtod(line, &pt1);
    flux[nmeas] = (double) strtod(pt1, &pt2);
    weight[nmeas] = (double) strtod(pt2, NULL);
    weight[nmeas] = 1.0 / weight[nmeas] / weight[nmeas];
    nmeas++;
  }
  fclose(inf);

  /* Do search */
  mybls(time, flux, weight, nmeas, 1.0, 20.0, 0.5, 1.5, 1.0, 
	period, statistic, dc, duration, epoch, depth, &np);

  /* Find best values */
  for (ip = 0; ip < np; ip++) {
    if (statistic[ip] > stat) {
      stat = statistic[ip];
      per = period[ip];
      dcv = dc[ip];
      dur = duration[ip];
      ep = epoch[ip];
      dep = depth[ip];
    }
  }
  printf("Best fit:\nStat Per Epoch DC Depth Dur\n");
  printf("%.2f %.5f %.5f %.5f %.5f %.3f\n", stat, per, ep, dcv, dep, dur);
  
  //print test output
  //long i;
  //for(i=0;i<np;i++){
  //  printf("%5li %8lf %10lf %9lf %9lf %9lf %8lf\n", i, *(period+i), *(statistic+i), *(dc+i), *(duration+i), *(epoch+i), *(depth+i));
  //  }
  
  /* End */
  return(0);

}


