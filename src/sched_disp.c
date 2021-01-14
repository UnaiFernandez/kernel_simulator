#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "define_hariak.h"
#include "tree.h"


/*
 * Core guztietan prozesu gutxien dituenaren identifikadorea lortzeko.
 */
int minimum = 0;
int busy_arr[MAX_CORE_KOP];
int ir;
int pc;
int ptbr;

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
* MMU-aren funtzioa
*/
int mmu_function(int vaddr, int ptbr){
    char helblog[7], orrz[6], desp[2];
    int i, j, k, orrizenb, markozenb, desplaz, haddr;


    //Lortu orri-zenbakia eta desplazamendua
    sprintf(helblog, "%06X", vaddr);
    sprintf(orrz, "%c%c%c%c%c", helblog[0], helblog[1], helblog[2], helblog[3], helblog[4]);
    sprintf(desp, "%c", helblog[5]);
    orrz[5] = '\0';
    desp[1] = '\0';

    orrizenb = (int)strtol(orrz, NULL, 16);
    desplaz = (int)strtol(desp, NULL, 16);
    DEBUG_WRITE("orri-zenbakia:%05X\n", orrizenb);
    DEBUG_WRITE("desplazamendua:%01X\n", desplaz);

    markozenb = mem[ptbr+orrizenb*4];
    DEBUG_WRITE("markozenbakia:%06X\n", markozenb);
    haddr = markozenb + desplaz;

    return haddr;
}

/*
 * Schedulerraren funtzioa.
 */
void *scheduler_dispatcher(void *hari_par){

    struct hari_param *param;
    param = (struct hari_param *)hari_par;
    int core_num, i1 = 1, vrunt, i, sch_tam, nextCore, haddr, dataaddr, ldataaddr, erreg;
    char execcom[9], ldataaddr_str[7];
    char *com, *reg;
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
    //cpu.core[i].root = root;
    cpu.core[i].busy = 0;
    cpu.core[i].pc = 0;
    //sch_tam = sch_arr_tam;
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
                /*for(int k = 0; k < param->core_kop; k++){
                    printf("core%d:", k);
                    inorder(cpu.core[k].root);
                    printf("\n");
                }*/
                cpu.core[i].treetam--;
                cpu.core[i].pc = execdata.pc;

                DEBUG_WRITE("ptbr sch: %06X\n", execdata.ptbr);
                haddr = mmu_function(cpu.core[i].pc, execdata.ptbr);

                sprintf(execcom, "%08X", mem[haddr]);

                com = getcommand(execcom[0]);
                reg = getregister(execcom[1]);
                erreg = (int) execcom[1];
                sprintf(ldataaddr_str, "%c%c%c%c%c%c", execcom[2], execcom[3], execcom[4], execcom[5], execcom[6], execcom[7]);
                ldataaddr = (int) strtol(ldataaddr_str, NULL, 16);
                if(strstr(com, "ld") != NULL){
                      dataaddr = mmu_function(ldataaddr, execdata.ptbr);
                      //printf("+++++++++++++++++++++++++++++%08X  %d\n", mem[dataaddr], mem[dataaddr]);
                      execdata.err[erreg] = mem[dataaddr];
                      printf("╔════════════════════════════════════════════════════════════════╗\n CORE %d:\n process id: %d            PC: 0x%06X           data: [%08X]\n \n Command:\n %s   %s, %d\n |\n  -->  %s = %d\n╚════════════════════════════════════════════════════════════════╝\n", cpu.core[i].core_num, execdata.pid, cpu.core[i].pc, mem[haddr], com, getregister(execcom[1]), mem[dataaddr], getregister(execcom[1]), mem[dataaddr]);
                }else if(strstr(com, "add") != NULL){
                      int r1, r2, r3, err1, err2, err3;
                      err1 = (int) execcom[1];
                      err2 = (int) execcom[2];
                      err3 = (int) execcom[3];
                      r2 = execdata.err[err2];
                      r3 = execdata.err[err3];
                      r1 = r2 + r3;
                      execdata.err[err1] = r1;
                      printf("[Batuketa]  (%s)%d + (%s)%d = %d\n", getregister(execcom[2]), r2, getregister(execcom[3]), r3, execdata.err[err1]);
                      printf("╔════════════════════════════════════════════════════════════════╗\n CORE %d info:\n process id: %d            PC: 0x%06X           data: [%08X]\n \n Command:\n %s   %s, %s, %s\n |\n  -->  %d + %d = %d\n╚════════════════════════════════════════════════════════════════╝\n", cpu.core[i].core_num, execdata.pid, cpu.core[i].pc, mem[haddr], com, getregister(execcom[1]), getregister(execcom[2]), getregister(execcom[3]), r2, r3, execdata.err[err1]);
                }else if(strstr(com, "st") != NULL){
                      dataaddr = mmu_function(ldataaddr, execdata.ptbr);
                      mem[dataaddr] = execdata.err[erreg];
                      printf("╔════════════════════════════════════════════════════════════════╗\n CORE %d:\n process id: %d            PC: 0x%06X           data: [%08X]\n \n Command:\n %s   %s, %d\n |\n  -->  %s = %d\n╚════════════════════════════════════════════════════════════════╝\n", cpu.core[i].core_num, execdata.pid, cpu.core[i].pc, mem[haddr], com, getregister(execcom[1]), mem[dataaddr], getregister(execcom[1]), mem[dataaddr]);
                }else if(strstr(com, "exit") != NULL){
                      printf("╔════════════════════════════════════════════════════════════════╗\n CORE %d:\n process id: %d            PC: 0x%06X           data: [%08X]\n \n Command:\n %s\n |\n  --> Exiting...\n╚════════════════════════════════════════════════════════════════╝\n", cpu.core[i].core_num, execdata.pid, cpu.core[i].pc, mem[haddr], com);
                }

                //printf("%s %s----------------------------------------------------\n", com, reg);

                //printf("---------core%d---------    thread 1: [ id: %d vruntime: %d ]\n", cpu.core[i].core_num, execdata.pid, execdata.vruntime);
                //printf("╔════════════════════════════════════════════════════════════════╗\n CORE %d:\nthread 1: [ id: %d 0x%06X: [%08X] ]\n╚════════════════════════════════════════════════════════════════╝\n", cpu.core[i].core_num, execdata.pid, cpu.core[i].pc, mem[haddr]);

                execdata.pc+=4;

                vrunt =execdata.vruntime;
                vrunt = vrunt + (param->timer * execdata.decay_factor);
                execdata.vruntime = vrunt;
                execdata.rtime -= param->timer;
                execdata.egoera = 1;
                //inorder(cpu.core[i].root);



                //if(mem[haddr] == -268435456){
                if(strstr(com, "exit") != NULL){
                    cpu.core[i].busy = 0;
                }else if(execdata.rtime > 0){
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
                    //treetam++;
                }
            }else{
                cpu.core[i].root = new_node(nulua);
                printf("╔════════════════════════════════════════════════════════════════╗\n CORE %d:\n process id: %d\n \n [!] No process detected.\n╚════════════════════════════════════════════════════════════════╝\n", cpu.core[i].core_num, nulua.pid);
                //printf("---------core%d---------    thread 1: [ id: %d vruntime: %d ]\n", cpu.core[i].core_num, nulua.pid, nulua.vruntime);
                //treetam++;
            }
            /*for(int k = 0; k < param->core_kop; k++){
                printf("core%d:", k);
                inorder(cpu.core[k].root);
                printf("\n");
            }*/
        }
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
