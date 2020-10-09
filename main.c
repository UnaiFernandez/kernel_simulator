#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>

#include "define_hariak.h"





void sortu_hariak(int hari_kop, int proz_kop){

    int i, err;
    pthread_t *hariak;
    struct hari_param *h_p;

    hariak = malloc(hari_kop * sizeof(pthread_t));
    h_p = malloc(hari_kop * sizeof(struct hari_param));

    for(i = 0; i < hari_kop; i++){
        if(i == 0){
            h_p[i].name = "Scheduler/Dispatcher";
            h_p[i].id = i;
            err = pthread_create(&hariak[i], NULL, scheduler_dispatcher, (void *)&h_p[i]);;

            if(err > 0){
                fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
                exit(1);
            }
        }
        if(i == 1){
            h_p[i].name = "Process Generator";
            h_p[i].id = i;
            h_p[i].p_kop = proz_kop;
            err = pthread_create(&hariak[i], NULL, process_generator, (void *)&h_p[i]);;

            if(err > 0){
                fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
                exit(1);
            }
        }
        if(i == 2){
            h_p[i].name = "Timer";
            h_p[i].id = i;
            err = pthread_create(&hariak[i], NULL, timer, (void *)&h_p[i]);;

            if(err > 0){
                fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
                exit(1);
            }
        }
        if(i == 3){
            h_p[i].name = "Clock";
            h_p[i].id = i;
            err = pthread_create(&hariak[i], NULL, clockfunc, (void *)&h_p[i]);;

            if(err > 0){
                fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
                exit(1);
            }
        }
    }

    for(i = 0;i < hari_kop;i++) // Ume guztiak amaitu arte 
        pthread_join(hariak[i], NULL);

    free(hariak);
    free(h_p);
}

int main(int argc, char *argv[]){

    int proz_kop, c, i;

    while ((c = getopt (argc, argv, "p:")) != -1){
        switch (c){
            case 'p':
                proz_kop = atoi(optarg);
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

    sortu_hariak(HARIKOP, proz_kop);

    for(i = 0;i < proz_kop;i++) // Ume guztiak amaitu arte 
        waitpid(-1, NULL, 0);

    return(0);
}
