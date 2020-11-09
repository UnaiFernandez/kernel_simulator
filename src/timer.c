#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"

volatile int timtick = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int done = 0;

void *timer(void *hari_par){

    struct hari_param *param;
    int t;
    param = (struct hari_param *)hari_par;
    printf("[TIMER:                   id = %d    name = %s               ]\n", param->id, param->name);
    //sleep(1);
    t = tick;

    while(1){
        sem_wait(&semt);
//        printf("[TIMER] tick read!\n");
//        printf("[timer] tick: %d\n", tick);
        if(tick == t+param->timer){
            printf("\n");
            pthread_mutex_lock(&mutex);
            printf("[TIMER] seinalea bidalita\n");
            fflush(stdout);
            while(done < param->core_kop)
                pthread_cond_wait(&cond, &mutex);
            t = tick;
            printf("\n");
            done = 0;
            pthread_cond_broadcast(&cond2);
            pthread_mutex_unlock(&mutex);
//            for (i = 0; i < param->core_kop; i++)
//                sem_post(&sems);
        }
    }
    t = tick;
    pthread_exit(NULL);
}
