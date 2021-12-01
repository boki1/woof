#include <stdio.h>

#include "sharedmem.h"

int
main(int argc, char* const argv[])
{
    if (argc != 1)
	return EXIT_FAILURE;

    if (destroy_memblock(FILENAME)) {
	printf("successfully destroyed shared memory block\n");
    } else {
	printf("error occured\n");
    }

    return EXIT_SUCCESS;
}
