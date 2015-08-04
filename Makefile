CC=g++
CFLAGS=-std=c++11 -Wall -pedantic#-g
all: main

.PHONY: clean

notes.o: notes.cpp notes.h
	$(CC) $(CFLAGS) -c notes.cpp -o notes.o
output.o: output.cpp output.h
	$(CC) $(CFLAGS) -c output.cpp -o output.o
melobeep.o: melobeep.cpp melobeep.h notes.h
	$(CC) $(CFLAGS) -c melobeep.cpp -o melobeep.o
main.o: main.cpp notes.h melobeep.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o
main: notes.o output.o melobeep.o main.o
	$(CC) $(CFLAGS) notes.o output.o melobeep.o main.o -o melobeep

clean:
	rm -f *.o