#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"
#include "tree.h"


struct process_control_block atera;



void *process_generator(void *hari_par){

    int k, j, i, p_kop, p, tam;
    int lower = 0, upper = 40, minrt = 30, maxrt = 250;
    struct hari_param *param;
    struct process_control_block pcb;
    
    param = (struct hari_param *)hari_par;
    printf("[PROCESS GENERATOR:       id = %d    name = %s   ]\n", param->id, param->name);
    //sleep(1);
    //t = tick;
    p_kop = param->p_kop;
    i = 0;
    p = 0;
    tam = p_kop;

    while(i <= p_kop){
        srand(tick*time(NULL));
        
        sem_wait(&semp);

//            printf("[PROCESS GENERATOR] tick read! %d\n", tick); 
        if(i == p_kop){
            pcb.pid = 555;
            pcb.weight = 555;
            pcb.vruntime = 555;
        }else{
            pcb.pid = rand() % 100;
            pcb.weight = (rand() % (upper - lower + 1)) + lower;
            pcb.vruntime = (rand() % (maxrt - minrt + 1)) + minrt;
        }
        //printf("[PROCESS GENERATOR] id: %d vruntime: %d \n", pcb.pid, pcb.weight);
        sch_arr[i] = pcb;
        i++;
    }
    pthread_exit(NULL);
}



