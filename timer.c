#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "define_hariak.h"


void *timer(void *hari_par){

    struct hari_param *param;
    int t;
    param = (struct hari_param *)hari_par;
    printf("TIMER:\n id = %d    name = %s\n", param->id, param->name);
    //int tick;
    t = tick;
    printf("hlksjdhffjhagfhjga%d\n", t+3);
    while(tick != t+1){
        //sleep(1);
        printf("timer: %d\n", tick);
    }

    printf("sfhkaslhhsdfklhaf%d\n", t);
    pthread_exit(NULL);
}
