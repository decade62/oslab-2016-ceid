/* Included system libs */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
/* Included custom lib */
#include "display.h"

int main()
{
    sem_t * sema; /* The Semaphore */
    int i; /* Loop Counter */
    sema = sem_open("sema", O_CREAT, S_IRUSR, 1); /* Open Semaphore */
    if (fork())
    {
        for (i=0;i<10;i++)
        {
        	sem_wait(sema); /* Lock Semaphore */
    		display("Hello world\n"); /* Call Display Function */
    		sem_post(sema); /* unLock Semaphore */
            sched_yield();
        }
        wait(NULL); /* Wait Proc */
    }
    else
    {
        for (i=0;i<10;i++)
        {
            sem_wait(sema); /* Lock Semaphore */
    		display("Kalimera kosme\n"); /* Call Display Function */
    		sem_post(sema); /* unLock Semaphore */
    	}  	  
    }
    sem_close(sema); /* Close Semaphore */
    sem_unlink("sema"); /* Unlink Semaphore */
    sem_destroy(sema); /* Destroy Semaphore */
}
