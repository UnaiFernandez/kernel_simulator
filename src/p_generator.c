#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"
#include "rbtree.h"



void *process_generator(void *hari_par){

    int k, j, i, p_kop, p, tam;
    int lower = -20, upper = 15, minrt = 30, maxrt = 250;
    struct hari_param *param;
    struct process_control_block pcb, atera;
    
    
    param = (struct hari_param *)hari_par;
    printf("[PROCESS GENERATOR:       id = %d    name = %s   ]\n", param->id, param->name);
    //sleep(1);
    //t = tick;
    p_kop = param->p_kop;
    i = 0;
    p = 0;
    tam = TAM;
    while(i <= tam){
    //while(1){
        srand(tick*time(NULL));
        
        sem_wait(&semp);

        if(p <= p_kop){
//            printf("[PROCESS GENERATOR] tick read! %d\n", tick);
            pcb.pid = rand() % 100;
            pcb.weight = (rand() % (upper - lower + 1)) + lower;
            pcb.vruntime = (rand() % (maxrt - minrt + 1)) + minrt;
//            printf("[PROCESS GENERATOR] Ni ume bat naiz, nire identifikatzailea %d da, eta lehentasuna %d da\n", pcb.pid, pcb.lehen);
            sch_arr[i]=pcb;
        }
        if(p != p_kop){
            if(i == tam){
                for(k = 0; k < tam; k++){
                    sch_arr[k] = sch_arr[k+1]; 
                }
                atera = sch_arr[0];
                //printf("[PROCESS GENERATOR]: %d corera doa!\n", atera.pid);
                sem_wait(&sems);
                insertion(atera);
                i--;
            }
/*            printf("[ ");
            for(j = 0; j <= i; j++){
                printf("(%d, %d) ", sch_arr[j].pid, sch_arr[j].lehen);
            }
            printf("]\n");*/

        }else{
            while(tam != 0){
            for(k = 0; k < tam; k++){
                sch_arr[k] = sch_arr[k+1]; 
            }
            tam--;
            atera = sch_arr[0];
//            printf("[PROCESS GENERATOR]: %d corera doa!\n", atera.pid);
            sem_wait(&sems);
            insertion(atera);
/*            printf("[ ");
            for(j = 0; j <= tam; j++){
                printf("(%d, %d) ", sch_arr[j].pid, sch_arr[j].lehen);
            }
            printf("]\n");*/
            }
        }
        i++;
        p++;
    }
    //inorderTraversal(root);    
    //printf("\n");
    pthread_exit(NULL);
}


int getMin(int *arr, int kop){
    int i, min = 10000000;

    for(i = 0; i < kop; i++){
        if(arr[i] <= min){
            min = arr[i];
        }
    }
    return min;
}
