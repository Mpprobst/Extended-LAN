CC=g++
CFLAGS= -Wall -g
OBJECTS = main.o Port.o Bridge.o Network.o

spanningtree: $(OBJECTS)
	$(CC) $(CFLAGS) -o spanningtree $(OBJECTS)

main.o: main.cpp Port.h Bridge.h Network.h
	$(CC) $(CFLAGS) -c main.cpp

Port.o: Port.h

Bridge.o: Bridge.h Port.h

Network.o: Network.h Bridge.h Port.h

clean: 
	-rm spanningtree $(OBJECTS)

run: spanningtree
	./spanningtree lanconfig.txt 1 4
