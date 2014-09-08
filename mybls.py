
def mybls(time, flux, weight, \ # input data arrays
          pmin, pmax, \ # minimum & maximum period
          kmin, kmax, \ # minimum & maximum stellar radius (constraints duration range)
          sampling): # sampling parameter
    
#    figure out length of input arrays and store in variable nmeas

#    create output arrays period, statistic, dc, duration, epoch,
#    depth (length np, where np should be large and hardwired)

#    call myblc C function:
#      calling sequence:
#        mybls(time, flux, weight, nmeas, pmin, pmax, kmin, kmax, sampling,
#	       period, statistic, dc, duration, epoch, depth, &np)

#    on output, np is actual length of periodogram: discard the rest

  return period, statistic, dc, duration, epoch, depth
