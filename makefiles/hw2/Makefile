CC=g++
CFLAGS=-I./include

all: hw2.o utils.o
	$(CC) hw2.o utils.o -o rrt

hw2.o: ./src/hw2/hw2.cpp
	$(CC) -c $(CFLAGS) ./src/hw2/hw2.cpp

utils.o: ./src/hw2/utils.cpp
	$(CC) -c $(CFLAGS) ./src/hw2/utils.cpp

clean:
	rm -f *.o