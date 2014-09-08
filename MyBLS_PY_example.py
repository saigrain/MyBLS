#!/usr/bin/python

from MyBLS_PY import MyBLS
import numpy as np

print "MyBLS test script..."

#read in example CoRoT LC
time,flux,weight = np.loadtxt("example.dat",unpack=True)
weight = 1. / weight**2

#call MyBLS wrapper function
period,statistic,dc,duration,epoch,depth = MyBLS(time,flux,weight,1.0,20.0,0.5,1.5,\
  1.0)
print min(statistic), max(statistic)
#find best values
ind = np.where(statistic == statistic.max())
print "Best fit:\nStat Per Epoch DC Depth Dur"
print "%.2f %.5f %.5f %.5f %.5f %.3f" % \
  (statistic[ind],period[ind],epoch[ind],dc[ind],depth[ind],duration[ind])

#print test output
#for i in range(period.size):
#  print "%5i %8f %10f %9f %9f %9f %8f" % (i,period[i],statistic[i],dc[i],duration[i],epoch[i],depth[i])
