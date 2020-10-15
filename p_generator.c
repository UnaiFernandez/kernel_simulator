#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"



void *process_generator(void *hari_par){

    int i, id, proz_kop;
    struct hari_param *param;
    struct process_control_block pcb;

    param = (struct hari_param *)hari_par;
    printf("[PROCESS GENERATOR:]\n id = %d    name = %s\n", param->id, param->name);

    proz_kop = param->p_kop;
    for(i = 0; i < proz_kop; i++){
        id = fork();

        if(id == 0){
            pcb.pid = getpid();
            printf("[PROCESS GENERATOR] Ni ume bat naiz, nire identifikatzailea %d da\n", pcb.pid);
            exit(0);
        }
        else if(id < 0){
            fprintf(stderr, "Errore bat gertatu da fork exekutatzean.\n");
            exit(1);
        }
    }
    pthread_exit(NULL);
}
