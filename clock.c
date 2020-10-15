#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//#include <time.h>
#include "define_hariak.h"


volatile int tick = 0;

void *clockfunc(void *hari_par){

    sem_t sem1;
    struct hari_param *param;
    int t = 0;
    
    sem_init(&sem1, 0, 1);
    param = (struct hari_param *)hari_par;
    printf("CLOCK:\n id = %d    name = %s\n", param->id, param->name);
    while(1){
        //sleep(1);
        sem_wait(&sem1);
        if(t < param->maiz){
            t++;
            //printf("t: %d\n", t);
        }else{
            t = 0;
            tick++;
            printf("tick: %d\n", tick);
        }
        sem_post(&sem1);
    }
    sem_destroy(&sem1);
    printf("sem destroyed\n");
    pthread_exit(NULL);
}
