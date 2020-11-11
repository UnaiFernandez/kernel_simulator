#SE makefile

CC = gcc
CFLAGS = -g -pthread
OBJ = binaries/seso

all:
		$(CC) src/main.c src/sched_disp.c src/timer.c src/clock.c src/p_generator.c src/rbtree.c -o $(OBJ) $(CFLAGS)
exec:
		./binaries/seso -p10 -m1 -t40 -c1
execomp:
		./binaries/seso -p 50 -m 1 -t 40 -c 4

clean:
		rm -rf binaries/seso
		rm -rf *.o

