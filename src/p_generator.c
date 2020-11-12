#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"
#include "tree.h"


int minimum = 0;
int tam_arr[MAX_CORE_KOP];
struct process_control_block atera;

int getMin(int *arr, int kop){
    int i, id, min = 10000000;

    for(i = 0; i < kop; i++){
        if(arr[i] <= min){
            min = arr[i];
            id=i;
        }
    }
    return id;
}

void initArray(int *arr){
    int i;
    for(i = 0; i < MAX_CORE_KOP; i++){
        arr[i] = 0;
    }
}

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

    initArray(tam_arr);
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
        atera = pcb;
        //minimum = rand()%param->core_kop;
        minimum = getMin(tam_arr, param->core_kop);
        i++;
    }
    pthread_exit(NULL);
}



