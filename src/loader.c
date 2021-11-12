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


/*
* zenbakiaren arabera komando bat bueltatzen du
*/
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

/*
* zenbaki baten arabera erregistroa bueltatzen du
*/
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


/*
*  aginduaren helbidea lortzeko
*/
void addrcomm(char *hex){
    int i, j = 2;

    for(i = 0; i < 8; i++){
       hex[i] = hex[j];
       j++;
    }
}

/*
*  .text eta .data hitzak ezabatzeko
*/
void deleteword(char *str){
    int i, j = 6;

    for(i = 0; i < 8; i++){
       str[i] = str[j];
    }
}

/*
* Memoria hasieratzeko funtzioa
*/
void initmem(){
    int i;

    for(i = 0; i < MEM_CAP; i++)
        mem[i] = 0;
}


/*
* Memorian dauden hutsuneen ilara hasieratu
*/
void initfreequeue(){
    struct freequeue fq;

    fq.addr = 0;
    fq.freespace = MEM_CAP;

    freemem[0] = fq;
}


/*
*  Memorian dauden hutsuneen ilara lortu
*/
void getfreequeue(int p_kop, int addr){
    int i, j, first, kont;
    struct freequeue fq;
    i = 0;
    first = 0;
    fq.addr = 0;
    fq.freespace = 0;

    while(i < p_kop){
        kont = 1;
        for(j = addr; j < orrtau; j++){
            if(mem[j] == 0 && first == 0){
                fq.addr = j;
                first = 1;
            }else if (mem[j] == 0 && first == 1)
                kont++;
            else if(mem[j] != 0 && first == 1){
                first = 0;
                addr = j;
                break;
            }
            fq.freespace = kont;
            freemem[i] = fq;
        }
        if(j < orrtau)
            i++;
        else
            break;
    }
printf("freemem [");
i = 0;
while(i < p_kop){
    printf("(%d, %d)", freemem[i].addr, freemem[i].freespace);
    i++;
}
printf("]\n");
}


/*
*  Memorian procesuaren datuak sartzeko lekua dagoen bilatzeko
*/
int checkmemspace(int progsize, int p_kop){
    int i, addr;
    addr = -1;

    for(i = 0; i < p_kop; i++){
        if(freemem[i].freespace > progsize*4){
            addr = freemem[i].addr;
            break;
        }
    }
    return addr;
}


/*
*  memorian datu bat gordetzeko
*/
void storedata(int addr, int data){
    if(addr != -1)
        mem[addr] = data;


}

/*
 * Process generator funtzioa
 */
