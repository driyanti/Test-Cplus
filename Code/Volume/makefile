
CINCLUDE = -I.

CC       = g++
CFLAGS   = -O3 -Wall $(CINCLUDE)

OBJA     = application.o \
           horizon.o

default: clean $(OBJA) main.o
	$(CC) $(CFLAGS) -o volume main.o $(OBJA)

main.o:
	$(CC) $(CFLAGS) -c main.cpp -o main.o

application.o:
	$(CC) $(CFLAGS) -c application.cpp -o application.o
horizon.o:
	$(CC) $(CFLAGS) -c horizon.cpp -o horizon.o

clean:
	-rm -rf volume main.o $(OBJA)
	

