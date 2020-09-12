#include <stdio.h>

#include "../sharedmem.h"

int main(int argc, char* const argv[]) {
    if (argc != 1)
	return EXIT_FAILURE;

    char* bl = attach_memblock(FILENAME, BL_SIZE);
    if (!bl) {
	printf("error occured\n");
	return EXIT_FAILURE;
    }

    printf("Read: %s\n", bl);

    detach_memblock(bl);

    return EXIT_SUCCESS;
}
