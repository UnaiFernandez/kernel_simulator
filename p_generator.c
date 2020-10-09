#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "define_hariak.h"



void *process_generator(void *hari_par){

    struct hari_param *param;

    param = (struct hari_param *)hari_par;
    printf("PROCESS GENERATOR:\n id = %d    name = %s\n", param->id, param->name);
    pthread_exit(NULL);
}
