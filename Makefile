#SE makefile

CC = gcc
CFLAGS = -pthread
OBJ = binaries/main

all:
		$(CC) main.c sched_disp.c timer.c clock.c p_generator.c -o $(OBJ) $(CFLAGS)
exec:
		./binaries/main -p 10 -m 10000000 -t 40
execf:
		./binaries/main -p 10 -m 100000 -t 10 > binaries/file

clean:
		rm -rf binaries/main
		rm -rf *.o

