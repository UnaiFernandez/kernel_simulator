#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"



void *scheduler_dispatcher(void *hari_par){

    struct hari_param *param;

    param = (struct hari_param *)hari_par;
    printf("[SCHEDULER/DISPATCHER] id = %d    name = %s\n", param->id, param->name);
    sleep(1);
    while(1){
//        sem_wait(&sems);
        done++;
        printf("[SCHEDULER/DISPATCHER %d] tick read!\n", param->id);
        //sem_post(&semt);
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex);
    }
    pthread_exit(NULL);
}
