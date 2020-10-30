/*MAIN PROGRAMAKO DEFINIZIOAK*/
#ifndef _define_hariak_h
#define _define_hariak_h

#define HARIKOP 4

#include <semaphore.h>
#include "linkedlist.h"
#define TAM 4


struct hari_param{
    int id;
    char * name;
    int p_kop;
    int maiz;
    int timer;
};

void *scheduler_dispatcher(void *hari_param);
void *process_generator(void *hari_param);
void *timer(void *hari_param);
void *clockfunc(void *hari_param);
void sortu_hariak(int hari_kop, int proz_kop, int maiz, int tim, int core_kop);

struct process_control_block{
    int pid;
    int lehen;
};

// tick en azken balioa irakurtzen dela bermatzeko
extern volatile int tick;
extern volatile int timtick;
/*
#define SEM 1
#ifdef SEM




#endif*/
extern sem_t semt, semc, semp, sems, semc2, semc3;


// prozesu array-a
extern volatile struct process_control_block *sch_arr;

struct core_t{
    node lehentasuna[140];
    node hari_1[1];
    node hari_2[1];
};
#endif

