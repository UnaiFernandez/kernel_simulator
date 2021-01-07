#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include <time.h>

#include "define_hariak.h"


struct process_control_block *sch_arr;
int sch_arr_tam = 0;
int *arr;
struct cpu cpu;
int mem[MEM_CAP];
struct freequeue *freemem;
sem_t semt, semc, semp;


void sortu_hariak(int hari_kop, int proz_kop, int maiz, int tim, int core_kop){

    int i, j, err, c_kop;
    pthread_t *hariak;
    struct hari_param *h_p;
    struct process_control_block *pcb;


    //Hasieraketak
    cpu.core = malloc(core_kop * sizeof(struct core_t));
    hariak = malloc((hari_kop + core_kop) * sizeof(pthread_t));
    h_p = malloc((hari_kop + core_kop) * sizeof(struct hari_param));
    sch_arr = malloc(proz_kop * sizeof(pcb));
    arr = malloc(core_kop * sizeof(int));
    sch_arr_tam = proz_kop;
    freemem = malloc(MEM_CAP * sizeof(struct freequeue));

    printf("\n");
    printf("\n");
    printf("Creating threads...\n");
    printf("----------------------------------------------------------------\n");

    //Hari bakoitza sortu
    for(i = 0; i < hari_kop; i++){
        if(i == 0){
            h_p[i].name = "Process Generator";
            h_p[i].id = i;
            h_p[i].p_kop = proz_kop;
            h_p[i].core_kop = core_kop;
            err = pthread_create(&hariak[i], NULL, loader, (void *)&h_p[i]);;

            if(err > 0){
                fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
                exit(1);
            }
        }
        if(i == 1){
            h_p[i].name = "Timer";
            h_p[i].id = i;
            h_p[i].timer = tim;
            h_p[i].core_kop = core_kop;
            err = pthread_create(&hariak[i], NULL, timer, (void *)&h_p[i]);;

            if(err > 0){
                fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
                exit(1);
            }
        }
        if(i == 2){
            h_p[i].name = "Clock";
            h_p[i].id = i;
            h_p[i].maiz = maiz;
            err = pthread_create(&hariak[i], NULL, clockfunc, (void *)&h_p[i]);;

            if(err > 0){
                fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
                exit(1);
            }
        }

    }
    c_kop = core_kop + 3;
    for(j = 3; j < c_kop; j++){
        h_p[j].name = "Scheduler/Dispatcher";
        h_p[j].id = j;
        h_p[j].core_kop = core_kop;
        h_p[j].timer = tim;
        h_p[i].p_kop = proz_kop;
        err = pthread_create(&hariak[j], NULL, scheduler_dispatcher, (void *)&h_p[j]);;

        if(err > 0){
            fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
            exit(1);
        }
    }
    sleep(1);
    printf("----------------------------------------------------------------\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("Clock, timer and process signals:\n");
    printf("----------------------------------------------------------------\n");
    hari_kop = hari_kop + core_kop;
    for(i = 0;i < hari_kop-1;i++) // Ume guztiak amaitu arte
        pthread_join(hariak[i], NULL);

    free(hariak);
    free(h_p);
}

int main(int argc, char *argv[]){

    int proz_kop, c, i, maiz, tim, core_kop, maizMHz;
    float periodoa;
    char *p;

    //getopt erabilita parametroak eskuratu
    while ((c = getopt (argc, argv, "p:m:t:c:")) != -1){
        switch (c){
            case 'p':
                proz_kop = atoi(optarg);
                break;
            case 'm':
                maiz = strtol(optarg, &p, 10);
                break;
            case 't':
                tim = strtol(optarg, &p, 10);
                break;
            case 'c':
                core_kop = strtol(optarg, &p, 10);
                break;
            case '?':
                if (optopt == 'p')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort ();
      }
    }

    maizMHz = maiz * 1000000;
    periodoa = (1.0/maizMHz)*1000000000.0;

    printf("\n");
    printf("\n");
    printf("Parametroak:\n");
    printf("----------------------------------------------------------------\n");
    printf("[Maiztasuna: %d MHz", maiz);
    printf("    Periodoa: %.0f ns", periodoa);
    printf("    Prozesu kopurua: %d", proz_kop);
    printf("    Core kopurua: %d]\n", core_kop);
    sem_init(&semt, 0, 0);
    sem_init(&semp, 0, 0);
    sem_init(&semc, 0, 0);
    sortu_hariak(HARIKOP, proz_kop, maizMHz, tim, core_kop);

    sem_destroy(&semp);
    sem_destroy(&semc);
    sem_destroy(&semt);
    for(i = 0;i < proz_kop;i++) // Ume guztiak amaitu arte
        waitpid(-1, NULL, 0);

    free((void *)sch_arr);
    free(freemem);
    return(0);
}
