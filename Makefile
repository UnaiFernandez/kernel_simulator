#SE makefile

CC = gcc
CFLAGS = -pthread
OBJ = binaries/main

all:
		$(CC) main.c sched_disp.c timer.c clock.c p_generator.c -o $(OBJ) $(CFLAGS)
exec:
		./binaries/main -p 10 -m 1
clean:
		rm -rf binaries/main
		rm -rf *.o

