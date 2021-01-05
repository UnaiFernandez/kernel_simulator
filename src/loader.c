#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#include "define_hariak.h"
#include "tree.h"



struct process_control_block atera;
struct node *root;
volatile int treetam = 0;
struct node *lefmost;

char* getcommand(char command){
    char *com;
    switch (command){
            case '0': com = "ld"; break;
            case '1': com = "st"; break;
            case '2': com = "add"; break;
            case 'F': com = "exit"; break;
            default: abort ();
      }
    return com;
}


char* getregister(char regist){
    char *reg;
    switch (regist){
            case '0': reg = "r0"; break;
            case '1': reg = "r1"; break;
            case '2': reg = "r2"; break;
            case '3': reg = "r3"; break;
            case '4': reg = "r4"; break;
            case '5': reg = "r5"; break;
            case '6': reg = "r6"; break;
            case '7': reg = "r7"; break;
            case '8': reg = "r8"; break;
            case '9': reg = "r9"; break;
            case 'A': reg = "r10"; break;
            case 'B': reg = "r11"; break;
            case 'C': reg = "r12"; break;
            case 'D': reg = "r13"; break;
            case 'E': reg = "r14"; break;
            case 'F': reg = "r15"; break;
            default: abort ();
      }
    return reg;
}


int getNum(char ch)
{
    int num=0;
    if(ch>='0' && ch<='9')
    {
        num=ch-0x30;
    }
    else
    {
        switch(ch)
        {
            case 'A': case 'a': num=10; break;
            case 'B': case 'b': num=11; break;
            case 'C': case 'c': num=12; break;
            case 'D': case 'd': num=13; break;
            case 'E': case 'e': num=14; break;
            case 'F': case 'f': num=15; break;
            default: num=0;
        }
    }
    return num;
}



void getfilename(int line, char *filename[]){
    FILE *fp;
    int i;

    fp = fopen("filenames.txt", "r");
    if(fp == NULL){
        perror("(getfilename)");
    }
    for(i = 0; i < line; i++){
        fgets(filename[i], ID_LEN+1, fp);
        printf("- %s\n", filename[i]);
    }

    fclose(fp);
}

void addrcomm(char *hex){
    int i, j = 2;

    for(i = 0; i < 8; i++){
       hex[i] = hex[j]; 
       j++;
    }
}


void deleteword(char *str){
    int i, j = 6;

    for(i = 0; i < 8; i++){
       str[i] = str[j]; 
    } 
}


void storedata(){

}

/*
 * Process generator funtzioa
 */
void *loader(void *hari_par){

    int j, i, p_kop, p, tam, lnum, kont, dat, num;
    int lower = 0, upper = 40, minrt = 30, maxrt = 250;
    struct hari_param *param;
    struct process_control_block pcb;
    FILE *fp;
    char line[16];
    char *a, *com, *reg, *reg2, *reg3, *fname;
    //char registroa;

    //char *filename = malloc((11+2)*sizeof(char));
    //Hasieraketak
    param = (struct hari_param *)hari_par;
    p_kop = param->p_kop;
    //i = 0;
    j = 0;
    //p = 0;
    //tam = p_kop;
    lefmost = root;
    lnum = 0;
    kont = 0;

    char **filenames = malloc(p_kop*sizeof(char*));
    for(i = 0; i < p_kop; i++){
        filenames[i] = malloc((ID_LEN+1)*sizeof(char));
    }
    //Hariaren hasierako informazioa pantailaratu
    printf("[PROCESS GENERATOR:       id = %d    name = %s   ]\n", param->id, param->name);

        getfilename(j+1, filenames);
    //Funtzioko loop-a
    while(j < p_kop){
    //while(1){
        //Ausazko zenbakiak sortzeko
        srand(tick*time(NULL));

        sem_wait(&semp);


        //char *f;
        printf("[filename] %s\n", filenames[j]);

        DEBUG_WRITE("[PROCESS GENERATOR] tick read! %d\n", tick);
        //Prozesu nulua sortu prozesu kopurura iristen bada.
        pcb.pid = j;//rand() % 100;
        pcb.nice =  (rand() % (upper - lower + 1)) + lower;
        pcb.weight = weight[pcb.nice];
        pcb.decay_factor = (float)weight0/pcb.weight;
        pcb.rtime = 0;
        pcb.vruntime = 0;




        //fp = fopen("prog000.elf", "r");
        //char *f;
        //int a = strcmp(filenames[j], f);
        //printf("%s\n", filenames[j]);
        //printf("%s\n", filenames[j]);
        //printf("%d", a);
        printf("[filename] %ssdlf\n", filenames[j]);
        fp = fopen(filenames[j], "r");
        //fp = fopen("src/prog000.elf", "r");
        if(fp == NULL){
            perror("Could not open file");
        }
        while(fgets(line, sizeof(line), fp) != NULL){
            if(lnum == 0){
                deleteword(line);
                pcb.mm.text = (int)strtol(line, NULL, 16);
                printf(".text %06X\n", pcb.mm.text);
            }else if(lnum == 1){
                deleteword(line);
                pcb.mm.data = (int)strtol(line, NULL, 16);    
            }else{
                if(pcb.mm.data == kont)
                    printf(".data %06X %d\n", pcb.mm.data, pcb.mm.data);
                if(pcb.mm.data > kont){
                    com = getcommand(line[0]);
                    if(strstr(com,"ld") != NULL || strstr(com, "st") != NULL){
                        reg = getregister(line[1]);
                        num = (int)strtol(line, NULL, 16);
                        char *lag;
                        lag = line;
                        addrcomm(lag);
                        dat = (int)strtol(lag, NULL, 16);

                        pcb.vruntime = pcb.vruntime + 7;
                        pcb.rtime = pcb.rtime + 7;
                        
                        printf("0x%06X [%08X]    %s   %s 0x%06X\n", kont, num, com, reg, dat);
                    }else if(strstr(com, "add") != NULL){
                        reg = getregister(line[1]);
                        reg2 = getregister(line[2]);
                        reg3 = getregister(line[3]);
                        
                        pcb.vruntime = pcb.vruntime + 5;
                        pcb.rtime = pcb.rtime + 5;

                        printf("0x%06X [%08X]    %s   %s,%s,%s\n", kont, num, com, reg, reg2, reg3);
                    }else{
                        printf("0x%06X [%08X]    %s\n", kont, num, com);
                    }
                }else{
                    int num = (int)strtol(line, NULL, 16);
                    printf("0x%06X [%08X] %d\n", kont, num, num);
                }
                kont+=4;
            }    
            lnum++;
        }
        printf("rtimes %d = %d, %d\n", pcb.pid, pcb.rtime, pcb.vruntime);
        lnum = 0;
        kont = 0;
	    fclose(fp);
        
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
