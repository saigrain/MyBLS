# Makefile for bls
#

#### Constants (edit to suit your system) ####

# C compiler
CC = gcc

# Compiler flags
CFLAGS = -g -O -Wall

# Linker flags
LIBS = -lm -lz

#### End constants section ####

SRCS = main.c mybls.c sort1double.c

OBJS=${SRCS:%.c=%.o}

# Rules for building C

.SUFFIXES: .c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: bls

# Rules for bls

bls: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

# Clean:
	rm -f $(OBJS)
	rm -f *.core *~
