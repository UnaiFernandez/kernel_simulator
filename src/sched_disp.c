#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"
#include "tree.h"


int minimum = 0;
int tam_arr[MAX_CORE_KOP];

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

void *scheduler_dispatcher(void *hari_par){

    struct hari_param *param;
    param = (struct hari_param *)hari_par;
    int core_num, i1 = 1, weightval, vrunt, i, sch_tam;
    struct process_control_block nulua;
    struct core_t core;
    struct node *root, *exec, *new;

    printf("[SCHEDULER/DISPATCHER:    id = %d    name = %s]\n", param->id, param->name);
    sleep(1);
   
    core_num = param->id - 3;

    nulua.pid = -1;
    nulua.weight = -1;
    nulua.vruntime = -1;

    core.core_num = core_num;
    core.root = root;
    core.tamaina = 0;
    sch_tam = sch_arr_tam;
    initArray(tam_arr);
    pthread_mutex_lock(&mutex);
    while(1){
        done++;
        if(i1 == 1){
            i1 = 0;
        }else{
            if(sch_arr[0].pid != 555 && minimum == core_num){
                if(core.tamaina == 0){
                    root = new_node(sch_arr[0]);
                    core.tamaina++;
                }else{
                    insert(root, sch_arr[0]);
                    core.tamaina++;
                }
                tam_arr[core_num] = core.tamaina;
                for(i = 0; i < sch_tam; i++){
                    sch_arr[i] = sch_arr[i+1];
                    //printf("%d ", sch_arr[i].pid);
                }
                sch_tam--;
                minimum = getMin(tam_arr, param->core_kop);
            }
            //printf("[SCHEDULER/DISPATCHER %d] tick read!\n", param->id);
            printf("---------core%d---------    thread 1: [ id: %d vruntime: %d ]\n", core_num, sch_arr[0].pid, sch_arr[0].vruntime);
            //fflush(stdout);
            inorder(root);
            printf("\n");
        }
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
