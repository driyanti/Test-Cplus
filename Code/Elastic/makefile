
CINCLUDE = -I.

CC       = g++
CFLAGS   = -O3 -Wall $(CINCLUDE)

OBJS     = elastic.o \
           solve.o

default: clean $(OBJS)
	$(CC) $(CFLAGS) -o disp $(OBJS)

elastic.o:
	$(CC) $(CFLAGS) -c elastic.cpp -o elastic.o

application.o:
	$(CC) $(CFLAGS) -c solve.cpp -o solve.o

clean:
	-rm -rf disp $(OBJS) *.out

