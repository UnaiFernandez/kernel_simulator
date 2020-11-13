/*MAIN PROGRAMAKO DEFINIZIOAK*/
#ifndef _define_hariak_h
#define _define_hariak_h

#define HARIKOP 4
#define MAX_CORE_KOP 32

#include <semaphore.h>
#include <pthread.h>
#define TAM 10

#define weight0 1024

// Hariaren parametroak
struct hari_param{
    int id;
    char * name;
    int p_kop;
    int maiz;
    int timer;
    int core_kop;
};

// Main programan erabiltzen diren funtzioak
void *scheduler_dispatcher(void *hari_param);
void *process_generator(void *hari_param);
void *timer(void *hari_param);
void *clockfunc(void *hari_param);
void sortu_hariak(int hari_kop, int proz_kop, int maiz, int tim, int core_kop);

// Prozesu bakoitzaren parametroak
struct process_control_block{
    int pid;
    int weight;
    int vruntime;
    float decay_factor;
};

// tick en azken balioa irakurtzen dela bermatzeko
extern volatile int tick;

// semaforoak
extern sem_t semt, semc, semp;

// timer eta schedulerraren sinkronizaziorako
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern pthread_cond_t cond2;
extern int done;

// prozesu array-a eta bere tamaina
extern struct process_control_block *sch_arr;
extern int sch_arr_tam;

//Core en prozesu kopurua gordetzeko array-a
extern int tam_arr[MAX_CORE_KOP];
//Prozesu gitxien dituen corearen identifikadorea 
extern int minimum;

//core struct
struct core_t{
    struct node *root; 
    int core_num;
    int tamaina;
    struct process_control_block hari1[1];
    struct process_control_block hari2[1];
};

//Weinght-en array-a
static const int weight[40] = {
/* -20 */ 88761, 71755, 56483, 46273, 36291,
/* -15 */ 29154, 23254, 18705, 14949, 11916,
/* -10 */ 9548, 7620, 6100, 4904, 3906,
/* -5 */ 3121, 2501, 1991, 1586, 1277,
/* 0 */ 1024, 820, 655, 526, 423,
/* 5 */ 335, 272, 215, 172, 137,
/* 10 */ 110, 87, 70, 56, 45,
/* 15 */ 36, 29, 23, 18, 15};


#endif

