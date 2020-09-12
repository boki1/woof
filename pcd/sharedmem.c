#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "sharedmem.h"

#define IPC_RESULT_ERROR (-1)

static int
get_shared_block(const char* fname, size_t size)
{
    key_t key;
    key = ftok(fname, 0);

    if (key == IPC_RESULT_ERROR)
	return IPC_RESULT_ERROR;

    /** 0644 -> r/w for user and only read for group and others */
    return shmget(key, size, 0644 | IPC_CREAT);
}

char*
attach_memblock(const char* fname, size_t size)
{
    int shmid = get_shared_block(fname, size);
    if (shmid == IPC_RESULT_ERROR)
	return NULL;
    char* result;

    /*  map the shared memory block in my address space and give me a pointer to
     * it.  */
    result = shmat(shmid, NULL, 0);
    if (result == (char*)IPC_RESULT_ERROR)
	return NULL;

    return result;
}

int
detach_memblock(char* block)
{
    return (shmdt(block) != IPC_RESULT_ERROR);
}

int
destroy_memblock(char* fname)
{
    int shmid = get_shared_block(fname, 0);
    if (shmid == IPC_RESULT_ERROR)
	return 0;

    return (shmctl(shmid, IPC_RMID, 0) != IPC_RESULT_ERROR);
}

