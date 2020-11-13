#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "define_hariak.h"


volatile int tick = 0;

/*
 * Clock funztioa
 */
void *clockfunc(void *hari_par){

    struct hari_param *param;
    int t = 0;
    
    //Hasieraketak
    param = (struct hari_param *)hari_par;
    
    //Hasierako hariaren informazioa pantailaratu
    printf("[CLOCK:                   id = %d    name = %s               ]\n", param->id, param->name);
    sleep(1);

    //Funtzioko loop-a
    while(1){
        if(t < param->maiz){
            t++;
            //DEBUG_WRITE("t: %d\n", t);
        }else{
            t = 0;
            tick++;
            //DEBUG_WRITE("[CLOCK] tick: %d\n", tick);
            sem_post(&semt);
            sem_post(&semp);
        }
    }
    pthread_exit(NULL);
}


