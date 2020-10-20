#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"



void *process_generator(void *hari_par){

    int t, id, proz_kop, num, i;
    struct hari_param *param;
    struct process_control_block pcb;

    param = (struct hari_param *)hari_par;
    printf("[PROCESS GENERATOR] id = %d    name = %s\n", param->id, param->name);
    sleep(1);
    t = tick;
    i = 0;
    proz_kop = param->p_kop;
    while(i < proz_kop){
    //while(1){
        srand(tick*time(NULL));
        /*sleep(1);
        id = fork();

        if(id == 0){
            sem_wait(&semp);
            pcb.pid = getpid();
            num = rand();
            printf("[PROCESS GENERATOR] Ni ume bat naiz, nire identifikatzailea %d da, eta lehentasuna %d da\n", pcb.pid, num);
            exit(0);
        }
        else if(id < 0){
            fprintf(stderr, "Errore bat gertatu da fork exekutatzean.\n");
            exit(1);
        }*/
        sem_wait(&semp);
        //printf("[PROCESS GENERATOR] tick read! %d\n", tick);
        if(tick == t+40){
            id = fork();

            if(id == 0){
                pcb.pid = getpid();
                num = rand()%140;
                printf("[PROCESS GENERATOR] Ni ume bat naiz, nire identifikatzailea %d da, eta lehentasuna %d da\n", pcb.pid, num);
                exit(0);
            }
            else if(id < 0){
                fprintf(stderr, "Errore bat gertatu da fork exekutatzean.\n");
                exit(1);
            }
            t = tick;
            i++;
        }
    }
    pthread_exit(NULL);
}
