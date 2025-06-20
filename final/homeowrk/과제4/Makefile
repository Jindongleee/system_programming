CC = gcc
CFLAGS = -Wall

all: terminal1 terminal2

terminal1: terminal1.c common.h
	$(CC) $(CFLAGS) -o terminal1 terminal1.c

terminal2: terminal2.c common.h
	$(CC) $(CFLAGS) -o terminal2 terminal2.c

clean:
	rm -f terminal1 terminal2 