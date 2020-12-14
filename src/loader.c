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

void deleteword(char *str){
    int i, j = 6;

    for(i = 0; i < 8; i++){
       str[i] = str[j]; 
    } 
}

/*
 * Process generator funtzioa
 */
void *loader(void *hari_par){

    int j, i, p_kop, p, tam, lnum, kont;
    int lower = 0, upper = 40, minrt = 30, maxrt = 250;
    struct hari_param *param;
    struct process_control_block pcb;
    FILE *fp;
    char line[16];

    //Hasieraketak
    param = (struct hari_param *)hari_par;
    p_kop = param->p_kop;
    i = 0;
    p = 0;
    tam = p_kop;
    lefmost = root;
    lnum = 0;
    kont = 0;
    //Hariaren hasierako informazioa pantailaratu
    printf("[PROCESS GENERATOR:       id = %d    name = %s   ]\n", param->id, param->name);

    //Funtzioko loop-a
    while(j < p_kop){
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
        pcb.rtime = (rand() % (maxrt - minrt + 1)) + minrt;
        pcb.decay_factor = (float)weight0/pcb.weight;
        
        
        fp = fopen("prog000.elf", "r");
        if(fp == NULL)
            printf("No such file or directory\n");
        while(fgets(line, sizeof(line), fp)!=NULL)
	    {
            if(lnum == 0){
                deleteword(line);
                pcb.mm.text = (int)strtol(line, NULL, 16);
                printf(".text = %08X\n", pcb.mm.text);
            }else if(lnum == 1){
                deleteword(line);
                pcb.mm.data = (int)strtol(line, NULL, 16);
                printf(".data = %08X\n", pcb.mm.data);
            }else{
                //printf("line = %s", line);
                //printf("char = %c\n", line[7]);
                int num = (int)strtol(line, NULL, 16);
                printf("0x%06X [%08X]\n", kont, num);
                kont+=4;
            }    
            lnum++;
	    }
        lnum = 0;
        kont = 0;
	    fclose(fp);
        //pcb.mm.text = fget(line, 30, fp);
        //printf(".text = %x\n", pcb.mm.text);
        DEBUG_WRITE("[PROCESS GENERATOR] id: %d vruntime: %d \n", pcb.pid, pcb.weight);

        //Prozesu bat dagokion zuhaitzean sartu.
        //if(pcb.pid != 83){
            if(cpu.core[i].root != NULL){
                insert(cpu.core[i].root, pcb);
                cpu.core[i].treetam++;
            }else{
                cpu.core[i].root = new_node(pcb);
                cpu.core[i].treetam++;
            }
        //}
        i = (i + 1) % param->core_kop;
        j++;
    }
    /*for(int k = 0; k < param->core_kop; k++){
        printf("core%d:", k);
        inorder(cpu.core[k].root);
        printf("\n");
    }*/

    pthread_exit(NULL);
}
