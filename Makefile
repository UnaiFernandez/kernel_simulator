#SE makefile


CC = gcc
CFLAGS = -g -pthread
OBJ = binaries/seso

all:
		$(CC) src/main.c src/sched_disp.c src/timer.c src/clock.c src/p_generator.c src/tree.c -o $(OBJ) $(CFLAGS)
exec1:
		./binaries/seso -p4 -m1 -t40 -c1
exec4:
		./binaries/seso -p10 -m10 -t 40 -c4

clean:
		rm -rf binaries/seso
		rm -rf *.o

