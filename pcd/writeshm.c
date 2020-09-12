#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>

#include "sharedmem.h"

int
main (int argc, char *const argv[])
{
    if (argc != 2)
	{
	    return EXIT_FAILURE;
	}

    int ITERATION_COUNTER = !strcmp (argv[1], "quit") ? 1 : 10;

    char *bl = attach_memblock (FILENAME, BL_SIZE);
    if (!bl)
	{
	    puts ("error occured");
	    return EXIT_FAILURE;
	}

    sem_t *sem_prod = sem_open (SEM_PRODUCER_FNAME, 0);
    if (sem_prod == SEM_FAILED)
	{
	    puts ("err: sem_open/producer");
	    exit (EXIT_FAILURE);
	}

    sem_t *sem_cons = sem_open (SEM_CONSUMER_FNAME, 0);
    if (sem_cons == SEM_FAILED)
	{
	    puts ("err: sem_open/consumer");
	    exit (EXIT_FAILURE);
	}

    for (int i = 0; i < ITERATION_COUNTER; ++i)
	{
	    sem_wait (sem_cons);
	    printf ("Writing...%s\n", argv[1]);
	    strncpy (bl, argv[1], BL_SIZE);
	    sem_post (sem_prod);
	}

    sem_close (sem_prod);
    sem_close (sem_cons);

    printf ("Done.\n");
    detach_memblock (bl);

    return EXIT_SUCCESS;
}
