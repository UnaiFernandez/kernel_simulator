#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"

volatile int timtick = 0;

void *timer(void *hari_par){

    struct hari_param *param;
    int t;
//    sem_t semt, semc;
    param = (struct hari_param *)hari_par;
    printf("[TIMER] id = %d    name = %s\n", param->id, param->name);
    sleep(1);
    //int tick;
    t = tick;

    //while(tick != t+param->timer){
    while(1){
        sem_wait(&semt);
        //sleep(param->maiz);
        //printf("[timer] tick: %d\n", tick);
        if(tick == t+param->timer){
        //    printf("[TIMER] denbora agortuta!\n");
            t = tick;
            sem_post(&sems);
        }
        //printf("[TIMER] tick read!\n");
    }

    printf("[TIMER] loop exit: %d\n", tick);
    t = tick;
    pthread_exit(NULL);
}
