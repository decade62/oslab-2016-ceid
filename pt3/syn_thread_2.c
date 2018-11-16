#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "display.h"

void *ab();
void *cd();

pthread_mutex_t sem_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_hand = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int shmaia = 1;

void main(){
  pthread_t threads[2];
  pthread_mutex_init(&sem_mut, NULL);
  pthread_mutex_init(&mutex_hand, NULL);
  pthread_mutex_lock(&mutex_hand);
  pthread_create(&threads[0],NULL,ab,NULL);
  pthread_create(&threads[1],NULL,cd,NULL);
  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);
  pthread_mutex_destroy(&mutex_hand);
  pthread_mutex_destroy(&sem_mut);
}

void *ab(){
  int i;
  for (i=0;i<10;++i){
    pthread_mutex_lock(&sem_mut);
    while( shmaia == 0 )
      pthread_cond_wait(&condition,&sem_mut);
    display("ab");
    shmaia = 0;
    pthread_mutex_unlock(&mutex_hand); 
    pthread_mutex_unlock(&sem_mut);
  }
  pthread_exit(NULL);
}

void *cd(){
  int i;
  for (i=0;i<10;++i){
    pthread_mutex_lock(&mutex_hand);
    display("cd\n");
    shmaia = 1;
    pthread_cond_signal(&condition);
  }
  pthread_exit(NULL);
}
