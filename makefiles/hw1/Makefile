CC=g++
CFLAGS=-I./include

all: simple_graph_search.o
	$(CC) simple_graph_search.o -o srch

simple_graph_search.o: ./src/hw1/simple_graph_search.cpp
	$(CC) -c $(CFLAGS) ./src/hw1/simple_graph_search.cpp

clean:
	rm -f *.o