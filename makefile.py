# Makefile for bls
#

#### Constants (edit to suit your system) ####

# C compiler
CC = gcc

# Compiler flags
# CFLAGS = -g -O -Wall

# Linker flags
LIBS = -lgfortran -lm -lz

#### End constants section ####

BUILD_FLAGS = -fno-strict-aliasing -fno-common -dynamic -DNDEBUG -g -fwrapv -Os -Wall -Wstrict-prototypes -DENABLE_DTRACE -arch i386 -arch ppc -arch x86_64 -pipe
INCLUDE = -I/System/Library/Frameworks/Python.framework/Versions/2.6/Extras/lib/python/numpy/core/include/ -I/System/Library/Frameworks/Python.framework/Versions/2.6/include/python2.6
BUNDLE_FLAGS = -Wl,-F. -bundle -undefined dynamic_lookup -arch i386 -arch ppc -arch x86_64

# build object file
all:
	${CC} ${BUILD_FLAGS} ${INCLUDE} -c mybls.c
	${CC} ${BUILD_FLAGS} ${INCLUDE} -c sort1double.c
	${CC} ${BUILD_FLAGS} ${INCLUDE} -c polynm.c
	${CC} ${BUILD_FLAGS} ${INCLUDE} -c MyBLS_PY.c
	${CC} ${BUNDLE_FLAGS} ${INCLUDE} mybls.o sort1double.o polynm.o MyBLS_PY.o -o MyBLS_PY.so

clean:
	rm *.o
