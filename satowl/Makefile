CC=g++
CFLAGS=-c -g -std=c++11

all: satowl

satowl: satowl.o
	$(CC) satowl.o -o satowl -static -L../sgp4 -lsgp4

satowl.o: satowl.cpp
	$(CC) $(CFLAGS) satowl.cpp

clean:
	rm -f *o satowl