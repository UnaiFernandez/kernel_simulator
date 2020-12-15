#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "define_hariak.h"
#include "tree.h"


struct process_control_block atera;
struct node *root;
volatile int treetam = 0;
struct node *lefmost;


char* getcommand(char command){
    char *com;
    switch (command){
            case '0':
                com = "ld";
                break;
            case '1':
                com = "st";
                break;
            case '2':
                com = "add";
                break;
            case 'F':
                com = "exit";
                break;
            default:
                abort ();
      }
    return com;
}


char* getregister(char regist){
    char *reg;
    switch (regist){
            case '0':
                reg = "r0";
                break;
            case '1':
                reg = "r1";
                break;
            case '2':
                reg = "r2";
                break;
            case '3':
                reg = "r3";
                break;
            case '4':
                reg = "r4";
                break;
            case '5':
                reg = "r5";
                break;
            case '6':
                reg = "r6";
                break;
            case '7':
                reg = "r7";
                break;
            case '8':
                reg = "r8";
                break;
            case '9':
                reg = "r9";
                break;
            case 'A':
                reg = "r10";
                break;
            case 'B':
                reg = "r11";
                break;
            case 'C':
                reg = "r12";
                break;
            case 'D':
                reg = "r13";
                break;
            case 'E':
                reg = "r14";
                break;
            case 'F':
                reg = "r15";
                break;
            default:
                abort ();
      }
    return reg;
}


double hextodec(char *hex, int len){
    int i, j;
    double dec = 0;
    char dig;
    j = -1;
    for(i = 0; i < len; i++){
        dig = hex[i];
        switch (dig){
            case '0':
                j++;
                dec += 0*pow(16,j);
                break;
            case '1':
                j++;
                dec += 1*pow(16,j);
                break;
            case '2':
                j++;
                dec += 2*pow(16,j);
                break;
            case '3':
                j++;
                dec += 3*pow(16,j);
                break;
            case '4':
                j++;
                dec += 4*pow(16,j);
                break;
            case '5':
                j++;
                dec += 5*pow(16,j);
                break;
            case '6':
                j++;
                dec += 6*pow(16,j);
                break;
            case '7':
                j++;
                dec += 7*pow(16,j);
                break;
            case '8':
                j++;
                dec += 8*pow(16,j);
                break;
            case '9':
                j++;
                dec += 9*pow(16,j);
                break;
            case 'A':
                j++;
                dec += 10*pow(16,j);
                break;
            case 'B':
                j++;
                dec += 11*pow(16,j);
                break;
            case 'C':
                j++;
                dec += 12*pow(16,j);
                break;
            case 'D':
                j++;
                dec += 13*pow(16,j);
                break;
            case 'E':
                j++;
                dec += 14*pow(16,j);
                break;
            case 'F':
                j++;
                dec += 15*pow(16,j);
                break;
            default:
                abort ();
      }
    }
    return dec;
}


void translator(char *command, char* reg, char *dat){
    char *a = command;
    char *b = reg;
    while(*a){
        a++;
    }
    while (*b) {
        *a = *b;
        b++;
        a++;
    }
    *a = '\0';
    printf("command = %s", command);
}

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
    char *a, *dat, *com, *reg;
    char registroa;

    //Hasieraketak
    param = (struct hari_param *)hari_par;
    p_kop = param->p_kop;
    i = 0;
    j = 0;
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
                printf(".text %06X\n", pcb.mm.text);
            }else if(lnum == 1){
                deleteword(line);
                pcb.mm.data = (int)strtol(line, NULL, 16);    
            }else{
                //printf("line = %s", line);
                //printf("char = %c\n", line[7]);
                if(pcb.mm.data == kont)
                    printf(".data %06X\n", pcb.mm.data);
                if(pcb.mm.data > kont){
                    com = getcommand(line[0]);
                    reg = getregister(line[1]);
                    int num = (int)strtol(line, NULL, 16);
                    int numint = hextodec(line, 8);
                    printf("0x%06X [%08X][%d]     %s,  %s\n", kont, num, numint, com, reg);
                }else{
                    int num = (int)strtol(line, NULL, 16);
                    printf("0x%06X [%08X]\n", kont, num);
                }
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
