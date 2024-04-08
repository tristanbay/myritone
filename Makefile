CC=gcc

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
