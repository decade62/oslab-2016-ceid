#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "display.h"

void *kosme();
void *world();

pthread_mutex_t sem_mut = PTHREAD_MUTEX_INITIALIZER;

void main(){
  pthread_t thread[2];
  pthread_create(&thread[0], NULL, kosme, NULL);
  pthread_create(&thread[1], NULL, world, NULL);
  pthread_join(thread[1], NULL);
  pthread_join(thread[0], NULL);
  pthread_mutex_destroy(&sem_mut);
}

void *kosme(){
  int i;
  for (i=0;i<10;++i) {
    pthread_mutex_lock(&sem_mut);
    display("Kalimera kosme\n");
    pthread_mutex_unlock(&sem_mut);
  }
  pthread_exit(NULL);
}

void *world(){
  int i;
  for (i=0;i<10;++i) {
    pthread_mutex_lock(&sem_mut);
    display("Hello world\n");
    pthread_mutex_unlock(&sem_mut);
  }
  pthread_exit(NULL);
}