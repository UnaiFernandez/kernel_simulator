#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"



void *process_generator(void *hari_par){

    int t, j, id, proz_kop, num, i;
    struct hari_param *param;
    struct process_control_block pcb;

    param = (struct hari_param *)hari_par;
    printf("[PROCESS GENERATOR] id = %d    name = %s\n", param->id, param->name);
    sleep(1);
    t = tick;
    i = 0;
    proz_kop = param->p_kop;
    while(i < TAM){
    //while(1){
        srand(tick*time(NULL));
        sem_wait(&semp);
        //printf("[PROCESS GENERATOR] tick read! %d\n", tick);
        pcb.pid = rand() % 100;
        pcb.lehen = rand()%140;
        printf("[PROCESS GENERATOR] Ni ume bat naiz, nire identifikatzailea %d da, eta lehentasuna %d da\n", pcb.pid, pcb.lehen);
        sch_arr[i]=pcb;
        printf("[ ");
        for(j = 0; j < i; j++){
            printf("(%d, %d) ", sch_arr[j].pid, sch_arr[j].lehen);
        }
        printf("]\n");
        i++;
    }
    pthread_exit(NULL);
}
