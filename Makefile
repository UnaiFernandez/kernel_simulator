#SE makefile


CC = gcc
CFLAGS = -g -pthread -ggdb -lm
OBJ = binaries/seso

all:
		$(CC) src/main.c src/sched_disp.c src/timer.c src/clock.c src/loader.c src/tree.c -o $(OBJ) $(CFLAGS)
exec1:
		./binaries/seso -p1 -m40 -t5 -c1
exec2:
		./binaries/seso -p2 -m40 -t5 -c2
exec4:
		./binaries/seso -p4 -m40 -t5 -c1
exec10:
		./binaries/seso -p10 -m10 -t40 -c10

clean:
		rm -rf binaries/seso
		rm -rf *.o
