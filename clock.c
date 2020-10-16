#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//#include <time.h>
#include "define_hariak.h"


volatile int tick = 0;
//sem_t semt, semc;
void *clockfunc(void *hari_par){

    struct hari_param *param;
    int t = 0;
    
    param = (struct hari_param *)hari_par;
    printf("[CLOCK]:\n id = %d    name = %s\n", param->id, param->name);
    while(1){
        //sleep(1);
        if(t < param->maiz){
            t++;
            //printf("t: %d\n", t);
        }else{
            sem_wait(&semt);
            t = 0;
            tick++;
            printf("[CLOCK] tick: %d\n", tick);
            sem_post(&semc);
        }
    }
        printf("sem destroyed\n");
    pthread_exit(NULL);
}


