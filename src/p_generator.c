#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"
#include "rbtree.h"


int min = 0;


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

    initArray(tam_arr);
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
                min = getMin(tam_arr, param->core_kop);
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
            min = getMin(tam_arr, param->core_kop);
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



