#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

#include "define_hariak.h"
void * prozesuaSortu1(void * var){
    int thread_id;
    struct hari_param *param;
    
    param = (struct hari_param *)var;
    thread_id = param->id;
    printf("th%d\n", thread_id);
    pthread_exit(NULL);
}
void * prozesuaSortu2(void * var){
    int thread_id;
    struct hari_param *param;
    
    param = (struct hari_param *)var;
    thread_id = param->id;
    printf("th%d\n", thread_id);
    pthread_exit(NULL);
}
void * prozesuaSortu3(void * var){
    int thread_id;
    struct hari_param *param;
    
    param = (struct hari_param *)var;
    thread_id = param->id;
    printf("th%d\n", thread_id);
    pthread_exit(NULL);
}
void * prozesuaSortu4(void * var){
    int thread_id;
    struct hari_param *param;
    
    param->id = (int)var;
    thread_id = param->id;
    printf("th%d\n", thread_id);
    pthread_exit(NULL);
}
void sortzailea(int hari_kop){
    /*create therad id*/
    pthread_t th1, th2, th3, th4;
    int ret1, ret2, ret3, ret4, id1 = 1, id2 = 2, id3 = 3, id4 = 4;

    ret1 = pthread_create(&th1, NULL, &prozesuaSortu1, (void *)id1); 
    ret2 = pthread_create(&th2, NULL, &prozesuaSortu2, (void *)id2); 
    ret3 = pthread_create(&th3, NULL, &prozesuaSortu3, (void *)id3);
    ret4 = pthread_create(&th4, NULL, &prozesuaSortu4, (void *)id4); 
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL); 
    pthread_join(th4, NULL);
}



int main(int argc, char *argv[]){
    int hari_kop;

    hari_kop = HARIKOP;
    printf("%d\n", HARIKOP);
    sortzailea(hari_kop);

    exit(0);
}
