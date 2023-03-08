CC=g++
CFLAGS=-I./include

all: hw3.o utils.o
	$(CC) hw3.o utils.o -o rrt

hw3.o: ./src/hw3/hw3.cpp
	$(CC) -c $(CFLAGS) ./src/hw3/hw3.cpp

utils.o: ./src/hw3/utils.cpp
	$(CC) -c $(CFLAGS) ./src/hw3/utils.cpp

clean:
	rm -f *.o