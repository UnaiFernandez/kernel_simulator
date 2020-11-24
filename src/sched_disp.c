#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"
#include "tree.h"


/*
 * Core guztietan prozesu gutxien dituenaren identifikadorea lortzeko.
 */
int minimum = 0;
int busy_arr[MAX_CORE_KOP];

int getCore(struct core_t *arr, int kop, int num){
    int id, i;
    for(i = 0; i < kop; i++){
        if(i == num){
            id=i;
        }
    }
    return id;
}

/*
 * Array bat 0 ekin hasieratzeko.
 */
void initArray(int *arr){
    int i;
    for(i = 0; i < MAX_CORE_KOP; i++){
        arr[i] = 0;
    }
}

pthread_mutex_t lock;

/*
 * Schedulerraren funtzioa.
 */
void *scheduler_dispatcher(void *hari_par){

    struct hari_param *param;
    param = (struct hari_param *)hari_par;
    int core_num, i1 = 1, vrunt, i, sch_tam, nextCore;
    struct process_control_block nulua, execdata;
    //struct core_t core;
    struct node *lag, *exec, *execold;

    //Hasieran sortutako schedulerraren informazioa
    printf("[SCHEDULER/DISPATCHER:    id = %d    name = %s]\n", param->id, param->name);
    sleep(1);
   

    //Hasieraketak
    core_num = param->id - 3;

    i = getCore(cpu.core, param->core_kop, core_num);
    nulua.pid = -1;
    nulua.weight = -1;
    nulua.vruntime = -1;

    cpu.core[i].core_num = core_num;
    cpu.core[i].root = root;
    cpu.core[i].busy = 0;
    sch_tam = sch_arr_tam;
    initArray(busy_arr);

    //Funtzioko loop-ean sartu, mutex baldintzatua erabiliz.
    pthread_mutex_lock(&mutex);
    while(1){
        done++;
        if(i1 == 1){
            i1 = 0;
        }else{
            //printf("nextcore = %d\n", nextCore);
            if(cpu.core[i].treetam >= 1 /*&& core.core_num == nextCore*/){
                if(cpu.core[i].busy != 1){
                    cpu.core[i].busy = 1;
                    cpu.core[i].exec = find_minimum(cpu.core[i].root);
                    busy_arr[cpu.core[i].core_num] = 1;
                }
                //lag = core.exec;
                execdata = cpu.core[i].exec->data;
                cpu.core[i].root = delete(cpu.core[i].root, cpu.core[i].exec->data);
                cpu.core[i].treetam--;
                printf("---------core%d---------    thread 1: [ id: %d vruntime: %d ]\n", cpu.core[i].core_num, execdata.pid, execdata.vruntime);
                
                vrunt =execdata.vruntime;
                vrunt = vrunt + (param->timer * execdata.decay_factor);
                execdata.vruntime = vrunt;
                execdata.rtime -= param->timer;
                execdata.egoera = 1;
                //inorder(cpu.core[i].root);

                if(execdata.rtime > 0){
                    if(cpu.core[i].root == NULL){
                        cpu.core[i].root = new_node(execdata);
                        cpu.core[i].treetam++;
                    }else{
                        insert(cpu.core[i].root, execdata);
                        cpu.core[i].treetam++;
                    }
                    cpu.core[i].busy = 0;
                }else{
                    cpu.core[i].busy = 0;
                }
                
                //Prozesuak amaitzen direnean, prozesu nulua sartzen du core barruan
                if(cpu.core[i].root == NULL){
                    cpu.core[i].root = new_node(nulua);
                    treetam++;
                }
            }else{
                cpu.core[i].root = new_node(nulua);
                printf("---------core%d---------    thread 1: [ id: %d vruntime: %d ]\n", cpu.core[i].core_num, nulua.pid, nulua.vruntime);
                treetam++;
            }
        }
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
