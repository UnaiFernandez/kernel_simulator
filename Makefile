#SE makefile

CC = gcc
CFLAGS = -pthread
OBJ = binaries/seso

all:
		$(CC) src/main.c src/sched_disp.c src/timer.c src/clock.c src/p_generator.c -o $(OBJ) $(CFLAGS)
exec:
		./binaries/seso -p 10 -m 10000000 -t 40
execf:
		./binaries/seso -p 10 -m 100000 -t 10 > binaries/file

clean:
		rm -rf binaries/seso
		rm -rf *.o

