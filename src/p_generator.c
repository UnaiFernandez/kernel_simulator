#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "define_hariak.h"
#include "tree.h"


struct process_control_block atera;
struct node *root;
volatile int treetam = 0;
struct node *lefmost;
/*
 * Process generator funtzioa 
 */
void *process_generator(void *hari_par){

    int k, j, i, p_kop, p, tam;
    int lower = 0, upper = 40, minrt = 10, maxrt = 60;
    struct hari_param *param;
    struct process_control_block pcb;
    
    //Hasieraketak
    param = (struct hari_param *)hari_par;
    p_kop = param->p_kop;
    i = 0;
    p = 0;
    tam = p_kop;
    lefmost = root;
    //Hariaren hasierako informazioa pantailaratu
    printf("[PROCESS GENERATOR:       id = %d    name = %s   ]\n", param->id, param->name);

    //Funtzioko loop-a
    while(i <= p_kop){
    //while(1){
        //Ausazko zenbakiak sortzeko
        srand(tick*time(NULL));
        
        sem_wait(&semp);

        DEBUG_WRITE("[PROCESS GENERATOR] tick read! %d\n", tick);
        //Prozesu nulua sortu prozesu kopurura iristen bada.
        pcb.pid = rand() % 100;
        pcb.nice =  (rand() % (upper - lower + 1)) + lower;
        pcb.weight = weight[pcb.nice];     
        pcb.vruntime = (rand() % (maxrt - minrt + 1)) + minrt;
        pcb.decay_factor = (float)weight0/pcb.weight; 
        DEBUG_WRITE("[PROCESS GENERATOR] id: %d vruntime: %d \n", pcb.pid, pcb.weight);
        
        //Prozesu bat zuhaitzean sartu.
        if(pcb.pid != 83){
        if(root != NULL){
            insert(root, pcb);
            treetam++;
        }else{
            root = new_node(pcb);
            treetam++;
        }
        }
        i++;
        //inorder(root);
        //printf("\n");
 
        /*
        //Prozesuak array-ean sartu
        sch_arr[i] = pcb;
        */
    }
    pthread_exit(NULL);
}



