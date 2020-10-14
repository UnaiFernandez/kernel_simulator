#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"


void *timer(void *hari_par){

    struct hari_param *param;
    int t;
    param = (struct hari_param *)hari_par;
    printf("TIMER:\n id = %d    name = %s\n", param->id, param->name);
    //int tick;
    t = tick;

    while(tick != t+param->timer){
        sleep(1);
        printf("timer: %d\n", tick);
        if(tick == t+param->timer){
            printf("denbora agortuta!\n");
            t = tick;
        }
    }

    printf("loop exit: %d\n", tick);
    t = tick;
    pthread_exit(NULL);
}
