#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"



void *scheduler_dispatcher(void *hari_par){

    struct hari_param *param;
    param = (struct hari_param *)hari_par;
    int core_num;

    printf("[SCHEDULER/DISPATCHER:    id = %d    name = %s]\n", param->id, param->name);
    //sleep(1);
   
    core_num = param->id - 3;

    pthread_mutex_lock(&mutex);
    while(1){
        done++;
        printf("---------core%d---------\n", core_num);
        printf("[SCHEDULER/DISPATCHER %d] tick read!\n", param->id);
        fflush(stdout);
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
