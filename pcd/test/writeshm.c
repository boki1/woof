#include <stdio.h>
#include <string.h>

#include "../sharedmem.h"

int
main(int argc, char* const argv[])
{
    if (argc != 2) {
	return EXIT_FAILURE;
    }

    char* bl = attach_memblock(FILENAME, BL_SIZE);
    if (!bl) {
	puts("error occured");
	return EXIT_FAILURE;
    }

    printf("Writing...\n");
    strncpy(bl, argv[1], BL_SIZE);

    printf("Done. Written: %s\n", bl);
    detach_memblock(bl);

    return EXIT_SUCCESS;
}
