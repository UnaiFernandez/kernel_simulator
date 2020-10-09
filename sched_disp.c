#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "define_hariak.h"



void *scheduler_dispatcher(void *hari_par){

    struct hari_param *param;

    param = (struct hari_param *)hari_par;
    printf("SCHEDULER/DISPATCHER:\n id = %d    name = %s\n", param->id, param->name);
    pthread_exit(NULL);
}
