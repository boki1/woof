#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>

#include "sharedmem.h"

int
main(int argc, char* const argv[])
{
    if (argc != 1)
	return EXIT_FAILURE;

    char* bl = attach_memblock(FILENAME, BL_SIZE);
    if (!bl) {
	printf("error occured\n");
	return EXIT_FAILURE;
    }

    sem_unlink(SEM_PRODUCER_FNAME);
    sem_unlink(SEM_CONSUMER_FNAME);

    sem_t* sem_prod = sem_open(SEM_PRODUCER_FNAME, O_CREAT, 0660, 0);
    if (sem_prod == SEM_FAILED) {
	puts("err: sem_open/producer");
	exit(EXIT_FAILURE);
    }

    sem_t* sem_cons = sem_open(SEM_CONSUMER_FNAME, O_CREAT, 0660, 1);
    if (sem_cons == SEM_FAILED) {
	puts("err: sem_open/consumer");
	exit(EXIT_FAILURE);
    }

    while (1) {
	sem_wait(sem_prod);
	if (strlen(bl) > 0) {
	    printf("Read: %s\n", bl);
	    int done = !strcmp(bl, "quit");
	    bl[0] = 0;
	    if (done) {
		break;
	    }
	}
	sem_post(sem_cons);
    }

    sem_close(sem_prod);
    sem_close(sem_cons);
    detach_memblock(bl);

    return EXIT_SUCCESS;
}

