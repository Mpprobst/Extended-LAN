CC=g++
CFLAGS= -Wall -g

main: main.o Port.o Bridge.o Network.o
	$(CC) $(CFLAGS) -o main main.o Port.o Bridge.o Network.o

main.o: main.cpp Port.h Bridge.h Network.h
	$(CC) $(CFLAGS) -c main.cpp

Port.o: Port.h

Bridge.o: Bridge.h Port.h

Network.o: Network.h Bridge.h Port.h
