CC=gcc
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
