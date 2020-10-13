#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//#include <time.h>

#include "define_hariak.h"


volatile int tick = 3;

void *clockfunc(void *hari_par){

    struct hari_param *param;
    param = (struct hari_param *)hari_par;
    printf("CLOCK:\n id = %d    name = %s\n", param->id, param->name);
    while(tick < 10){
        sleep(param->maiz);
        tick++;
        printf("%d\n", tick);
    }
    pthread_exit(NULL);
}
