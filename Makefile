all: precolates
	./precolates
precolates: main.o
	g++  main.o -o precolates
main.o: main.cpp
	g++ -c main.cpp
clear:
	rm -f *.o