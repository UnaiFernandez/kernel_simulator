#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "define_hariak.h"
#include "rbtree.h"




void *scheduler_dispatcher(void *hari_par){

    struct hari_param *param;
    param = (struct hari_param *)hari_par;
    int core_num;
    struct process_control_block exec, txikiena;

    printf("[SCHEDULER/DISPATCHER:    id = %d    name = %s]\n", param->id, param->name);
    sleep(2);
   
    core_num = param->id - 3;

    pthread_mutex_lock(&mutex);
    while(1){
        done++;
        printf("---------core%d---------\n", core_num);
        fflush(stdout);
        printf("[SCHEDULER/DISPATCHER %d] tick read!\n", param->id);
        fflush(stdout);
        inorderTraversal(root);
        printf("\n");
        getLeftMost(root);
        exec = leftMost();
        printf("%d\n", exec.pid);
        deletion(exec);
        inorderTraversal(root);
        printf("\n");
        getLeftMost(root);
        txikiena = leftMost();
        printf("%d\n", txikiena.pid);
        //sem_post(&sems);
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
