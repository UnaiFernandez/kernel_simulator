#SE makefile


CC = gcc
CFLAGS = -g -pthread -ggdb
OBJ = binaries/seso

all:
		$(CC) src/main.c src/sched_disp.c src/timer.c src/clock.c src/p_generator.c src/tree.c -o $(OBJ) $(CFLAGS)
exec1:
		./binaries/seso -p2 -m10 -t40 -c1
exec4:
		./binaries/seso -p4 -m100 -t4 -c1

clean:
		rm -rf binaries/seso
		rm -rf *.o

