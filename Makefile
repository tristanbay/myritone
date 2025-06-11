CC=gcc
<<<<<<< HEAD

IDIR=./include
ODIR=./build
SDIR=./src
CFLAGS=-I $(IDIR) -g -Wall

LIBS=-lm

_DEPS=main.h myritone.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o fileread.o output.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c $< $(CFLAGS) -o $@

myritone: $(OBJ)
	$(CC) $^ $(CFLAGS) $(LIBS) -o $(ODIR)/$@

.PHONY: clean

clean:
	rm -f $(ODIR)/*
=======
INCL=./include
BLD=./build
SRC=./src
CFLAGS=-I $(INCL) -g -Wall
LIBS=-lm
PROG=myritone

all:
	$(CC) $(SRC)/*.c $(CFLAGS) $(LIBS) -o $(BLD)/$(PROG)

clean:
	rm -f $(BLD)/*

.PHONY: clean
>>>>>>> 7f590de (simplify makefile)
