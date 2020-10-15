#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"

void *timer(void *hari_par){

    struct hari_param *param;
    int t;
//    sem_t semt, semc;
    param = (struct hari_param *)hari_par;
    printf("[TIMER:]\n id = %d    name = %s\n", param->id, param->name);
    //int tick;
    t = tick;

    //while(tick != t+param->timer){
    while(1){
        //sleep(param->maiz);
        //printf("timer: %d\n", tick);
        sem_wait(&semc);
        if(tick == t+param->timer){
            printf("[TIMER] denbora agortuta!\n");
            t = tick;
        }
        sem_post(&semt);
    }

    printf("[TIMER] loop exit: %d\n", tick);
    t = tick;
    pthread_exit(NULL);
}
