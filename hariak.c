#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "define_hariak.h"





void sortu_hariak(int hari_kop){

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
    
    //char *p;
    //int i, hari_kop;

    if(argc !=2){
        fprintf(stderr, "Erabilpena: ./4arik hari-kopurua\n");
        exit(1);
    }

    //hari_kop = strtol(argv[1], &p, 10);

    sortu_hariak(HARIKOP);

    return(0);
}
