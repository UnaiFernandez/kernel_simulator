/*MAIN PROGRAMAKO DEFINIZIOAK*/
#ifndef _define_hariak_h
#define _define_hariak_h

#include <semaphore.h>
#include <pthread.h>

#define HARIKOP 4
#define MAX_CORE_KOP 32
#define TAM 10
#define weight0 1024
#define ADDRTAM 2
#define ID_LEN 30
#define default_filename "prog/prog"
#define MEM_CAP 16777216
//#define MEM_CAP 100
#define orrtau 4194304
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
void *loader(void *hari_param);
void *timer(void *hari_param);
void *clockfunc(void *hari_param);
void sortu_hariak(int hari_kop, int proz_kop, int maiz, int tim, int core_kop);
//memory management
struct mm{
    int text;
    int data;
    int pgb;
};

// Prozesu bakoitzaren parametroak
struct process_control_block{
    int pid;
    int nice;
    int weight;
    int vruntime;
    float decay_factor;
    int egoera;
    int rtime;
    int ptbr;
    struct mm mm;
    int err[16];
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


//Core en prozesu kopurua gordetzeko array-a
extern int busy_arr[MAX_CORE_KOP];

//core struct
struct core_t{
    struct node *root;
    struct node *exec;
    int core_num;
    int busy;
    int treetam;
    struct process_control_block hari1;
    struct process_control_block hari2;
};


struct cpu{
    struct core_t * core;
};

extern struct cpu cpu;
//extern struct node *lefmost;

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
extern pthread_mutex_t lock;


//helbide logikoak
//struct l_addr{
//    int orr_zenb;
//    int desp;
//};
extern int l_addr;
//helbide fisikoa
//struct f_addr{
//    int frame;
//    int desp;
//};
extern int f_addr;

//memoria
//struct mem{
//  int helb;
//  struct frame frame;
//};


//memoriako array
extern int mem[MEM_CAP];


//memoria hutsa non dagoen jakiteko helbideak
struct freequeue{
    int addr;
    int freespace;
};

extern struct freequeue *freemem;

//ir eta pc erregistroak
extern int ir;
extern int pc;
#endif

#ifdef DEBUG
    #define DEBUG_WRITE(x, ...) printf(x, __VA_ARGS__)
#else
    #define DEBUG_WRITE(x, ...)
#endif // DEBUG
