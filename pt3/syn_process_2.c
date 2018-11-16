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
    sem_t * sema, * semb; /* The Semaphores */
    int i; /* Loop Counter */
    sema = sem_open("sema", O_CREAT, S_IRUSR, 1); /* Open Semaphore */
    semb = sem_open("semb", O_CREAT, S_IRUSR, 1); /* Open Semaphore */
    sem_wait(sema); /* Lock sema Semaphore */
    if (fork())
    {
        for (i=0;i<10;i++)
        {
            sem_wait(semb); /* Lock semb Semaphore */
    		display("ab"); /* Call Display Function */
    		sem_post(sema); /* unLock sema Semaphore */
        }
        wait(NULL);
    }
    else
    {
        for (i=0;i<10;i++)
        {
            sem_wait(sema); /* Lock sema Semaphore(if possible) */
    		display("cd\n"); /* Call Display Function */
    		sem_post(semb); /* unLock semb Semaphore */
    	}  	  
    }
    sem_close(sema); /* Close Semaphore */
    sem_close(semb); /* Close Semaphore */
    sem_unlink("sema"); /* Unlink Semaphore */
    sem_unlink("semb"); /* Unlink Semaphore */
    sem_destroy(sema); /* Destroy Semaphore */
    sem_destroy(semb); /* Destroy Semaphore */    
    return 0;
}