void *loader(void *hari_par){

    int j, i, p_kop, p, tam, lnum, kont, dat, num, progsize, memspace, framekont, orrikont;
    int lower = 20, upper = 40, minrt = 30, maxrt = 250;
    struct hari_param *param;
    struct process_control_block pcb;
    FILE *fp;
    char line[16], file_name[16];
    char *a, *com, *reg, *reg2, *reg3;
    double progsize_bit;
    //char registroa;

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
    progsize = 0;
    progsize_bit = 0;
    framekont = 0;
    orrikont = -4;


    //Hariaren hasierako informazioa pantailaratu
    printf("[PROCESS GENERATOR:       id = %d    name = %s   ]\n", param->id, param->name);

    //memoria hasieratu
    initmem();
    initfreequeue();


    //Funtzioko loop-a
    while(j < p_kop){
    //while(1){
        //Ausazko zenbakiak sortzeko
        srand(tick*time(NULL));

        sem_wait(&semp);

        DEBUG_WRITE("orritaula = %06X\n", orrtau);

        //Fitxategiaren izena osatu
        sprintf(file_name, "%s%03d.elf\0", default_filename, j);

        DEBUG_WRITE("[PROCESS GENERATOR] tick read! %d\n", tick);
        //Prozesu bat sortu
        int randomzenb;
        for(int a = 0; a < 20; a++){
            randomzenb = (rand() % (upper - lower + 1)) + lower;
        }
        pcb.pid = j;
        pcb.nice = randomzenb;
        pcb.weight = weight[pcb.nice];
        pcb.decay_factor = (float)weight0/pcb.weight;
        pcb.rtime = 0;
        pcb.vruntime = 0;
        pcb.pc = 0;


        //libre dauden memoria zatiak lortu
        getfreequeue(p_kop, memspace);
        printf("\n");
        printf("\n");


        printf("%s\n", file_name);
        
        //fitxategiak dituen lerroak kontatu tamaina kalkulatzeko
        fp = fopen(file_name, "r");
        //fp = fopen("prog/prog002.elf", "r");
        if(fp == NULL)
            perror("Could not open file");
        while(fgets(line, sizeof(line), fp) != NULL){
            progsize++;
        }
        fclose(fp);



        progsize = progsize - 2;
        pcb.tamaina = progsize;
        progsize = progsize*4;
        progsize_bit = progsize * 0.125;
        printf("    -programaren tamaina: %0.1fB\n", progsize_bit);
        memspace = checkmemspace(progsize, p_kop);
        printf("    -memspace: 0x%06X\n", memspace);
        progsize = 0;
        progsize_bit = 0;


        if(memspace != -1){
        //fitxategiko lerro bakoitza prozesatu
        fp = fopen(file_name, "r");
	printf(":::::::::::::::::: Filename:%s\n", file_name);
        //fp = fopen("prog/prog002.elf", "r");
        //fp = fopen("src/prog000.elf", "r");
        if(fp == NULL){
            perror("Could not open file");
        }
        printf("╔════════════════════════════════════════╗\n");
        while(fgets(line, sizeof(line), fp) != NULL){
            if(lnum == 0){
                //Lehenengo lerroan dagoen .text balioa gorde
                deleteword(line);
                pcb.mm.text = (int)strtol(line, NULL, 16);
                printf(".text %06X\n", pcb.mm.text);
            }else if(lnum == 1){
                //bigarren lerroan dagoen .data balioa gorde
                deleteword(line);
                pcb.mm.data = (int)strtol(line, NULL, 16);
            }else{
                if(pcb.mm.data == kont)
                    printf(".data %06X %d\n", pcb.mm.data, pcb.mm.data);
                if(pcb.mm.data > kont){
                    com = getcommand(line[0]);
                    //ld eta st komandoak
                    if(strstr(com,"ld") != NULL || strstr(com, "st") != NULL){
                        reg = getregister(line[1]);
                        num = (int)strtol(line, NULL, 16);
                        char *lag;
                        lag = line;
                        addrcomm(lag);
                        dat = (int)strtol(lag, NULL, 16);

                        pcb.vruntime = pcb.vruntime + 7;
                        pcb.rtime = pcb.rtime + 7;

                        if(lnum == 3){
                            pcb.ptbr = orrtau + orrikont;
                        }
                        //datua memorian gorde
                        storedata(memspace, num);
                        if(memspace != -1){
                            if(framekont == 0){
                                orrikont+=4;
                                //printf("memspace: %d %08X   ", orrtau + orrikont, memspace);
                                mem[orrtau + orrikont] = memspace;
                                DEBUG_WRITE("%06X      \n", mem[orrtau + orrikont]);
                                framekont++;
                                memspace+=4;
                            }else if(framekont == 4){
                                orrikont+=4;
                                //printf("memspace: %d %08X   ", orrtau + orrikont, memspace);
                                mem[orrtau + orrikont] = memspace;
                                framekont = 1;
                                memspace+=4;
                            }else{
                                framekont++;
                                memspace+=4;
                            }
                        }

                        printf("0x%06X [%08X]    %s   %s 0x%06X\n", kont, num, com, reg, dat);
                    //add komandoa
                    }else if(strstr(com, "add") != NULL){
                        //erregistroak lortu
                        num = (int)strtol(line, NULL, 16);
                        reg = getregister(line[1]);
                        reg2 = getregister(line[2]);
                        reg3 = getregister(line[3]);

                        pcb.vruntime = pcb.vruntime + 5;
                        pcb.rtime = pcb.rtime + 5;

                        //datua memorian gorde
                        storedata(memspace, num);
                        if(memspace != -1){
                            if(framekont == 0){
                                orrikont+=4;
                                mem[orrtau + orrikont] = memspace;
                                DEBUG_WRITE("%06X     \n ", mem[orrtau + orrikont]);
                                framekont++;
                                memspace+=4;
                            }else if(framekont == 4){
                                orrikont+=4;
                                mem[orrtau + orrikont] = memspace;
                                framekont = 1;
                                memspace+=4;
                            }else{
                                framekont++;
                                memspace+=4;
                            }
                        }
                        printf("0x%06X [%08X]    %s   %s,%s,%s\n", kont, num, com, reg, reg2, reg3);
                    //exit komandia
                    }else if(strstr(com, "exit") != NULL){
                        num = (int)strtol(line, NULL, 16);
                        
                        pcb.vruntime = pcb.vruntime + 3;
                        pcb.rtime = pcb.rtime + 3;
                        
                        //datua memorian gorde
                        storedata(memspace, num);
                        if(memspace != -1){
                            if(framekont == 0){
                                orrikont+=4;
                                mem[orrtau + orrikont] = memspace;
                                DEBUG_WRITE("%06X      \n", mem[orrtau + orrikont]);
                                framekont++;
                                memspace+=4;
                            }else if(framekont == 4){
                                orrikont+=4;
                                mem[orrtau + orrikont] = memspace;
                                framekont = 1;
                                memspace+=4;
                            }else{
                                framekont++;
                                memspace+=4;
                            }
                        }
                        printf("0x%06X [%08X]    %s\n", kont, num, com);
                    }
                }else{
                    int num = (int)strtol(line, NULL, 16);

                    //datua memorian gorde
                    storedata(memspace, num);
                        if(memspace != -1){
                            if(framekont == 0){
                                orrikont+=4;
                                mem[orrtau + orrikont] = memspace;
                                DEBUG_WRITE("%06X      \n", mem[orrtau + orrikont]);
                                framekont++;
                                memspace+=4;
                            }else if(framekont == 4){
                                orrikont+=4;
                                mem[orrtau + orrikont] = memspace;
                                framekont = 1;
                                memspace+=4;
                            }else{
                                framekont++;
                                memspace+=4;
                            }
                        }
                    printf("0x%06X [%08X] %d\n", kont, num, num);
                }
                kont+=4;
            }
            lnum++;
            DEBUG_WRITE("orrikont: %d\n", orrikont);
        }
        printf("ptbr: %d  %06X\n", pcb.ptbr, pcb.ptbr);



        printf("prog%d zikloak: %d, %d\n", pcb.pid, pcb.rtime, pcb.vruntime);
        printf("╚═════════════════════════════════════════╝\n\n");
        lnum = 0;
        kont = 0;
	    fclose(fp);

        DEBUG_WRITE("[PROCESS GENERATOR] id: %d vruntime: %d \n", pcb.pid, pcb.weight);

        //Prozesu bat dagokion zuhaitzean sartu.
            if(cpu.core[i].root != NULL){
                insert(cpu.core[i].root, pcb);
                cpu.core[i].treetam++;
            }else{
                cpu.core[i].root = new_node(pcb);
                cpu.core[i].treetam++;
            }
        i = (i + 1) % param->core_kop;
        }else{
            printf("[ERR]  There's no space in the memory!\n");
            printf("\n");
        }
        framekont = 4;

        j++;
    }
    for(i = 0; i < 300; i+=4)
        DEBUG_WRITE("memspace: %08X %08X\n", i, mem[i]);


    DEBUG_WRITE("\n");
    DEBUG_WRITE("\n");
    DEBUG_WRITE("\n");

    for(j = orrtau; j < orrtau + 100; j+=4)
        DEBUG_WRITE("marko: %d %08X\n", j, mem[j]);


    /*for(int k = 0; k < param->core_kop; k++){
        printf("core%d:", k);
        inorder(cpu.core[k].root);
        printf("\n");
    }*/

    pthread_exit(NULL);
}
