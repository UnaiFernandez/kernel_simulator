#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "define_hariak.h"



void *clockfunc(void *hari_par){

    struct hari_param *param;

    param = (struct hari_param *)hari_par;
    printf("CLOCK:\n id = %d    name = %s\n", param->id, param->name);
    pthread_exit(NULL);
}