#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"



void *process_generator(void *hari_par){

    int i, id, proz_kop, num;
    struct hari_param *param;
    struct process_control_block pcb;

    param = (struct hari_param *)hari_par;
    printf("[PROCESS GENERATOR] id = %d    name = %s\n", param->id, param->name);
    sleep(1);

    proz_kop = param->p_kop;
    while(1){
    //for(i = 0; i < proz_kop; i++){
        sem_wait(&semp);
        id = fork();

        if(id == 0){
            pcb.pid = getpid();
            num = rand()%8;
            //printf("[PROCESS GENERATOR] Ni ume bat naiz, nire identifikatzailea %d da, eta lehentasuna %d da\n", pcb.pid, num);
            exit(0);
        }
        else if(id < 0){
            fprintf(stderr, "Errore bat gertatu da fork exekutatzean.\n");
            exit(1);
        }
        //printf("[PROCESS GENERATOR] tick read!\n");
    }
    pthread_exit(NULL);
}
